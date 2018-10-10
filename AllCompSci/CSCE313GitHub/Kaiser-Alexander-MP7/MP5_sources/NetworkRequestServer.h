#ifndef _NETWORKREQUESTCHANNEL_H_
#define _NETWORKREQUESTCHANNEL_H_

#include <iostream>
#include <fstream>
#include <cstring>
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

#include "semaphore.h"

using namespace std;

class NetworkRequestServer{
	private:
		int filed;
		int otherfd;
	public:
		NetworkRequestServer(const string _server_host_name, const unsigned short _port_no);
		NetworkRequestServer(const unsigned short _port_no, void * (*connection_handler) (void*), int _backlog);
		~NetworkRequestServer();
		string send_request(string _request);
		string cread();
		int cwrite(string _msg);
		int read_fd();
};

#endif