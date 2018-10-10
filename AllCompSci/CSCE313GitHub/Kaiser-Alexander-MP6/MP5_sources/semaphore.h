/* 
    File: semaphore.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/11

*/

#ifndef _semaphore_H_                   // include file only once
#define _semaphore_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <pthread.h>
using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   s e m a p h o r e  */
/*--------------------------------------------------------------------------*/

class semaphore {
private:
    /* -- INTERNAL DATA STRUCTURES */
	int count;
	pthread_mutex_t mutex;
	pthread_cond_t cond;

public:

    /* -- CONSTRUCTOR/DESTRUCTOR */

    semaphore(int _val) {
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	count = _val;
	}

    ~semaphore(){
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
    }

    /* -- SEMAPHORE OPERATIONS */
    
    void P() {
	pthread_mutex_lock(&mutex);
	
	while(count <= 0){
		pthread_cond_wait(&cond,&mutex);
	}
	count --;
	
	pthread_mutex_unlock(&mutex);
    }

    void V() {
	pthread_mutex_lock(&mutex);
    count ++;
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond);
    

    }
};

#endif


