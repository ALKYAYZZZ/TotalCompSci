#include <algorithm>
#include <string>
#include <sstream>
#include <memory>
#include <vector>

#include <grpc++/grpc++.h>
#include "tsnsr.grpc.pb.h"

using grpc::InsecureServerCredentials;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;

using tsnsr::snsRoute;
using tsnsr::Message;
using tsnsr::Response;

using std::find;
using std::remove;
using std::string;
using std::unique_ptr;
using std::vector;

string getIP(string peer) {
    std::stringstream peer_string(peer);
    vector<string> tokens;
    string token;
    while(std::getline(peer_string, token, ':'))
        tokens.push_back(token);
    return tokens[1];
}

class routeImpl final : public snsRoute::Service {
    vector<string> masters; // IP-address:port
    string availableIP;

    // let routing server know about new master
    Status AddMaster(ServerContext *context,
                const Message *request, Response *resp) override {
        
        string port = request->text();
        string ip = getIP(context->peer());
        resp->set_result(Response::SUCCESS);
        string master = ip + ":" + port;
        if (find(masters.begin(), masters.end(), master) == masters.end())
            masters.push_back(master);
        resp->set_msg(master);
        if (availableIP == "")
            availableIP = master;
        return Status::OK;
    }
    
    // return the server which is being used for passing messages between clients
    Status GetAvailable(ServerContext *context,
                const Message *request, Response *resp) override {
        resp->set_msg(availableIP);
        if (availableIP != "")
            resp->set_result(Response::SUCCESS);
        else resp->set_result(Response::FAILURE);
        return Status::OK;
    }
    
    Status DualPing(ServerContext* context,
		ServerReaderWriter<Message, Message>* stream) override {
        Message m;
        string port = "";
        while (stream->Read(&m)) {
            if (port == "")
                port = m.text();
            stream->Write(m); // just send it back
        }
        // when the client dies remove it from masters and clear availableIP
        string master = getIP(context->peer()) + ":" + port;
        masters.erase(remove(masters.begin(), masters.end(), master), masters.end());
        if (master == availableIP) {
            availableIP = "";
            if (masters.size() > 0)
                availableIP = masters[0]; // routing server decides the next available server
        }
        return Status::OK;
    }
};

void runServer(string port) {
    ServerBuilder builder;
    string host = string("0.0.0.0:") + port;
    builder.AddListeningPort(host, InsecureServerCredentials());

    routeImpl service;
    builder.RegisterService(&service);

    unique_ptr<Server> server(builder.BuildAndStart());
    server->Wait();
}

int main(int argc, char **argv) {
    string port;
    if (argc == 2)
        port = argv[1];
    else port = "4000";
    runServer(port);
}
