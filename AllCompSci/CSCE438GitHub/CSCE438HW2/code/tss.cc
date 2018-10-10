#include <iostream>
//#include <memory>
//#include <thread>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <unistd.h>
#include <grpc++/grpc++.h>
#include "service.pb.h"
#include "service.grpc.pb.h"
#include "client.h"


using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using socialnetwork::SocialNetwork;
using socialnetwork::Person;
using socialnetwork::Update;
using socialnetwork::Commands;
using socialnetwork::Commands_Choices;
using socialnetwork::CommandReply;
using socialnetwork::CommandReply_ReturnVal;



using namespace std;

class SocialNetworkServer final : public SocialNetwork::Service{
    private:
    std::unordered_map<std::string,Person> _people;
    std::unordered_map<std::string,std::queue<Update>> _people_queues;

    
    std::string path = "./log/";
    bool existing_file(string& name){
        std::ifstream file(name.c_str());
        return file.good();
    }
    std::vector<string> return_followers(Person &person){
        std::vector<string> follows;

        for(int i = 0; i < person.followers_size(); i++){
            follows.push_back(person.followers(i));
        }
        return follows;
    }
    std::vector<string> return_following(Person &person){
        vector<string> following;

        for(int i = 0; i < person.following_size(); i++){
            following.push_back(person.following(i));
        }
        return following;
    }
    void update_file(string name){
        auto person_iterator = _people.find(name);
        if(person_iterator == _people.end()){
            return;
        }
        else{
            string person_file = path + person_iterator->first + "-info";
            std::fstream file(person_file.c_str(), std::fstream::in | std::fstream::out);
            person_iterator->second.SerializeToOstream(&file);
            file.close();
        }
    }
    int update(string person, Update &update){
        auto person_iterator = _people.find(person);
        if(person_iterator == _people.end()){
            return -1;
        }
        *(person_iterator->second.add_timeline()) = update;
        std::vector<string> followers = return_followers(person_iterator->second);
    }

    CommandReply_ReturnVal follow(string follower, string following){
        return socialnetwork::CommandReply_ReturnVal_SUCCESS;
    }
    CommandReply_ReturnVal unfollow(string follower, string following){
        return socialnetwork::CommandReply_ReturnVal_SUCCESS;
    }
    CommandReply_ReturnVal list(string person){
        return socialnetwork::CommandReply_ReturnVal_SUCCESS;
    }
    CommandReply_ReturnVal timeline(string person){
        return socialnetwork::CommandReply_ReturnVal_SUCCESS;
    }


public:
    Status sendCommand(ServerContext* context, const Commands* command, CommandReply* reply){
        string _person = command->person();
        string _cmd = command->cmd();
        string _args = command->args();
        Commands_Choices type = command->cmd_choice();
        if(type == socialnetwork::Commands_Choices_UNKNOWN){
            reply->set_return_val(socialnetwork::CommandReply_ReturnVal_FAILURE_INVALID);
        }
        else if(type == socialnetwork::Commands_Choices_FOLLOW){
            reply->set_return_val(follow(_person, _args));
        }
        else if(type == socialnetwork::Commands_Choices_UNFOLLOW){
            reply->set_return_val(unfollow(_person,_args));
        }
        else if(type == socialnetwork::Commands_Choices_LIST){
            reply->set_return_val(list(_person));
        }
        else if(type == socialnetwork::Commands_Choices_TIMELINE){
            reply->set_return_val(timeline(_person));
        }
        return Status::OK;
    }
    Status connectUser(ServerContext* context, const Person* person, CommandReply* reply){
        string _person = person->name();
        if (_people.find(_person) == _people.end()) {
            string person_file = path + _person + "-info";
            if(existing_file(person_file)){
                
            }
            else{
                Person new_person;
                new_person.set_name(_person);

                _people.emplace(_person, new_person);
            }
        reply->set_return_val(socialnetwork::CommandReply_ReturnVal_SUCCESS);
        return Status::OK;
        }
    }
    Status timelineMode(ServerContext* context, ServerReaderWriter<Update,Update>* stream){
        return Status::OK;
    }






};





void RunServer(std::string port) {
    std::string server_address("0.0.0.0:" + port);
    SocialNetworkServer service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char** argv){
    string port = "1024";

    

    if(argc == 2){
        port = argv[1];
    }
    else if (argc < 2){
        cerr << "Port number was never specified, please try again." << endl;
        return 1;
    }
    else{
        cerr << "Too many arguments";
        return 1;
    }


    RunServer(port);

    return 0;
}

