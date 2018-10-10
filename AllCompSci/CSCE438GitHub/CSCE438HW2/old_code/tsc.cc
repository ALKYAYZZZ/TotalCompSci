#include <iostream>
//#include <memory>
//#include <thread>
//#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <grpc++/grpc++.h>
#include "service.pb.h"
#include "service.grpc.pb.h"
#include "client.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using socialnetwork::SocialNetwork;
using socialnetwork::Person;
using socialnetwork::Update;
using socialnetwork::Commands;
using socialnetwork::Commands_Choices;
using socialnetwork::CommandReply;
using socialnetwork::CommandReply_ReturnVal;


class Client : public IClient{
	public:
		Client(const std::string& hname,
				const std::string& uname,
				const std::string& p)
			:hostname(hname), username(uname), port(p) {}
	protected:
		virtual int connectTo();
		virtual IReply processCommand(std::string& input);
		virtual void processTimeline();
	private:
		Commands convert(std::string raw);
		std::string hostname;
		std::string username;
		std::string port;
		std::unique_ptr<SocialNetwork::Stub> stub_;

		// You can have an instance of the client stub
		// as a member variable.
		//std::unique_ptr<NameOfYourStubClass::Stub> stub_;
};

int main(int argc, char** argv) {
	std::string hostname = "localhost";
	std::string username = "default";
	std::string port = "3010";
	int opt = 0;
	while ((opt = getopt(argc, argv, "h:u:p:")) != -1){
		switch(opt) {
			case 'h':
				hostname = optarg;break;
			case 'u':
				username = optarg;break;
			case 'p':
				port = optarg;break;
			default:
				std::cerr << "Invalid Command Line Argument\n";
		}
	}

	Client myc(hostname, username, port);
	// You MUST invoke "run_client" function to start business logic
	myc.run_client();
}

int Client::connectTo(){
	// ------------------------------------------------------------
	// In this function, you are supposed to create a stub so that
	// you call service methods in the processCommand/porcessTimeline
	// functions. That is, the stub should be accessible when you want
	// to call any service methods in those functions.
	// I recommend you to have the stub as
	// a member variable in your own Client class.
	// Please refer to gRpc tutorial how to create a stub.
	// ------------------------------------------------------------
	stub_ = SocialNetwork::NewStub(grpc::CreateChannel(this->hostname + ":" + this->port, grpc::InsecureChannelCredentials()));
	Person person;
	grpc::ClientContext context;
	CommandReply reply;
	person.set_name(this->username);


	stub_->connectUser(&context, person, &reply);

	return 1; // return 1 if success, otherwise return -1
}

IReply Client::processCommand(std::string& input){
	// ------------------------------------------------------------
	// GUIDE 1:
	// In this function, you are supposed to parse the given input
	// command and create your own message so that you call an
	// appropriate service method. The input command will be one
	// of the followings:
	//
	// FOLLOW <username>
	// UNFOLLOW <username>
	// LIST
	// TIMELINE
	//
	// - JOIN/LEAVE and "<username>" are separated by one space.
	// ------------------------------------------------------------

	// ------------------------------------------------------------
	// GUIDE 2:
	// Then, you should create a variable of IReply structure
	// provided by the client.h and initialize it according to
	// the result. Finally you can finish this function by returning
	// the IReply.
	// ------------------------------------------------------------

	// ------------------------------------------------------------
	// HINT: How to set the IReply?
	// Suppose you have "Join" service method for JOIN command,
	// IReply can be set as follow:
	//
	//     // some codes for creating/initializing parameters for
	//     // service method
	//     IReply ire;
	//     grpc::Status status = stub_->Join(&context, /* some parameters */);
	//     ire.grpc_status = status;
	//     if (status.ok()) {
	//         ire.comm_status = SUCCESS;
	//     } else {
	//         ire.comm_status = FAILURE_NOT_EXISTS;
	//     }
	//
	//      return ire;
	//
	// IMPORTANT:
	// For the command "LIST", you should set both "all_users" and
	// "following_users" member variable of IReply.
	// ------------------------------------------------------------
	grpc::ClientContext context;
	Commands c = convert(input);
	CommandReply reply;

	IReply ire;

	ire.grpc_status = stub_->sendCommand(&context, c, &reply);

	return ire;
}

void Client::processTimeline(){
	// ------------------------------------------------------------
	// In this function, you are supposed to get into timeline mode.
	// You may need to call a service method to communicate with
	// the server. Use getPostMessage/displayPostMessage functions
	// for both getting and displaying messages in timeline mode.
	// You should use them as you did in hw1.
	// ------------------------------------------------------------

	// ------------------------------------------------------------
	// IMPORTANT NOTICE:
	//
	// Once a user enter to timeline mode , there is no way
	// to command mode. You don't have to worry about this situation,
	// and you can terminate the client program by pressing
	// CTRL-C (SIGINT)
	// ------------------------------------------------------------
}

Commands Client::convert(std::string raw){
	Commands c;
	c.set_person(this->username);
	c.set_cmd(raw);

	std::stringstream ss(raw);
	std::string temp;
	getline(ss, temp, ' ');
	if(temp == "FOLLOW"){
		c.set_cmd_choice(socialnetwork::Commands_Choices_FOLLOW);
		getline(ss, temp, ' ');
		c.set_args(temp);
	}
	else if(temp == "UNFOLLOW"){
		c.set_cmd_choice(socialnetwork::Commands_Choices_UNFOLLOW);
		getline(ss, temp, ' ');
		c.set_args(temp);
	}
	else if(temp == "LIST"){
		c.set_cmd_choice(socialnetwork::Commands_Choices_UNFOLLOW);
	}
	else if(temp == "TIMELINE"){
		c.set_cmd_choice(socialnetwork::Commands_Choices_TIMELINE);
	}
	else{
		c.set_cmd_choice(socialnetwork::Commands_Choices_UNKNOWN);
	}
	return c;
}
