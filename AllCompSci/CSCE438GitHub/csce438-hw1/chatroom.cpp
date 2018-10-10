#include "chatroom.hpp"

#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>

using namespace std;

Chatroom::Chatroom(string name, int port, int mastersocket, bool alive):
	_name(name),
	_port(port),
	_master_socket(mastersocket),
	_alive(alive)
{

}

void Chatroom::add_member(int fd)
{
	_members.push_back(fd);
}

void Chatroom::delete_member(int fd)
{
	for(int i = 0; i < _members.size(); i++){
		if(_members[i] == fd){
			_members.erase(_members.begin() + i);
		}
	}
}

void Chatroom::send_message_to_members(std::string message)
{
	for (int i = 0; i < _members.size(); i++)
	{
		// Send message to slave socket.
		write(_members[i], message.c_str(), message.size()+1);
	}
}

void Chatroom::change_alive()
{
	_alive = false;
}