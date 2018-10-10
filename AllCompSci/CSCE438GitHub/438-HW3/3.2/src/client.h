#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <grpc++/grpc++.h>

#define MAX_DATA 256

enum IStatus{
	SUCCESS,
	FAILURE_ALREADY_EXISTS,
	FAILURE_NOT_EXISTS,
	FAILURE_INVALID_USERNAME,
	FAILURE_INVALID,
	FAILURE_UNKNOWN
};

/*
 * IReply structure is designed to be used for displaying the
 * result of the command that has been sent to the server.
 * For example, in the "processCommand" function, you should
 * declare a variable of IReply structure and set based on
 * the type of command and the result.
 *
 * - FOLLOW/UNFOLLOW/TIMELINE command:
 * IReply ireply;
 * ireply.grpc_status = return value of a service method
 * ireply.comm_status = one of values in IStatus enum
 *
 * - LIST command:
 * IReply ireply;
 * ireply.grpc_status = return value of a service method
 * ireply.comm_status = one of values in IStatus enum
 * reply.users = list of all users who connected to the server at least onece
 * reply.followers = list of users who following current user;
 *
 * This structure is not for communicating between server and client.
 * You need to design your own rules for the communication.
 */
struct IReply{
	grpc::Status grpc_status;
	enum IStatus comm_status;
	std::vector<std::string> all_users;
	std::vector<std::string> followers;
};

class IClient{
	public:
		void run();
		virtual int connectTo() = 0;
		virtual IReply processCommand(std::string& cmd) = 0;
		virtual void processTimeline() = 0;

	protected:

	private:
		void displayTitle() const;
		std::string getCommand() const;
		void displayCommandReply(const std::string& comm, const IReply& reply) const;
		void toUpperCase(std::string& str) const;
};

void IClient::run(){
	int ret = connectTo();
	if(ret != 0){
		std::cerr << "connection failed: " << ret << std::endl;
		exit(EXIT_FAILURE);
	}

	displayTitle();
	while(true){
		std::string cmd = getCommand();
		if(cmd == "EOF") break;
		IReply reply = processCommand(cmd);
		displayCommandReply(cmd, reply);
		if(reply.grpc_status.ok() && reply.comm_status == SUCCESS && cmd == "TIMELINE") {
			std::cout << "Now you are in the timeline" << std::endl;
			processTimeline();
		}
	}
}

void IClient::displayTitle() const {
	std::cout << "\n========= TINY SNS CLIENT =========\n"
			  << " Command Lists and Format:\n"
			  << " FOLLOW <username>\n"
			  << " UNFOLLOW <username>\n"
			  << " LIST\n"
			  << " TIMELINE\n"
			  << "=====================================\n";
}

std::string IClient::getCommand() const {
	std::string input = "EOF";
	while(true){
		std::cout << "Cmd> ";
		if(std::getline(std::cin, input)); else break;

		std::size_t index = input.find_first_of(" ");
		if(index != std::string::npos){
			std::string cmd = input.substr(0, index);
			toUpperCase(cmd);
			if(input.length() == index+1){
				std::cout << "Invalid Input -- No Arguments Given\n";
				continue;
			}
			std::string argument = input.substr(index+1, (input.length()-index));
			input = cmd + " " + argument;
		}
		else{
			toUpperCase(input);
			if(input != "LIST" && input != "TIMELINE") {
				std::cout << "Invalid Command: " << input << "\n";
				continue;
			}
		}
		break;
	}
	return input;
}

void IClient::displayCommandReply(const std::string& comm, const IReply& reply) const {
	if(reply.grpc_status.ok()){
		switch (reply.comm_status){
			case SUCCESS:
				std::cout << "Command completed successfully\n";
				if(comm == "LIST" && !reply.all_users.empty()) {
					std::cout << "All users: " << reply.all_users[0];
					for(std::size_t i=1; i<reply.all_users.size(); ++i){
						std::cout << ", " << reply.all_users[i];
					}
					std::cout << "\nFollowers: ";
					if(!reply.followers.empty()){
						std::cout << reply.followers[0];
						for(std::size_t i=1; i<reply.followers.size(); ++i){
							std::cout << ", " << reply.followers[i];
						}
					}
					std::cout << '\n';
				}
				break;
			case FAILURE_ALREADY_EXISTS:
				std::cout << "Input username already exists, command failed\n";
				break;
			case FAILURE_NOT_EXISTS:
				std::cout << "Input username does not exists, command failed\n";
				break;
			case FAILURE_INVALID_USERNAME:
				std::cout << "Command failed with invalid username\n";
				break;
			case FAILURE_INVALID:
				std::cout << "Invalid Command: " << comm << "\n";
				break;
			case FAILURE_UNKNOWN:
				std::cout << "Command failed with unknown reason\n";
				break;
			default:
				std::cout << "Invalid status\n";
				break;
		}
	}
	else std::cout << "grpc failed: " << reply.grpc_status.error_message() << std::endl;
}

void IClient::toUpperCase(std::string& str) const {
	std::locale l;
	for(std::string::size_type i=0; str[i]; ++i) str[i] = toupper(str[i], l);
}

std::string getPostMessage(){
	char buf[MAX_DATA];
	buf[0] = '\n';
	while(stdin && buf[0] == '\n') fgets(buf, MAX_DATA, stdin);
	std::string message(buf);
	return message;
}

void displayPostMessage(const std::string& sender, const std::string& message, std::time_t& time){
	std::string t_str(std::ctime(&time));
	t_str[t_str.size()-1] = '\0';
	std::cout << sender << "(" << t_str << ") >> " << message << std::endl;
}
