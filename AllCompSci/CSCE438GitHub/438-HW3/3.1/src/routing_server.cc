#include <iostream>
#include <set>
#include "tsd_utils.h"

using namespace std;

set<pair<string, string> > servers;
string avail_host, avail_port;

// Timeout in milliseconds before we pick a new Available Master Server (AMS)
unsigned long long TIME_OUT = 1000;
unsigned long long last_ping = 0;// Last time we got a ping from the AMS

// Whether to print debug code
bool debug = true;

class SNetworkServiceImpl final : public SNetwork::Service {
	Status connect(ServerContext*, const ConnectRequest* req, ConnectReply* reply) override {
		string addr = req->my_addr(), port = req->my_port();
		//if(debug) cout << "Got a ping from:" << addr << ":" << port << "!\n";

		// If addr and port are not empty, then this is a server
		if(!addr.empty() && !port.empty()){
			// If this is a new server, link it up
			if(!servers.count(make_pair(addr, port))){
				if(debug) cout << "New server connected!\n";
				servers.insert(make_pair(addr, port));
			}
			//cout << "time since last ping: " << (get_current_time() - last_ping) << endl;
			// The AMS hasn't been responding! Elect a new AMS
			//if(debug) cout << "Time since ping: " << (get_current_time() - last_ping) << endl;
			if(get_current_time() - last_ping > TIME_OUT){
				if(debug) cout << "Elected a new Available Master Server!\n";
				avail_host = addr;
				avail_port = port;
			}
			if(addr == avail_host && port == avail_port){
				// This is the Available Master Server (GOD), send it extra data
				// about other servers on the network
				for(pair<string, string> server : servers){
					reply->add_other_hosts(server.first);
					reply->add_other_ports(server.second);
				}
				last_ping = get_current_time();
				//cout << "Last ping: " << last_ping << endl;
			}
		}
		reply->set_host(avail_host);
		reply->set_port(avail_port);
		reply->set_status("SUCCESS");
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
	string host = "localhost", port = "1413";
	char opt;
	while((opt = getopt(argc, argv, "h:p:")) != -1) {
		if(opt == 'h') host = optarg;
		else if(opt == 'p') port = optarg;
		else cerr << "Invalid Command Line Argument\n";
	}
	if(debug) cout << "Done parsing input args, starting router\n";

	// Start the GRPC server
	run_server(host, port);
}
