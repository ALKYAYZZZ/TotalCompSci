#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _my_allocator_h_                   // include file only once
#define _my_allocator_h_

#define HEADER_SIZE 13

typedef void * Addr; 

struct	header{
	struct header* next;
	int size;
	char free;
};

char* MP;
struct header** block_list;
int MP_size;
int BB_size;
int BB_index;


unsigned int init_allocator(unsigned int _basic_block_size, 
			    unsigned int _length); 
/* This function initializes the memory allocator and makes a portion of 
   ’_length’ bytes available. The allocator uses a ’_basic_block_size’ as 
   its minimal unit of allocation. The function returns the amount of 
   memory made available to the allocator. If an error occurred, 
   it returns 0. 
*/ 

int release_allocator(); 
/* This function returns any allocated memory to the operating system. 
   After this function is called, any allocation fails.
*/ 

Addr my_malloc(unsigned int _length); 
/* Allocate _length number of bytes of free memory and returns the 
   address of the allocated portion. Returns 0 when out of memory. */ 

int my_free(Addr _a); 
/* Frees the section of physical memory previously allocated 
   using ’my_malloc’. Returns 0 if everything ok. */ 
   
void PrintList();

#endif 
