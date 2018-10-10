/*
* Server.
*/

#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <thread>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <mutex>

#include "chatroom.hpp"
#include "interface.h"
#include "message.hpp"

using namespace std;

// Map the name of a room to the room.
unordered_map<string, Chatroom*> rooms;
// All the ports (to make sure we don't double up).
unordered_set<int> ports;
unordered_map<string, unordered_set<int>> sockets;
// Given a port, we want to be able to find its chat room.
unordered_map<int, Chatroom*> socket_to_room;
unordered_map<int, Chatroom*> port_to_room;

//Thread to receive and send all messages from all clients
void send_thread(int s_sock,  Chatroom* croom, bool &breaker){
	array<char, 2048> resp;
  	while (true) {
		memset(resp.data(), '\0', sizeof(resp));
		int read_return = 0;
		read_return = read(s_sock, resp.data(), resp.size());
		std::mutex mtx;
		mtx.lock();
		//If user disconnects from chat server, close the slave socket
		if(read_return <= 0){
			close(s_sock);
			cerr << "Client Disconnected" << endl;
			croom->delete_member(s_sock);
			sockets[croom->get_name()].erase(s_sock);
			breaker = true;
			return;
		}
		//Forward message to all sockets associated with given chat room
		for (auto it = sockets[croom->get_name()].begin(); it != sockets[croom->get_name()].end(); ++it) {
			if (s_sock == *it)
				continue;
			std::string response_s(resp.data());
			cerr << "ALL FD: " << *it << endl;
			write(*it, response_s.c_str(), response_s.size() + 1);
		}
		mtx.unlock();
	}
}

//Thread for constantly accepting new clients to given chat room master socket
void loop_thread(int m_sock, struct sockaddr_in* client_addr, Chatroom* croom){
	int slave = 0;
	bool breaker = false;
	while(breaker == false){
		slave = accept(m_sock,(struct sockaddr*)NULL, NULL);
		if(slave == -1){
			perror("BAD_ACCESS");
		}
		sockets[croom->get_name()].insert(slave);
		//Detach thread for sending and receiving messages from given slave socket
		std::thread(send_thread, slave, std::ref(croom), std::ref(breaker)).detach();
	}
	
}


Reply handle_join(Message m)
{
	Reply r;
	string name = m.argument;
	//Check if room exists
	if (rooms.find(name) == rooms.end())
	{
		cerr << "Room " << name << " does not exist" << endl;
		r.status = Status::FAILURE_NOT_EXISTS;
		return r;
	}
	else
	{
		// Join the room by returning room port number and master socket
		rooms[name]->add_member(m.fd);
		socket_to_room.insert(make_pair(m.fd, rooms[name]));
		r.status = Status::SUCCESS;
		r.port = rooms[m.argument]->get_port();
		r.num_member;
		return r;
	}
	
}

Reply handle_create(string name, int fd)
{
	Reply r;
	if (rooms.find(name) != rooms.end())
	{
		cerr << "Room " << name << " already exists" << endl;
		r.status = Status::FAILURE_ALREADY_EXISTS;
		return r;
	}
	else
	{
	
	// Server listening socket.
	int newroomfd = 0;
	// Socket connection.

	struct sockaddr_in client_addr;

	// Create a socket.
	
	newroomfd = socket(AF_INET,SOCK_STREAM,0);
	if(newroomfd == -1){
		cerr << "Socket not created" << endl;
		return r;
	}

	//Set sockaddr_in info
	r.port = CHAT_PORT;
	memset(&client_addr, '\0', sizeof(client_addr));
	client_addr.sin_family = AF_INET; 
	client_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	
	//Look for open port by attempting to bind to each port individually
	int result;
	do{
		(r.port)++;
		client_addr.sin_port = htons(r.port);
		result = bind(newroomfd, (struct sockaddr*)&client_addr, sizeof(client_addr) + 1);
	}
	while(result == -1);


	// Mark the socket as "passive"
	int m_sock = listen(newroomfd, 10);
	if (m_sock == -1)
	{
		cerr << "Failed to listen!" << endl;
		return r;
	}
	//Create new chatroom at givent port
	Chatroom * croom = new Chatroom(name,r.port,newroomfd, true);
	//Detach new thread to constantly accept new clients for chat room
	std::thread (loop_thread,newroomfd, &client_addr, croom).detach();
	
	//Set info in reply form and in main chatroom map
	rooms[name] = croom;
	r.status = Status::SUCCESS;
	port_to_room[r.port] = croom;

	return r;
	}
}

Reply handle_delete(Message m)
{
	string name = m.argument;
	Reply r;
	//Check if room exists
	if (rooms.find(name) == rooms.end())
	{
		cerr << "Room " << name  << " does not exist" << endl;
		r.status = Status::FAILURE_NOT_EXISTS;
	}
	else
	{
		// Delete the room, remove it from the dictionary, and message members.
		Chatroom* current = rooms[name];
		string delete_room_msg = "ROOM IS BEING DELETED (PRESS Q TO RETURN TO COMMAND LINE)";
		std::mutex mtx;
		mtx.lock();
		//Send message to all members that room is being closed
		for (auto it = sockets[name].begin(); it != sockets[name].end(); ++it) {
			cerr << "DELETE SIZE: " << sockets[current->get_name()].size() << endl;
			cerr << "ALL FD: " << *it << endl;
			write(*it, delete_room_msg.c_str(), delete_room_msg.size());
		}
		sockets[current->get_name()].clear();
		
		current->change_alive();
		// Delete the rooms.
		rooms.erase(name);
		port_to_room.erase(current->get_master_socket());
		
		//Set Reply Status
		r.status = Status::SUCCESS;
		r.port = current->get_port();
		r.num_member = 0;
		mtx.unlock();
	}

	return r;
}

Reply handle_list()
{
	Reply r;
	stringstream ss;
	//Return all chatrooms in chatrooms vector
	if(rooms.begin() == rooms.end()){
		ss << "Empty";
	}
	else{
		for (auto it = rooms.begin(); it != rooms.end(); it++)
		{
			if((it->first).compare("") != 0)
				ss << it->first << ",";
			}
	}
	// https://stackoverflow.com/a/4118787/5415895
	strcpy(r.list_room, ss.str().c_str());
	r.status = Status::SUCCESS;

	return r;
}

Reply process_message(Message m, int fd)
{
	Reply r;
	switch (m.command)
	{
		case Command::CREATE:
			cout << "Create room " << m.argument << endl;
			r = handle_create(m.argument, fd);
			break;
		case Command::DELETE:
			cout << "Delete room " << m.argument << endl;
			r = handle_delete(m);
			break;
		case Command::JOIN:
			cout << "Join room " << m.argument << endl;
			r = handle_join(m);
			break;
		case Command::LIST:
			cout << "List rooms" << endl;
			r = handle_list();
			break;
		default:
			r.status = FAILURE_INVALID;
			//throw invalid_argument("Thats not even possible");
	}

	return r;
}
//Struct to hold values for pthread
struct arg_struct{
	int orig_fd;
	int new_fd;
};
//Pthread void function to send and receive command functions in reply format
void* handle_request(void* argss){
	//Convert args to usable integers
	struct arg_struct *args = (struct arg_struct *)argss;
	int fd = (args->new_fd);
	int listenfd = (args->orig_fd);

	array<char, 1025> resp;
	memset(resp.data(), '\0', sizeof(resp));
	// Read data from socket.
	read(fd, resp.data(), resp.size());
	string line(resp.data());
	
	Message m(line, fd);
	Reply r = process_message(m, fd);

	// Create and send response as a string
	string response;
	//If command is delete, do not send chatroom
	if(m.command == Command::DELETE){
		response = reply_to_string(r, m.command, NULL);
	}
	else{
		response = reply_to_string(r, m.command, rooms[m.argument]);
	}
	//Write back to client
	write(fd, response.c_str(), response.size()+1);
	close(fd);
}
int main()
{
	
	// Server listening socket.
	int listenfd = 0;
	// Socket connection.
	int connfd = 0;

	struct sockaddr_in serv_addr;



	
	// Create a socket.
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		cerr << "Socket not created" << endl;
	}
	
	memset(&serv_addr, '\0', sizeof(serv_addr));

	//Set sockaddr_in info
	serv_addr.sin_family = AF_INET;    
  	serv_addr.sin_addr.s_addr = inet_addr(CHAT_IP); 
  	serv_addr.sin_port = htons(CHAT_PORT);

	// Bind the socket to the address.
	int bind_return = 0;
	if((bind_return = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr) + 1)) < 0){
		cerr << "Bind Failure" << endl;
	}

	printf("Chat server running at:\n\n\t %s:%d\n\n", CHAT_IP, CHAT_PORT);

	// Mark the socket as "passive"
	int listen_return = 0;
	if ((listen_return = listen(listenfd, 10)) < 0)
	{
		cerr << "Failed to listen!" << endl;
		return EXIT_FAILURE;
	}

	int s_sock;
	int m_sock = listenfd;
	//Create new Concurrent TCP connection for all clients contacting main server
	pthread_t th; pthread_attr_t ta;
  	pthread_attr_init(&ta);
  	pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_DETACHED);
  	for (;;) {
    	s_sock = accept(m_sock,(struct sockaddr*)NULL, NULL);
    	if (s_sock < 0){ 
     		if (errno == EINTR) continue;
			else exit(EXIT_FAILURE);
		}
		struct arg_struct args;
		args.orig_fd = listenfd;
		args.new_fd = s_sock;
    	pthread_create(&th, &ta, handle_request, (void*)&args);

	}
	
	
}