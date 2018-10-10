#include <string>
#include <sstream>
#include <iomanip>

#include <sys/time.h>
#include <cassert>
#include <assert.h>

#include <cmath>
#include <numeric>
#include <algorithm>

#include <list>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "NetworkRequestServer.h"

using namespace std;

NetworkRequestServer::NetworkRequestServer(const string _server_host_name, const unsigned short _port_no){
	struct addrinfo *result;
	struct addrinfo sInput;
	
	memset(&sInput,0,sizeof(sInput));
	sInput.ai_socktype = SOCK_STREAM;
	sInput.ai_family = AF_INET;

	int stat;
	char port[_port_no];
	snprintf(port, sizeof(port), "%ld", _port_no);
	//std::cerr << port;
	//std::cerr << "WEF";
	stat = getaddrinfo(_server_host_name.c_str(),port,&sInput,&result);
	//std::cerr << "dog";
	if(stat != 0){
		cerr << "Error with Status" << std::endl;
	}
	
	filed = socket(result->ai_family,result->ai_socktype,result->ai_protocol);
	//cerr << filed;
	if(filed < 0){
		cerr << "Error making the socket." << std::endl;
	}
	
	if((connect(filed, result->ai_addr, result->ai_addrlen)) < 0){
		cerr << "Error connecting socket." << strerror(errno) << std::endl;
	}
	freeaddrinfo(result);
	
	cout << "Finished connecting to server " << _server_host_name.c_str() << std::endl;
}

NetworkRequestServer::NetworkRequestServer(const unsigned short _port_no, void * (*connection_handler) (void*), int _backlog){
	struct addrinfo *server;
	struct addrinfo sInput;
	struct sockaddr_storage other_address;
	//cerr << "port num: " << _port_no << endl;
	char buffer[1024];
	bool first_time = true;
	memset(&sInput, 0, sizeof(sInput));
	sInput.ai_socktype = SOCK_STREAM;
	sInput.ai_family = AF_UNSPEC;
	sInput.ai_flags = AI_PASSIVE;
	
	
	
	char port[_port_no];
	snprintf(port, sizeof(port), "%ld", _port_no);
	
	int address_value = getaddrinfo(NULL,port, &sInput, &server);
	
	if(address_value != 0){
		cerr << "Error with Status" << std::endl;
	}
	
	filed = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
	
	if(filed < 0){
		cerr << "Error connecting socket." << std::endl;
	}
	
	const int trueFlag = 1;
	/*
	if((setsockopt(filed, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int))) < 0){
		cerr << "Error set socket." << std::endl;
	}
	*/
	if((bind(filed, server->ai_addr, server->ai_addrlen)) < 0){
		close(filed);
		cerr << "Error Binding Server." << strerror(errno) << std::endl;
		exit(1);
	}
	
	freeaddrinfo(server);
	
	if(listen(filed, _backlog) < 0){
		cerr << "Error Listening" << std::endl;
	}
	
	socklen_t server_size;
	
	while(true){
		server_size = sizeof(other_address);
		
		pthread_t thread;
		
		otherfd = accept(filed, (struct sockaddr*)&other_address, &server_size);
		
		int *temp = new int(otherfd);
		
		if(otherfd == -1){
			cerr << "Acceptance Error." << std::endl;
		}
		else if(otherfd != -1 && first_time == true){
			cerr << "Connection Success" << std::endl;
			first_time = false;
		}
		
		pthread_create(&thread, NULL, connection_handler, temp);
	}
}

NetworkRequestServer::~NetworkRequestServer(){
	close(filed);
}

string NetworkRequestServer::send_request(string _request){
	cwrite(_request);
	string temp = cread();
	return temp;
}

int NetworkRequestServer::cwrite(string _msg){
	if(_msg.length() > 255){
		cerr << "Message too long!" << std::endl;
		return -1;
	}
	
	const char * temp = _msg.c_str();
	
	if(send(filed, temp, strlen(temp) + 1, 0) == -1){
		cerr << "Writing Error." << std::endl;
	}
	
	return _msg.length();
}

string NetworkRequestServer::cread(){
	char buffer[1024];
	int receive = recv(filed, buffer, sizeof(buffer), 0);
	if(receive < 0){
		cerr << recv(filed, buffer, sizeof(buffer), 0);
		cerr << "Reading Error" << std::endl;
		return NULL;
	}
	
	string s = buffer;
	return s;
}

int NetworkRequestServer::read_fd(){
	return filed;
}
	