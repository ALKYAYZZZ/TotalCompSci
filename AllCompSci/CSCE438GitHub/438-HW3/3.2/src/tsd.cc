#include <iostream>
#include <mutex>
#include <queue>
#include "tsd_utils.h"

using grpc::ClientReaderWriter;
using grpc::ClientContext;
using namespace std;

// TODO: Available Server needs to send its data to other servers, and if they
// are down it needs to keep a queue (or somesuch) of the data that that server
// is missing out on so that when it comes back online it can be updated

// Boolean for if this server is the Available Master Server (AMS)
bool AMS = false;

// Whether to print debug messages
bool debug = true;

// Connection tor Routing Server
StubPtr stub_;

// List of other servers in the TSN (used only if I am elected as AMS)
map<pair<string, string>, StubPtr> servers;
map<pair<string, string>, queue<string>> missed_msgs;

// Need a mutex for the server list (as AMS)
mutex servers_mutex;

void connect_to_router(const std::string& h, const std::string& p,
		const std::string& my_host, const std::string& my_port)
{
	// Create a client stub with which to send messages to Router
	//if(debug) cout << "Connecting to router\n";
	stub_ = connect_to(h, p);

	// A means of identifying myself
	//const pair<string, string> my_hostport = make_pair(my_host, my_port);

	// Thread to ping router every ~1 second
	std::thread([&](){ while(true){
		usleep(500000);// Don't overwhelm the router server with pings

		// These need to be redefined every time or GRPC complains.
		ClientContext context;
		ConnectRequest request;
		ConnectReply reply;
		request.set_my_addr(my_host), request.set_my_port(my_port);

		Status status = stub_->connect(&context, request, &reply);

		if(status.ok() && reply.status() == "SUCCESS"){
			//if(debug) cout << "Pinged the router!\n";
			if(reply.host() == my_host && reply.port() == my_port){
				if(!AMS){
					AMS = true;
					if(debug) cout << "I, " << my_host << ":" << my_port
								<< ", have been elected master!\n";
				}
				// Since I am the AMS, I need to know about other servers
				vector<string> hosts, ports;
				for(string s : reply.other_hosts()) hosts.push_back(s);
				for(string p : reply.other_ports()) ports.push_back(p);
				servers_mutex.lock();
				for(int i=0; i<hosts.size(); ++i){
					if(hosts[i] != my_host || ports[i] != my_port){
						pair<string,string> hostport = make_pair(hosts[i], ports[i]);
						// Only add new servers
						if(!servers.count(hostport)){
							servers[hostport] = connect_to(hosts[i], ports[i]);
						}
					}
				}
				servers_mutex.unlock();
			}
		}
		else if(debug) cout << "Unable to reach the router!\n";
	}}).detach();
}

// Server Implementation
class SNetworkServiceImpl final : public SNetwork::Service {
	Status create_user(ServerContext*, const CreateRequest* request, CreateReply* reply) override {
		// If the user is already logged on from elsewhere
		if(debug) cout << "User joined: " << request->username() << endl;
		if(users.count(request->username())){
			reply->set_status("FAILURE_ALREADY_EXISTS");
		}
		else if(request->username().empty()){
			reply->set_status("FAILURE_INVALID_USERNAME");
		}
		else{
			add_user(request->username());

			reply->set_status("SUCCESS");
			if(AMS){
				servers_mutex.lock();
				for(auto& server : servers){
					ClientContext context;
					CreateReply reply;
					server.second->create_user(&context, *request, &reply);
				}
				servers_mutex.unlock();
			}
		}
		return Status::OK;
	}

	Status follow(ServerContext*, const PersonRequest* request, PersonReply* reply) override {
		string watcher = request->requestuser();
		string watched = request->targetuser();

		// Can't follow yourself or someone who doesn't exist!
		if(watcher == watched || !users.count(watched)){
			reply->set_status("FAILURE_INVALID_USERNAME");
		}
		// You are already following this person!
		else if(is_following(watcher, watched)){
			reply->set_status("FAILURE_ALREADY_EXISTS");
		}
		// Good to go! Add this person as a follower
		else{
			add_follower(watcher, watched);
			reply->set_status("SUCCESS");
			if(AMS){
				if(debug) cout << "Forwarding follow() command to other servers\n";
				// Forward this command to other servers so they stay updated
				servers_mutex.lock();
				for(auto& server : servers){
					ClientContext context;
					PersonReply reply;
					server.second->follow(&context, *request, &reply);
				}
				servers_mutex.unlock();
			}
		}
		return Status::OK;
	}

	Status unfollow(ServerContext*, const PersonRequest* request, PersonReply* reply) override {
		string watcher = request->requestuser();
		string watched = request->targetuser();

		// Can't unfollow yourself, someone who doesn't exist,
		// or someone who you aren't following!
		if(watcher == watched || !users.count(watched) || !is_following(watcher, watched)){
			reply->set_status("FAILURE_INVALID_USERNAME");
		}
		else{
			remove_follower(watcher, watched);
			reply->set_status("SUCCESS");
			if(AMS){
				// Forward this command to other servers so they stay updated
				servers_mutex.lock();
				for(auto& server : servers){
					ClientContext context;
					PersonReply reply;
					server.second->unfollow(&context, *request, &reply);
				}
				servers_mutex.unlock();
			}
		}
		return Status::OK;
	}

	Status list(ServerContext*, const ListRequest* request, ListReply* reply) override {
		// Pretty straightforward. Send back a reply containing two lists,
		// a lists of all users and a list of this person's followers
		for(auto user : users){
			reply->add_users(user.first);
		}
		for(string& follower : get_followers(request->username())){
			reply->add_followers(follower);
		}
		reply->set_status("SUCCESS");
		return Status::OK;
	}

	Status timeline(ServerContext*, timeline_stream stream) override {
		// Read a message containing the username for this stream
		TimelineStream t;
		stream->Read(&t);
		string username = t.username();

		// Hold onto this stream pointer so we can get the freshest posts
		users[username] = stream;

		// Give them the last 20 events from their timeline
		for(string p : get_timeline(username)){
			stream->Write(post_from_str(p));
		}

		// Read incoming messages and post them to the user's timeline
		while(stream->Read(&t)){
			post_to_timeline(t);
			if(debug) cout << "Got timeline post from: " << username << '\n';
			if(AMS){
				// Forward this post to other servers so they stay updated
				for(auto& server : servers){
					cout << "forwarding post to " <<
						server.first.first << ":" << server.first.second << "\n";
					servers_mutex.lock();
					ClientContext context;
					shared_ptr<ClientReaderWriter<TimelineStream, TimelineStream>>
						stream2(server.second->timeline(&context));
					stream2->Write(t);
					stream2->WritesDone();
					servers_mutex.unlock();
					cout << "sent post!\n";
				}
			}
		}

		// If we've exited the while and dropped down here, the user disconnected.
		// Update the user stream (timeline will still get updates,
		// but they won't be sent until the user reconnects)
		users[username] = nullptr;
		return Status::OK;
	}
};

// Open the server channel (start listening for incoming commands)
void run_server(const std::string& host, const std::string& port){
	// Launch the GRPC server
	ServerBuilder builder;
	builder.AddListeningPort(host+":"+port, grpc::InsecureServerCredentials());
	SNetworkServiceImpl service;
	builder.RegisterService(&service);
	std::unique_ptr<Server>(builder.BuildAndStart())->Wait();
}

int main(int argc, char** argv){
	// Default values in case the user doesn't supply anything
	string host = "localhost", port = "1415";
	string r_host = "localhost", r_port = "1413";
	string m_pid = "";
	char opt;
	if(debug) cout << "Parsing input args\n";
	while((opt = getopt(argc, argv, "h:p:r:t:s:")) != -1) {
		if(opt == 'h') host = optarg;
		else if(opt == 'p') port = optarg;
		else if(opt == 'r') r_host = optarg;
		else if(opt == 't') r_port = optarg;
		else if(opt == 's') m_pid = optarg;
		else cerr << "Invalid Command Line Argument\n";
	}

	// Mark this server as a slave server (it won't handle commands)
	if(!m_pid.empty()) run_slave(stoi(m_pid), r_host, r_port);

	// Load the user database (persistent file storage)
	if(debug) cout << "Loading user database\n";
	load_users();

	// Link this server up to the router before proceeding
	connect_to_router(r_host, r_port, host, port);

	// Launch the GRPC server
	if(debug) cout << "Opening server\n";
	run_server(host, port);
}
