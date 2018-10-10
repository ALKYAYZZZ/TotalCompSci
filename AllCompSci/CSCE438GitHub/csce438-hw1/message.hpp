/*
* Wrapper for messages sent to server,
* and also some functions for handling a Reply.
*/

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <sstream>

#include "interface.h"
#include "chatroom.hpp"

// The command to send to the server.
enum Command
{
	ERROR,
	CREATE,
	DELETE,
	LIST,
	JOIN,
	// This means the client is in chat mode.
	CHATMODE
};

// This is a message that a client sends to the server.
struct Message
{
	Command command;
	std::string argument;
	int fd;

	Message(Command _command, std::string _argument = "");
	// This is the line sent by the client.
	Message(std::string line);
	Message(std::string line, int _fd);
};

Message::Message(Command _command, std::string _argument):
	command(_command),
	argument(_argument)
{

}

Message::Message(std::string line, int _fd):
	fd(_fd)
{
	std::stringstream ss(line);
	std::string temp;
	getline(ss, temp, ' ');
	if (temp == "LIST")
	{
		command = Command::LIST;
	}
	else if (temp == "JOIN")
	{
		command = Command::JOIN;
		getline(ss, temp, ' ');
		argument = temp;
	}
	else if (temp == "DELETE")
	{
		command = Command::DELETE;
		getline(ss, temp, ' ');
		argument = temp;
	}
	else if (temp == "CREATE")
	{
		command = Command::CREATE;
		getline(ss, temp, ' ');
		argument = temp;
	}
	else
	{
		command = Command::ERROR;
		//throw std::invalid_argument("Could not parse line: " + line);
	}
}

Message::Message(std::string line):
	Message(line, 0)
{

}

// Convert a Reply to an easily parsable string.
std::string reply_to_string(Reply r, Command c, const Chatroom* chatroom)
{
	std::stringstream ss;
	switch (r.status)
	{
		case Status::SUCCESS:
			ss << "SUCCESS";
			break;
		case Status::FAILURE_ALREADY_EXISTS:
			ss << "FAILURE_ALREADY_EXISTS";
			break;
		case Status::FAILURE_NOT_EXISTS:
			ss << "FAILURE_NOT_EXISTS";
			break;
		case Status::FAILURE_INVALID:
			ss << "FAILURE_INVALID";
			break;
		case Status::FAILURE_UNKNOWN:
			ss << "FAILURE UNKNOWN";
			break;
		default:
			throw std::invalid_argument("Bad reply");
	}
	if(r.status == Status::SUCCESS){
		if (c == Command::JOIN)
		{
			ss << std::endl;
			ss << chatroom->num_members() << " " << chatroom->get_port();
		}
		else if (c == Command::LIST)
		{
			ss << std::endl;
			ss << r.list_room;
		}
		else if (c == Command::CREATE)
		{
			ss << std::endl;
			ss << chatroom->num_members() << " " << chatroom->get_port();
			
		}
		else if (c == Command::DELETE)
		{
			ss << std::endl;
		}
	}
	return ss.str();
}

/*
* Given a string of the format outputted in `reply_to_string` and the command
* issued, construct a Reply class.
*/
Reply string_to_reply(std::string s, Command c)
{
	Reply r;
	std::stringstream ss(s);
	std::string status;
	// Get the status message.
	ss >> status;
	if (status == "SUCCESS")
	{
		r.status = Status::SUCCESS;
		if (c == Command::JOIN)
		{
			int n;
			int p;
			ss >> n >> p;
			r.num_member = n;
			r.port = p;
		}
		else if (c == Command::LIST)
		{
			std::string rooms;
			ss >> rooms;
			strcpy(r.list_room, rooms.c_str());
		}
		else if (c == Command::CREATE){
			int n;
			int p;
			ss >> n >> p;
			r.num_member = n;
			r.port = p;
		}
		else if (c == Command::DELETE){
			int n;
			int p;
			ss >> n >> p;
			r.num_member = n;
			r.port = p;
		}
	}
	else if (status == "FAILURE_ALREADY_EXISTS")
	{
		r.status = Status::FAILURE_ALREADY_EXISTS;
	}
	else if (status == "FAILURE_NOT_EXISTS")
	{
		r.status = Status::FAILURE_NOT_EXISTS;
	}
	else if (status == "FAILURE_INVALID")
	{
		r.status = Status::FAILURE_INVALID;
	}
	else if (status == "FAILURE_UNKNOWN")
	{
		r.status = Status::FAILURE_UNKNOWN;
	}
	else
	{
		throw std::invalid_argument("Failed to parse Reply: " + s);
	}

	return r;
}

#endif