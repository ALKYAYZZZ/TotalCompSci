#include <cassert>
#include <cstring>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

class semaphore{
int count;
pthread_mutex_t mutex;
pthread_cond_t cond;


public:
semaphore(int value){
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	count = value;
};

~semaphore(){
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	
};

void P();
void V();

};