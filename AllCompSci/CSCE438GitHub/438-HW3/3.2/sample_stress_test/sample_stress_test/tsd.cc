/*
 *
 * Copyright 2015, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <ctime>

#include <google/protobuf/timestamp.pb.h>
#include <google/protobuf/duration.pb.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <thread>
#include <unistd.h>
#include <google/protobuf/util/time_util.h>
#include <grpc++/grpc++.h>
#include <bits/stdc++.h>
#include <map>
#include <algorithm>

#include "sns.grpc.pb.h"
#include "tsnsr.grpc.pb.h"

using google::protobuf::Timestamp;
using google::protobuf::Duration;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using csce438::MapReply;
using csce438::Message;
using csce438::ListReply;
using csce438::Request;
using csce438::Reply;
using csce438::SNSService;
using tsnsr::snsRoute;

struct Client {
  std::string username;
  bool connected = true;
  int following_file_size = 0;
  std::vector<Client*> client_followers;
  std::vector<Client*> client_following;
  ServerReaderWriter<Message, Message>* stream = 0;
  bool operator==(const Client& c1) const{
    return (username == c1.username);
  }
};

//Vector that stores every client that has been created
std::vector<Client*> client_db;
bool outside_update = false;

//Helper function used to find a Client object given its username
int find_user(std::string username){
  int index = 0;
  for(Client* c : client_db){
    if(c->username == username)
      return index;
    index++;
  }
  return -1;
}

void write_userlist() {
    std::string filename = "userlist.txt";
    std::ofstream user_file(filename,std::ios::trunc|std::ios::out|std::ios::in);
    for (Client* c : client_db) {
        user_file << c->username << " " << c->following_file_size << std::endl;
        std::vector<Client*>::const_iterator it;
        for(it = c->client_followers.begin(); it != c->client_followers.end(); it++) {
            user_file << (*it)->username << " ";
        }
        user_file << std::endl;
        for(it = c->client_following.begin(); it != c->client_following.end(); it++) {
            user_file << (*it)->username << " ";
        }
        user_file << std::endl;
    }
}

void read_userlist() {
    std::ifstream in("userlist.txt");
    std::map<std::string, std::vector<std::string>> followers;
    std::map<std::string, std::vector<std::string>> followings;

    std::string line1;
    while(getline(in, line1)) {
        Client* c = new Client();
        std::stringstream s_line1(line1);
        std::string username;
        std::string file_size;

        getline(s_line1, username, ' ');
        getline(s_line1, file_size, ' ');
        c->username = username;
        c->following_file_size = std::atoi(file_size.c_str());
        client_db.push_back(c);

        std::string line2, line3, item;
        std::vector<std::string> v_followers,  v_followings;
        getline(in, line2);
        std::stringstream s_line2(line2);
        while (getline(s_line2, item, ' ')) {
            v_followers.push_back(item);
        }
        followers[username] = v_followers;

        getline(in, line3);
        std::stringstream s_line3(line3);
        while (getline(s_line3, item, ' ')) {
            v_followings.push_back(item);
        }
        followings[username] = v_followings;
    }

    std::map<std::string, std::vector<std::string>>::iterator it;
    for (it = followers.begin(); it != followers.end(); it++) {
        std::string username = it->first;
        std::vector<std::string> v_followers = it->second;
        Client* user = client_db[find_user(username)];

        std::vector<std::string>::const_iterator itt;
        for (itt = v_followers.begin(); itt != v_followers.end(); itt++) {
            user->client_followers.push_back(client_db[find_user(*itt)]);
        }
    }
    for (it = followings.begin(); it != followings.end(); it++) {
        std::string username = it->first;
        std::vector<std::string> v_followings = it->second;
        Client* user = client_db[find_user(username)];

        std::vector<std::string>::const_iterator itt;
        for (itt = v_followings.begin(); itt != v_followings.end(); itt++) {
            user->client_following.push_back(client_db[find_user(*itt)]);
        }
    }
}


class SNSServiceImpl final : public SNSService::Service {

  Status List(ServerContext* context, const Request* request, ListReply* list_reply) override {
    Client* user = client_db[find_user(request->username())];
    int index = 0;
    for(Client* c : client_db){
      list_reply->add_all_users(c->username);
    }
    std::vector<Client*>::const_iterator it;
    for(it = user->client_followers.begin(); it!=user->client_followers.end(); it++){
      list_reply->add_followers((*it)->username);
    }
    return Status::OK;
  }

  Status Follow(ServerContext* context, const Request* request, Reply* reply) override {
    std::string username1 = request->username();
    std::string username2 = request->arguments(0);
    int join_index = find_user(username2);
    if(join_index < 0 || username1 == username2)
      reply->set_msg("Follow Failed -- Invalid Username");
    else{
	    Client *user1 = client_db[find_user(username1)];
	    Client *user2 = client_db[join_index];
	    if(std::find(user1->client_following.begin(), user1->client_following.end(), user2) != user1->client_following.end()){
		    reply->set_msg("Follow Failed -- Already Following User");
		    return Status::OK;
	    }
	    user1->client_following.push_back(user2);
	    user2->client_followers.push_back(user1);
	    reply->set_msg("Follow Successful");

	    for (Client* c : client_db) {
		    std::string filename = c->username + "followers.txt";
		    std::ofstream user_file(filename,std::ios::trunc|std::ios::out|std::ios::in);
		    std::vector<Client*>::const_iterator it;
		    for(it = c->client_followers.begin(); it!=c->client_followers.end(); it++) {
			    user_file << (*it)->username << std::endl;
		    }
	    }
	    write_userlist();
    }
    return Status::OK;
  }

  Status UnFollow(ServerContext* context, const Request* request, Reply* reply) override {
    std::string username1 = request->username();
    std::string username2 = request->arguments(0);
    int leave_index = find_user(username2);
    if(leave_index < 0 || username1 == username2)
      reply->set_msg("UnFollow Failed -- Invalid Username");
    else{
      Client *user1 = client_db[find_user(username1)];
      Client *user2 = client_db[leave_index];
      if(std::find(user1->client_following.begin(), user1->client_following.end(), user2) == user1->client_following.end()){
	reply->set_msg("UnFollow Failed -- Not Following User");
        return Status::OK;
      }
      user1->client_following.erase(find(user1->client_following.begin(), user1->client_following.end(), user2));
      user2->client_followers.erase(find(user2->client_followers.begin(), user2->client_followers.end(), user1));
      reply->set_msg("UnFollow Successful");
      for (Client* c : client_db) {
	      std::string filename = c->username + "followers.txt";
	      std::ofstream user_file(filename,std::ios::trunc|std::ios::out|std::ios::in);
	      std::vector<Client*>::const_iterator it;
	      for(it = c->client_followers.begin(); it!=c->client_followers.end(); it++) {
		      user_file << (*it)->username << std::endl;
	      }
      }
      write_userlist();
    }
    return Status::OK;
  }

  Status Login(ServerContext* context, const Request* request, Reply* reply) override {
    Client* c = new Client();
    std::string username = request->username();
    int user_index = find_user(username);
    if(user_index < 0){
      c->username = username;
      c->client_followers.push_back(c);
      c->client_following.push_back(c);
      client_db.push_back(c);
      reply->set_msg("Login Successful!");
    }
    else{
      Client *user = client_db[user_index];
      if(user->connected) {
        reply->set_msg("Invalid Username");
	return Status::OK;
      }else{
        std::string msg = "Welcome Back " + user->username;
	reply->set_msg(msg);
        user->connected = true;
      }
    }
    write_userlist();
    return Status::OK;
  }

  Status Ping(ServerContext* context, const Request* request, Reply* reply) override {
    return Status::OK;
  }

  Status Timeline(ServerContext* context,
		ServerReaderWriter<Message, Message>* stream) override {
        Message message;
        Client *c;
        while(stream->Read(&message)) {
            std::string username = message.username();
            int user_index = find_user(username);
            c = client_db[user_index];

            //Write the current message to "username.txt"
            std::string filename = username+".txt";
            std::ofstream user_file(filename,std::ios::app|std::ios::out|std::ios::in);
            google::protobuf::Timestamp temptime = message.timestamp();
            std::string time = google::protobuf::util::TimeUtil::ToString(temptime);
            std::string fileinput = time+" :: "+message.username()+":"+message.msg()+"\n";
            //"Set Stream" is the default message from the client to initialize the stream
            if(message.msg() != "Set Stream")
                user_file << fileinput;
            //If message = "Set Stream", print the first 20 chats from the people you follow
            else{
                if(c->stream==0)
                    c->stream = stream;

                std::string line;
                std::vector<std::string> newest_twenty;
                std::ifstream in(username+"following.txt");
                int count = 0;
                //Read the last up-to-20 lines (newest 20 messages) from userfollowing.txt
                while(getline(in, line)){
                    if(c->following_file_size > 20){
                        if(count < c->following_file_size-20){
                            count++;
                            continue;
                        }
                    }
                    newest_twenty.push_back(line);
                }
                Message new_msg;
                //Send the newest messages to the client to be displayed
                for(int i = 0; i<newest_twenty.size(); i++){
                    new_msg.set_msg(newest_twenty[i]);
                    stream->Write(new_msg);
                }
                continue;
            }
            //Send the message to each follower's stream
            std::vector<Client*>::const_iterator it;
            for(it = c->client_followers.begin(); it!=c->client_followers.end(); it++){
                Client *temp_client = *it;
                if(temp_client->stream!=0 && temp_client->connected && temp_client->username != c->username) {
                    message.set_msg(fileinput);
                    temp_client->stream->Write(message);
                }
                //For each of the current user's followers, put the message in their following.txt file
                std::string temp_username = temp_client->username;
                std::string temp_file = temp_username + "following.txt";
                std::ofstream following_file(temp_file,std::ios::app|std::ios::out|std::ios::in);
                following_file << fileinput;
                temp_client->following_file_size++;
                std::ofstream user_file(temp_username + ".txt",std::ios::app|std::ios::out|std::ios::in);
                user_file << fileinput;
                write_userlist();
            }
        }
        //If the client disconnected from Chat Mode, set connected to false
        c->connected = false;
        return Status::OK;
    }

  Status GetFollowerInfo(ServerContext *context,
                        const Request *request, MapReply *mapReply) {
      
      auto getNames = [](std::vector<Client*> clients) {
          std::vector<std::string> names;
          for (auto client : clients)
              names.push_back(client->username);
          return names;
      };

      auto join = [](std::vector<std::string> tokens, char sep) {
          std::string res = "";
          for (auto it = tokens.begin(); it != tokens.end(); it++) {
              res += *it;
              if (it != tokens.end() - 1)
                  res += sep;
          }
          return res;
      };
      
      for (auto client : client_db) {
          // convert followers+following to compact string and insert key-value pair into mapReply
          std::string followersString = join(getNames(client->client_followers), '\t');
          std::string followingString = join(getNames(client->client_following), '\t');
          std::vector<std::string> parts = {followersString, followingString};
          (*mapReply->mutable_user_info())[client->username] = join(parts, '\n');
      }
      return Status::OK;
  } 
};

class RouteClient {
    std::unique_ptr<snsRoute::Stub> stub;
public:
    RouteClient(std::shared_ptr<grpc::Channel> channel) : stub(snsRoute::NewStub(channel)) {}
    std::string GetAvailable() {
        grpc::ClientContext context;
        tsnsr::Message msg;
        tsnsr::Response resp;
        Status status = stub->GetAvailable(&context, msg, &resp);
        if (!status.ok() || resp.result() == tsnsr::Response::FAILURE)
            return "Not a host";
        return resp.msg();
    }

    std::string AddMaster(std::string port) {
        grpc::ClientContext context;
        tsnsr::Message msg;
        tsnsr::Response resp;
        msg.set_text(port);
        Status status = stub->AddMaster(&context, msg, &resp);
        if (status.ok() && resp.result() == tsnsr::Response::SUCCESS)
            return resp.msg();
        else return "";
    }

    void Ping(std::string port) {
        grpc::ClientContext ctxt;
        std::shared_ptr<grpc::ClientReaderWriter<tsnsr::Message, tsnsr::Message>> stream(stub->DualPing(&ctxt));
        tsnsr::Message msg;
        msg.set_text(port);
        stream->Write(msg);
        while (stream->Read(&msg))
            stream->Write(msg);
        stream->WritesDone();
    }
};

void GetFollowerInformation(std::string available_host) {
    // establish stub to available host
    std::unique_ptr<SNSService::Stub> stub = 
            SNSService::NewStub(grpc::CreateChannel(
                          available_host, grpc::InsecureChannelCredentials()));
    // call stub function to get follower data
    grpc::ClientContext context;
    Request req;
    MapReply rep;
    Status status = stub->GetFollowerInfo(&context, req, &rep);
    // for each user, recreate follower files
    std::map<std::string, std::string> users(rep.user_info().begin(), rep.user_info().end());
    for (auto it = users.begin(); it != users.end(); it++) {
        std::string client = it->first;
        int client_index;
        if ((client_index = find_user(client)) == -1) {
            Client *c = new Client();
            c->username = client;
            client_db.push_back(c);
            client_index = client_db.size() - 1;
        }
        std::istringstream info(it->second);
        std::string followers, following, name;
        std::getline(info, followers);
        std::getline(info, following);
        std::istringstream flwr(followers), flwng(following);
        while (std::getline(flwr, name, '\t')) {
            int f_index;
            if ((f_index = find_user(name)) == -1) {
                Client *c = new Client();
                c->username = name;
                client_db[client_index]->client_followers.push_back(c);
                client_db.push_back(c);
            } else if (std::find(client_db[client_index]->client_followers.begin(), 
                                client_db[client_index]->client_followers.end(), client_db[f_index]) == 
                                client_db[client_index]->client_followers.end())
                client_db[client_index]->client_followers.push_back(client_db[f_index]);
        }
        while (std::getline(flwng, name, '\t')) {
            int f_index;
            if ((f_index = find_user(name)) == -1) {
                Client *c = new Client();
                c->username = name;
                client_db[client_index]->client_following.push_back(c);
                client_db.push_back(c);
            } else if (std::find(client_db[client_index]->client_following.begin(), 
                                client_db[client_index]->client_following.end(), client_db[f_index]) == 
                                client_db[client_index]->client_following.end())
                client_db[client_index]->client_following.push_back(client_db[f_index]);
        }
    }
    outside_update = true;
}

void standBy(std::string route_host, std::string port) {
    std::string available_host, master;
    RouteClient rc(grpc::CreateChannel(route_host, grpc::InsecureChannelCredentials()));

    while ((master = rc.AddMaster(port)) == "");
    while ((available_host = rc.GetAvailable()) != master)
        GetFollowerInformation(available_host);
}

void RunServer(std::string port_no, std::string route_host) {
  std::string server_address = "0.0.0.0:"+port_no;
  SNSServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  
  std::thread ping([port_no, route_host](){
      RouteClient rc(grpc::CreateChannel(route_host, grpc::InsecureChannelCredentials()));
      rc.Ping(port_no);
  });
  ping.join();
  server->Wait();
}

int main(int argc, char** argv) {

  // command line arguments initialized by bash script
  std::string route_host = argv[0];
  std::string port = argv[1];

  standBy(route_host, port);
  // only read userlist if client_db has not been written to while on standby
  if (!outside_update)
    read_userlist();
  RunServer(port, route_host);

  return 0;
}
