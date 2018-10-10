/*
    File: client.cpp

    Author: J. Higginbotham
    Department of Computer Science
    Texas A&M University
    Date  : 2016/05/21

    Based on original code by: Dr. R. Bettati, PhD
    Department of Computer Science
    Texas A&M University
    Date  : 2013/01/31
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */
    /* -- This might be a good place to put the size of
        of the patient response buffers -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*
    No additional includes are required
    to complete the assignment, but you're welcome to use
    any that you think would help.
*/
/*--------------------------------------------------------------------------*/
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
#include "reqchannel.h"

/*
    This next file will need to be written from scratch, along with
    semaphore.h and (if you choose) their corresponding .cpp files.
 */

#include "bounded_buffer.h"
#include "semaphore.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/
struct helper{
	std::string data;
	int count;
	std::vector<int> john;
	std::vector<int> jane;
	std::vector<int> joe;
	bounded_buffer *help;
	bounded_buffer *ind;
	int determ;
};

struct holper{
	RequestChannel *worker;
	RequestChannel *thread;
	std::vector<int> john;
	std::vector<int> jane;
	std::vector<int> joe;
	bounded_buffer *buff;
	bounded_buffer *johnB;
	bounded_buffer *janeB;
	bounded_buffer *joeB;
	int w;
	int r;
};
semaphore mutex(1);

/*
    All *_params structs are optional,
    but they might help.
 */
 
 std::string make_histogram_table(std::string name1, std::string name2,
        std::string name3, std::vector<int> *data1, std::vector<int> *data2,
        std::vector<int> *data3) {
	std::stringstream tablebuilder;
	tablebuilder << std::setw(25) << std::right << name1;
	tablebuilder << std::setw(15) << std::right << name2;
	tablebuilder << std::setw(15) << std::right << name3 << std::endl;
	for (int i = 0; i < data1->size(); ++i) {
		tablebuilder << std::setw(10) << std::left
		        << std::string(
		                std::to_string(i * 10) + "-"
		                        + std::to_string((i * 10) + 9));
		tablebuilder << std::setw(15) << std::right
		        << std::to_string(data1->at(i));
		tablebuilder << std::setw(15) << std::right
		        << std::to_string(data2->at(i));
		tablebuilder << std::setw(15) << std::right
		        << std::to_string(data3->at(i)) << std::endl;
	}
	tablebuilder << std::setw(10) << std::left << "Total";
	tablebuilder << std::setw(15) << std::right
	        << accumulate(data1->begin(), data1->end(), 0);
	tablebuilder << std::setw(15) << std::right
	        << accumulate(data2->begin(), data2->end(), 0);
	tablebuilder << std::setw(15) << std::right
	        << accumulate(data3->begin(), data3->end(), 0) << std::endl;

	return tablebuilder.str();
}
struct PARAMS_request {

};

struct PARAMS_WORKER {

};

struct PARAMS_STAT {

};

/*
    This class can be used to write to standard output
    in a multithreaded environment. It's primary purpose
    is printing debug messages while multiple threads
    are in execution.
 */
class atomic_standard_output {
    pthread_mutex_t console_lock;
public:
    atomic_standard_output() { pthread_mutex_init(&console_lock, NULL); }
    ~atomic_standard_output() { pthread_mutex_destroy(&console_lock); }
    void print(std::string s){
        pthread_mutex_lock(&console_lock);
        std::cout << s << std::endl;
        pthread_mutex_unlock(&console_lock);
    }
};

atomic_standard_output threadsafe_standard_output;

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* HELPER FUNCTIONS */
/*--------------------------------------------------------------------------*/

std::string make_histogram(std::string name, std::vector<int> *data) {
    std::string results = "Frequency count for " + name + ":\n";
    for(int i = 0; i < data->size(); ++i) {
        results += std::to_string(i * 10) + "-" + std::to_string((i * 10) + 9) + ": " + std::to_string(data->at(i)) + "\n";
    }
    return results;
}

/*
    You'll need to fill these in.
*/
void* request_thread_function(void* arg) {
	struct helper* pass_param = (helper*)arg;
	for(int i = 0; i < pass_param->count; i++) {
		pass_param->help->push_back(pass_param->data);
	}
}

/*
void* worker_thread_function(void* arg) {
	struct holper* pass_param = (holper*)arg;
	 
	 RequestChannel *workerChannel = pass_param->thread;
	 
    while(true) {
			std::string request = pass_param->buff->retrieve_front();
            

            if(request == "data John Smith") {
				std::string response = workerChannel->send_request(request);
                pass_param->johnB->push_back(response);
				
            }
            else if(request == "data Jane Smith") {
				std::string response = workerChannel->send_request(request);
                pass_param->janeB->push_back(response);
            }
            else if(request == "data Joe Smith") {
				std::string response = workerChannel->send_request(request);
                pass_param->joeB->push_back(response);
            }
            else if(request == "quit") {
				workerChannel->send_request("quit");
                delete workerChannel;
                break;
            }
			//pass_param->buff->push_back(request);
    }
}
*/

void* event_thread_function(void* arg){
	
	struct holper* pass_param = (holper*)arg;
	fd_set rfd;
	std::string pid[pass_param->w];
	int file_max = 0;
	int selects;
	int count_r = 0;
	int count_w = 0;
	struct timeval time = {0,10};
	vector<RequestChannel*> chans;
	RequestChannel * con = new RequestChannel("control", RequestChannel::CLIENT_SIDE);
	
	for(int i = 0; i < pass_param->w; i++){
		std::string s = con->send_request("newthread");
		RequestChannel * chan = new RequestChannel(s, RequestChannel::CLIENT_SIDE);
		chans.push_back(chan);
		pid[i] = "no";
	}
	
	for(int i = 0; i < pass_param->w; i++){
		//pid[i] = pass_param->buff->retrieve_front();
		
		pid[i] = pass_param->buff->retrieve_front();
		//std::cerr << pid[i] << std::endl;
		
		chans[i]->cwrite(pid[i]);
		//std::string no = chans[i]->cread();
		//std::cerr << no << std::endl;
		count_w++;
	}
	
	while(true){
		FD_ZERO(&rfd);
		//std::cerr << "SDF";
		for(int i = 0; i < pass_param->w; i++){
			if(chans[i]->read_fd() > file_max){
				file_max = chans[i]->read_fd();
				//std::cerr << file_max;
			}
			
			FD_SET(chans[i]->read_fd(), &rfd);
		}
		selects = select(file_max + 1, &rfd, NULL, NULL, &time);
		//std::cerr << selects;
		if(selects){
			for(int i = 0; i < pass_param->w; i++){
				if(FD_ISSET(chans[i]->read_fd(), &rfd)){
					count_r++;
					std::string request = chans[i]->cread();
					//std::cerr << "Interesting stuff " << request << endl;
					if(request != "unknown request"){

						if(pid[i] == "data John Smith") {
							std::string response = request; //con->send_request(request);
							//std::cerr << response << endl;
							pass_param->johnB->push_back(response);
							
						}
						else if(pid[i] == "data Jane Smith") {
							std::string response = request;//con->send_request(request);
							//std::cerr << response << endl;
							pass_param->janeB->push_back(response);
						}
						else if(pid[i] == "data Joe Smith") {
							std::string response = request;//con->send_request(request);
							//std::cerr << response << endl;
							pass_param->joeB->push_back(response);
						}
						
						else if(pid[i] == "quit") {
							con->send_request("quit");
							delete con;
							break;
						}
					}
					
				if(count_w < pass_param->r * 3){
					count_w++;
					pid[i] = pass_param->buff->retrieve_front();
					chans[i]->cwrite(pid[i]);
				}
				}
			}
		}
		if(count_r == pass_param->r * 3){
		break;
		}
	}
	for(int i = 0; i < pass_param->w; i++){
		chans[i]->cwrite("quit");
		pid[i] = "quit";
	}
}

void* stat_thread_function(void* arg) {
	struct helper* pass_param = (helper*)arg;
	 
    for(int i = 0; i < pass_param->count; i++) {
			std::string request = pass_param->data;
            

            if(pass_param->determ == 0) {
				std::string response = pass_param->ind->retrieve_front();
				//std::cerr << response << endl;
                pass_param->john.at(stoi(response) / 10) += 1;
				
            }
            else if(pass_param->determ == 1) {
				std::string response = pass_param->ind->retrieve_front();
				//std::cerr << response << endl;
                pass_param->jane.at(stoi(response) / 10) += 1;
            }
            else if(pass_param->determ == 2) {
				std::string response = pass_param->ind->retrieve_front();
				//std::cerr << response << endl;
                pass_param->joe.at(stoi(response) / 10) += 1;
            }
    }
	pthread_exit((void*)arg);
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
    int n = 10; //default number of requests per "patient"
    int b = 50; //default size of request_buffer
    int w = 10; //default number of worker threads
    bool USE_ALTERNATE_FILE_OUTPUT = false;
    int opt = 0;
    while ((opt = getopt(argc, argv, "n:b:w:m:h")) != -1) {
        switch (opt) {
            case 'n':
                n = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 'w':
                w = atoi(optarg);
                break;
            case 'm':
                if(atoi(optarg) == 2) USE_ALTERNATE_FILE_OUTPUT = true;
                break;
            case 'h':
            default:
                std::cout << "This program can be invoked with the following flags:" << std::endl;
                std::cout << "-n [int]: number of requests per patient" << std::endl;
                std::cout << "-b [int]: size of request buffer" << std::endl;
                std::cout << "-w [int]: number of worker threads" << std::endl;
                std::cout << "-m 2: use output2.txt instead of output.txt for all file output" << std::endl;
                std::cout << "-h: print this message and quit" << std::endl;
                std::cout << "Example: ./client_solution -n 10000 -b 50 -w 120 -m 2" << std::endl;
                std::cout << "If a given flag is not used, a default value will be given" << std::endl;
                std::cout << "to its corresponding variable. If an illegal option is detected," << std::endl;
                std::cout << "behavior is the same as using the -h flag." << std::endl;
                exit(0);
        }
    }

    int pid = fork();
	if (pid > 0) {
        struct timeval start_time;
        struct timeval finish_time;
        int64_t start_usecs;
        int64_t finish_usecs;
        std::ofstream ofs;
        if(USE_ALTERNATE_FILE_OUTPUT) ofs.open("output2.txt", std::ios::out | std::ios::app);
        else ofs.open("output.txt", std::ios::out | std::ios::app);

        std::cout << "n == " << n << std::endl;
        std::cout << "b == " << b << std::endl;
        std::cout << "w == " << w << std::endl;

        std::cout << "CLIENT STARTED:" << std::endl;
        std::cout << "Establishing control channel... " << std::flush;
        RequestChannel *chan = new RequestChannel("control", RequestChannel::CLIENT_SIDE);
        std::cout << "done." << std::endl;
		
		bounded_buffer *request_buffer = new bounded_buffer(b);
		bounded_buffer *janeBuffer = new bounded_buffer(b);
		bounded_buffer *johnBuffer = new bounded_buffer(b);
		bounded_buffer *joeBuffer = new bounded_buffer(b);
		
		std::vector<int> john_frequency_count(10, 0);
        std::vector<int> jane_frequency_count(10, 0);
        std::vector<int> joe_frequency_count(10, 0);
		
		pthread_t request[3];
		pthread_t event;
		pthread_t stat[3];
		
		helper *janes = new helper;
		janes->data = "data Jane Smith";
		janes->count = n;
		janes->jane = jane_frequency_count;
		janes->joe = joe_frequency_count;
		janes->john = john_frequency_count;
		janes->help = request_buffer;
		janes->ind = janeBuffer;
		janes->determ = 0;
		
		helper *johns = new helper;
		johns->data = "data John Smith";
		johns->count = n;
		johns->jane = jane_frequency_count;
		johns->joe = joe_frequency_count;
		johns->john = john_frequency_count;
		johns->help = request_buffer;
		johns->ind = johnBuffer;
		johns->determ = 1;
		
		helper *joes = new helper;
		joes->data = "data Joe Smith";
		joes->count = n;
		joes->jane = jane_frequency_count;
		joes->joe = joe_frequency_count;
		joes->john = john_frequency_count;
		joes->help = request_buffer;
		joes->ind = joeBuffer;
		joes->determ = 2;
		
		/*
		PARAMS_WORKER work;
		work.janeBuffing = janeBuffer;
		work.joeBuffing = joeBuffer;
		work.johnBuffing = johnBuffer;
		*/
		
		gettimeofday(&start_time,NULL);
		
		pthread_create(&request[0], NULL, request_thread_function, (void*) janes);
		pthread_create(&request[1], NULL, request_thread_function, (void*) joes);
		pthread_create(&request[2], NULL, request_thread_function, (void*) johns);
		
		std::cerr << request_buffer->size();
		holper *fun = new holper;
		fun->jane = jane_frequency_count;
		fun->joe = joe_frequency_count;
		fun->john = john_frequency_count;
		fun->joeB = joeBuffer;
		fun->janeB = janeBuffer;
		fun->johnB = johnBuffer;
		fun->w = w;
		fun->r = n;
		//for(int i = 0; i < w; i++){
			
			
			fun->buff = request_buffer;
			fun->worker = chan;
			//std::string s = chan->send_request("newthread");
			//fun->thread = new RequestChannel(s, RequestChannel::CLIENT_SIDE);
			pthread_create(&event, NULL, event_thread_function, (void*)fun);
		//}
		
		std::cerr << "WOOO";
		pthread_create(&stat[0], NULL, stat_thread_function, (void*) janes);
		pthread_create(&stat[1], NULL, stat_thread_function, (void*) johns);
		pthread_create(&stat[2], NULL, stat_thread_function, (void*) joes);
		
		fflush(NULL);
		for(int i = 0; i < 3; i++){
			pthread_join(request[i], NULL);
		}
		std::cerr << "asdf";
        for(int i = 0; i < w; ++i) {
            request_buffer->push_back("quit");
        }
		std::cerr << "DFS";
		
		//for(int i = 0; i < w; i++){
			pthread_join(event, NULL);
			
//}
		std::cerr << "GREAt";
		for(int i = 0; i < 3; i++){
			pthread_join(stat[i], NULL);
		}
		
		jane_frequency_count = johns->jane;
		joe_frequency_count = joes->joe;
		john_frequency_count = janes->john;
		
		std::string histogram_table = make_histogram_table("John Smith",
		        "Jane Smith", "Joe Smith", &john_frequency_count,
		        &jane_frequency_count, &joe_frequency_count);

        std::cout << "Results for n == " << n << ", w == " << w << std::endl;

		/*
		 	This is a good place to output your timing data.
		 */

        std::cout << histogram_table << std::endl;
		
		gettimeofday(&finish_time, NULL);
		
		start_usecs = (start_time.tv_sec * 1e6) + start_time.tv_usec;
        finish_usecs = (finish_time.tv_sec * 1e6) + finish_time.tv_usec;


        /*
            This time you're up a creek.
            What goes in this section of the code?
            Hint: it looks a bit like what went here
            in MP7, but only a *little* bit.
         */

        ofs.close();
		std::cout << "Time to completion: " << std::to_string((finish_usecs - start_usecs)) << " usecs" << std::endl;
        std::cout << "Sleeping..." << std::endl;
        usleep(10000);
        std::string finale = chan->send_request("quit");
        std::cout << "Finale: " << finale << std::endl;
    }
	else if (pid == 0)
		execl("dataserver", NULL);
}
