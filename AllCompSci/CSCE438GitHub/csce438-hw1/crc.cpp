/*
* Client
*/

// We need these includes because otherwise interface.h has errors.
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <thread>
#include <array>
#include <iostream>

#include "interface.h"
#include "message.hpp"

using namespace std;

/*
 * TODO: IMPLEMENT BELOW THREE FUNCTIONS
 */
int connect_to(const char *host, const int port);
struct Reply process_command(const int sockfd, char* command);
void process_chatmode(const char* host, const int port);

int main(int argc, char** argv) 
{

	display_title();
	while (true) {
		//Connect to Main Server
		int sockfd = connect_to(CHAT_IP, CHAT_PORT);
    
		char command[MAX_DATA];
		//Get User Command
        get_command(command, MAX_DATA);
		
		//Process Command input
		struct Reply reply = process_command(sockfd, command);
		display_reply(command, reply);
		
		touppercase(command, strlen(command) - 1);
		//If attempting to join, enter chatmode
		if (strncmp(command, "JOIN", 4) == 0 && (reply.status == Status::SUCCESS)) {
			printf("Now you are in the chatmode\n");
			process_chatmode(CHAT_IP, reply.port);
		}
		close(sockfd);
	}
	

    return 0;
}

/*
 * Connect to the server using given host and port information
 *
 * @parameter host    host address given by command line argument
 * @parameter port    port given by command line argument
 * 
 * @return socket fildescriptor
 */
int connect_to(const char *host, const int port)
{
	// ------------------------------------------------------------
	// GUIDE :
	// In this function, you are suppose to connect to the server.
	// After connection is established, you are ready to send or
	// receive the message to/from the server.
	// 
	// Finally, you should return the socket fildescriptor
	// so that other functions such as "process_command" can use it
	// ------------------------------------------------------------

	int sockfd = 0;
	int n = 0;
	// Use this instead of char pointers because they are safer.
	std::array<char, 1024> recv_buff;
	struct sockaddr_in serv_addr;

	memset(recv_buff.data(), '\0', sizeof(recv_buff.data()));
	//Create new main client socket
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Could not create socket???" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	//Set sockaddr_in info to main port and host
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = inet_addr(host);
	//Connect to server
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << "connect failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	return sockfd;
}

/* 
 * Send an input command to the server and return the result
 *
 * @parameter sockfd   socket file descriptor to commnunicate
 *                     with the server
 * @parameter command  command will be sent to the server
 *
 * @return    Reply    
 */
struct Reply process_command(const int sockfd, char* command)
{
	// ------------------------------------------------------------
	// GUIDE 1:
	// In this function, you are supposed to parse a given command
	// and create your own message in order to communicate with
	// the server. Surely, you can use the input command without
	// any changes if your server understand it. The given command
    // will be one of the followings:
	//
	// CREATE <name>
	// DELETE <name>
	// JOIN <name>
    // LIST
	//
	// -  "<name>" is a chatroom name that you want to create, delete,
	// or join.
	// 
	// - CREATE/DELETE/JOIN and "<name>" are separated by one space.
	// ------------------------------------------------------------

	std::string com(command);
	Command c;
	if (com.substr(0,6) == "CREATE")
		c = Command::CREATE;
	else if (com.substr(0,6) == "DELETE")
		c = Command::DELETE;
	else if (com.substr(0, 4) == "LIST")
		c = Command::LIST;
	else if (com.substr(0,4) == "JOIN")
		c = Command::JOIN;
	else
		c = Command::ERROR;

	// ------------------------------------------------------------
	// GUIDE 2:
	// After you create the message, you need to send it to the
	// server and receive a result from the server.
	// ------------------------------------------------------------

	// Send the command to the server.
	write(sockfd, com.c_str(), com.size()+1);

	//Read Response back from server
	const int buffer = 2048;
	array<char, buffer> server_response;
	memset(server_response.data(), '\0', sizeof(server_response));
	read(sockfd, server_response.data(), server_response.size());

	// ------------------------------------------------------------
	// GUIDE 3:
	// Then, you should create a variable of Reply structure
	// provided by the interface and initialize it according to
	// the result.
	//
	// For example, if a given command is "JOIN room1"
	// and the server successfully created the chatroom,
	// the server will reply a message including information about
	// success/failure, the number of members and port number.
	// By using this information, you should set the Reply variable.
	// the variable will be set as following:
	//
	// Reply reply;
	// reply.status = SUCCESS;
	// reply.num_member = number;
	// reply.port = port;
	// 
	// "number" and "port" variables are just an integer variable
	// and can be initialized using the message from the server.
	//
	// For another example, if a given command is "CREATE room1"
	// and the server failed to create the chatroom because it
	// already exists, the Reply variable will be set as following:
	//
	// Reply reply;
	// reply.status = FAILURE_ALREADY_EXISTS;
    // 
    // For the "LIST" command,
    // You are supposed to copy the list of chatroom to the list_room
    // variable. Each room name should be seperated by comma ','.
    // For example, if given command is "LIST", the Reply variable
    // will be set as following.
    //
    // Reply reply;
    // reply.status = SUCCESS;
    // strcpy(reply.list_room, list);
    // 
    // "list" is a string that contains a list of chat rooms such 
    // as "r1,r2,r3,"
	// ------------------------------------------------------------
	
	//Convert Server Response to Reply Form
	std::string response_s(server_response.data());
	Reply r = string_to_reply(response_s, c);
	return r;
}


//Thread function to constantly read for chat responses
void read_helper(int socketfd, bool &breakloop){
	while(breakloop == false){
		array<char, 2048> reply;
		memset(reply.data(), '\0', sizeof(reply));
		read(socketfd, reply.data(), reply.size() + 1);
		display_message(reply.data());
		std::string response_s(reply.data());
		cerr << endl;
	}
}
//Thread function to constantly wait for user input to send to chat server
void write_helper(int socketfd, bool &breakloop){
	while(breakloop == false){
		array<char, 2048> message;
		memset(message.data(), '\0', sizeof(message));
		get_message(message.data(),message.size()+1);
		std::string response_s(message.data());
		//If User wants to exit chatmode, exit thread
		if(response_s.compare("Q") == 0){
			breakloop = true;
		}
		else{
			write(socketfd, response_s.c_str(),response_s.size() + 1);
		}
	}
}




/* 
 * Get into the chat mode
 * 
 * @parameter host     host address
 * @parameter port     port
 */
void process_chatmode(const char* host, const int port)
{
	// ------------------------------------------------------------
	// GUIDE 1:
	// In order to join the chatroom, you are supposed to connect
	// to the server using host and port.
	// You may re-use the function "connect_to".
	// ------------------------------------------------------------

	int room = connect_to(host, port);

	// ------------------------------------------------------------
	// GUIDE 2:
	// Once the client have been connected to the server, we need
	// to get a message from the user and send it to server.
	// At the same time, the client should wait for a message from
	// the server.
	// ------------------------------------------------------------
	
    // ------------------------------------------------------------
    // IMPORTANT NOTICE:
    // 1. To get a message from a user, you should use a function
    // "void get_message(char*, int);" in the interface.h file
    // 
    // 2. To print the messages from other members, you should use
    // the function "void display_message(char*)" in the interface.h
    //
    // 3. Once a user entered to one of chatrooms, there is no way
    //    to command mode where the user  enter other commands
    //    such as CREATE,DELETE,LIST.
    //    Don't have to worry about this situation, and you can 
    //    terminate the client program by pressing CTRL-C (SIGINT)
	// ------------------------------------------------------------
	array<char, 2048> message;
		bool breakloop = false;
		//Create two threads for reading and writing to chatroom
		std::thread t1(write_helper, room, std::ref(breakloop));
		t1.detach();
		if(breakloop == false){
			std::thread t2(read_helper, room, std::ref(breakloop));
			t2.detach();
		}
		//Run until user wants to quit
		while(breakloop == false);
}