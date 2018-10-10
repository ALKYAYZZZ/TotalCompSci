#include "semaphore.h"

void semaphore::P(){
	
	pthread_mutex_lock(&mutex);
	
	while(count <= 0){
		pthread_cond_wait(&cond,&mutex);
	}
	--count;
	
	pthread_mutex_unlock(&mutex);
	
}

void semaphore::V(){
	pthread_mutex_lock(&mutex);
    count ++;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
	
}