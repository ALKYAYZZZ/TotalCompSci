#ifndef CHATROOM_HPP
#define CHATROOM_HPP

#include <string>
#include <vector>

// Represents a chat room.
class Chatroom
{
	std::string _name;
	
	int _port;

	// Sockets of members.
	std::vector<int> _members;

	int _master_socket;

	bool _alive;
	

public:
	Chatroom(std::string name, int port, int mastersocket, bool alive);

	//Chatroom alive or dead
	

	// Add a member to the chatroom.
	void add_member(int fd);

	// Remove a member
	void delete_member(int fd);

	// Get the number of members.
	int num_members() const
	{
		return _members.size();
	}

	// Get the port.
	int get_port() const
	{
		return _port;
	}

	//Get chatroom name
	std::string get_name() const
	{
		return _name;
	}

	//Get chatroom master socket
	int get_master_socket() const
	{
		return _master_socket;
	}

	//Get if chatroom is dead or not
	bool get_alive() const
	{
		return _alive;
	}

	//Change chatroom boolean
	void change_alive();

	// Send message to all the members.
	void send_message_to_members(std::string message);
};

#endif