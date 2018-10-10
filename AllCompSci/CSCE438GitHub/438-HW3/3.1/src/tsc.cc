#include <iostream>
#include <thread>
#include <string>
#include <unistd.h>
#include <mutex>
#include <grpc++/grpc++.h>
#include "tsn.grpc.pb.h"
#include "client.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using tsn::ConnectRequest;
using tsn::ConnectReply;
using tsn::CreateRequest;
using tsn::CreateReply;
using tsn::PersonRequest;
using tsn::PersonReply;
using tsn::ListRequest;
using tsn::ListReply;
using tsn::TimelineStream;
using tsn::SNetwork;

using namespace std;

// Whether to print debug messages
bool debug = false;

// Mutex for when calling reconnect in different threads
mutex connect_mutex;

class Client : public IClient {
	private:
		string hostname, port, username;
		unique_ptr<SNetwork::Stub> stub_;
		map<string, IStatus> status_map;
	protected:
		virtual void openChannel(const string&, const string&);
	public:
		virtual IReply processCommand(string&);
		virtual void processTimeline();
		virtual int connectTo();
		Client(const string& h, const string& p, const string& u) : hostname(h), port(p), username(u), stub_(nullptr) {
			status_map["SUCCESS"] = SUCCESS;
			status_map["FAILURE_ALREADY_EXISTS"] = FAILURE_ALREADY_EXISTS;
			status_map["FAILURE_NOT_EXISTS"] = FAILURE_NOT_EXISTS;
			status_map["FAILURE_INVALID_USERNAME"] = FAILURE_INVALID_USERNAME;
			status_map["FAILURE_INVALID"] = FAILURE_INVALID;
			status_map["FAILURE_UNKNOWN"] = FAILURE_UNKNOWN;
		}
};

int main(int argc, char** argv){
	// Defaults for lazy users
	string host = "localhost", port = "1413", user = "default";
	char opt;
	while((opt = getopt(argc, argv, "h:u:p:")) != -1){
		if(opt == 'h') host = optarg;
		else if(opt == 'u') user = optarg;
		else if(opt == 'p') port = optarg;
		else cerr << "Invalid Command Line Argument\n";
	}
	if(debug) cout << "Finished parsing input args\n";

	// Create the client with the provided address and username
	Client(host, port, user).run();
}

void Client::openChannel(const string& host, const string& port){
	if(debug) cout << "Opening new channel: " << host << ':' << port << '\n';
	this->stub_ = tsn::SNetwork::NewStub(CreateChannel(host+":"+port,
			grpc::InsecureChannelCredentials()));
}

int Client::connectTo(){
	openChannel(this->hostname, this->port);
	// Ask for the host & port of the Available Master
	ClientContext context1;
	ConnectRequest router_request;
	router_request.set_my_addr("");// Not used by connecting clients,
	router_request.set_my_port("");// They are used by connecting servers.
	ConnectReply router_reply;
	Status status1 = this->stub_->connect(&context1, router_request, &router_reply);
	if(!status1.ok() || router_reply.status() != "SUCCESS"){
		return 1;// Unable to reach Routing Server
	}
	openChannel(router_reply.host(), router_reply.port());

	// Attempt to create/join a user with the given name
	ClientContext context;
	CreateRequest request;
	CreateReply reply;
	request.set_username(this->username);
	Status status = this->stub_->create_user(&context, request, &reply);

	if(status.ok()){
		// It's okay if we already exist in the TSN;
		// it supports logging in from multiple locations.
		if(reply.status() == "SUCCESS" || reply.status() == "FAILURE_ALREADY_EXISTS"){
			if(debug) cout << "Connected to an available server!\n";
			return 0;
		}
	}
	// There was an issue with grpc or we got a bad status
	if(debug) cout << "Unable to connect to an available server!\n";
	return 1;
}

IReply Client::processCommand(string& input){
	// Grab the command out of the input string
	string command = input.substr(0, input.find(" "));
	string uname = "";

	// CAPS-ify the command
	//toUpperCase(command);
	//for(int i=0; command[i]; ++i) command[i] = toupper(command[i]);

	// struct to be populated when the server sends a reply
	IReply ire;
	string server_status = "FAILURE_INVALID";
	while(true){
		if(debug) cout << "Sending command to server\n";
		// Some generic GRPC variables for later use
		ClientContext context;
		Status status;

		if(command == "FOLLOW" || command == "UNFOLLOW"){
			// Get the username of the person we want to follow
			uname = input.substr(input.find(" ") + 1, input.length());

			PersonRequest request;
			request.set_requestuser(this->username);
			request.set_targetuser(uname);

			PersonReply reply;
			if(command == "FOLLOW")
				status = this->stub_->follow(&context, request, &reply);
			else
				status = this->stub_->unfollow(&context, request, &reply);

			ire.grpc_status = status;
			server_status = reply.status();
		}
		else if(command == "LIST"){
			ListRequest request;
			request.set_username(this->username);
			ListReply reply;
			status = this->stub_->list(&context, request, &reply);

			// Move all users to the ire struct
			for(string user : reply.users()) ire.all_users.push_back(user);

			// Move all followers to the ire struct
			for(string user : reply.followers()) ire.followers.push_back(user);

			ire.grpc_status = status;
			server_status = reply.status();
		}
		else if(command == "TIMELINE"){
			processTimeline();
			return ire;
		}
		if(status.ok()) break;
		connectTo();
	}

	// Set the appropriate status and return the IReply struct
	ire.comm_status = this->status_map[server_status];
	return ire;
}

typedef std::shared_ptr<ClientReaderWriter<TimelineStream, TimelineStream>> timeline_stream;
void Client::processTimeline(){
	string uname = this->username;

	// Create the bidirectional stream
	ClientContext context;
	timeline_stream stream(stub_->timeline(&context));

	// Start of by just sending my username
	TimelineStream t;
	t.set_username(this->username);
	stream->Write(t);

	// Thread for user input
	thread user_in([&](){
		TimelineStream t;
		string msg;
		while(true){
			msg = getPostMessage();
			msg[msg.size()-1] = '\0';//remove trailing newline
			t.set_username(uname);
			t.set_post(msg);
			t.set_time("");//This will be done server-side
			while(stream->Write(t) == false){
				if(debug) cout << "Lost connection, reconnecting...\n";
				usleep(500000);
				connect_mutex.lock();
				connectTo();
				connect_mutex.unlock();
				ClientContext context;
				stream = timeline_stream(stub_->timeline(&context));
				usleep(500000);
			}
			if(debug) cout << "Sent timeline post!\n";
		}
		stream->WritesDone();
	});

	// Thread for incoming message output
	thread serv_out([&](){
		while(true){
			TimelineStream t;
			while(stream->Read(&t)){
				if(debug) cout << "Got timeline post!\n";
				struct tm tm;
				strptime(t.time().c_str(), "%d-%m-%Y %H-%M-%S", &tm);
				time_t time = mktime(&tm);
				displayPostMessage(t.username(), t.post(), time);
			}
			/*
			if(debug) cout << "Lost connection2, reconnecting...\n";
			usleep(1000000);
			connect_mutex.lock();
			connectTo();
			connect_mutex.unlock();
			ClientContext context;
			stream = timeline_stream(stub_->timeline(&context));
			*/
		}
	});

	// Block until both threads have finished.
	// Since that will never happen, this effectively keeps us in Timeline-mode
	// Thus, to exit the program from this state you must run Ctrl-C
	user_in.join();
	serv_out.join();
}
