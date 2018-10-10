/* 
    File: my_allocator.c

    Author: <your name>
            Department of Computer Science
            Texas A&M University
    Date  : <date>

    Modified: 

    This file contains the implementation of the module "MY_ALLOCATOR".

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include<stdlib.h>
#include<math.h>
#include "my_allocator.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

/* Don't forget to implement "init_allocator" and "release_allocator"! */

unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length){
	basic_block_size = _basic_block_size;
	BB_index = (int)(log((double)basic_block_size)/log(2.0));
	
	
	MP_size = _length;
	
	
	MP = (char*)calloc(_length,1);
	block_list = (struct header**)calloc((int)(log((double)MP_size)/log(2.0)) - BB_index + 1,sizeof(struct header*));
	
	
	struct header* begin_begin = (struct header*) MP;
	begin_begin->free = '0';
	begin_begin->size = MP_size;
	
	begin_begin->next = NULL;
	block_list[(int)(log((double)MP_size)/log(2.0)) - BB_index] = begin_begin;
	
	
	return 0;
}

void re_init(){
	memset(MP, '\0', sizeof(MP));
	struct header* begin_begin = (struct header*) MP;
	begin_begin->free = '0';
	begin_begin->size = MP_size;
	
	
	begin_begin->next = NULL;
	block_list[(int)(log((double)MP_size)/log(2.0)) - BB_index] = begin_begin;
}
				
int release_allocator(){
	free(MP);
	
	free(block_list);
	MP = NULL;
	
	return 0;
} 



void PrintList(){
	int b_size = MP_size;
	int blockCount = 0;
	
	int i;
	printf("\n");
	for(i = ((int)(log((double)MP_size)/log(2.0)) - BB_index); i>=0; i--){
		if(block_list[i]==NULL){
			//Do nothing
		}else{
			struct header* curr = block_list[i];
			while(1){
				if(curr->next == NULL){
					blockCount++;
					
					break;
				}else{
					blockCount++;
					curr = curr->next;
				}
			}
		}
		printf("Block size: %d       \nBlocks free: %d \n", b_size, blockCount);
		blockCount = 0;
		b_size/=2;
	}
}


Addr my_malloc(unsigned int _length) {
	if(MP == NULL){
		printf("Allocator has been already released. Further allocation is impossible.\n");
		return NULL;
	}
	//figure out what size block is needed
	int size_needed = basic_block_size;
	
	while(1){
		if(size_needed >= _length + 13){
			break;
		}
		size_needed = size_needed*2;
	}
	//figure out what size is available
	int size_available = size_needed;
	
	while(1){
		if(size_available > MP_size){
			printf("Not enough space in allotted memory pool");
			return NULL;
		}
		//If there is a block in the list, we found our available size.
		if(block_list[(int)(log((double)size_available)/log(2.0)) - BB_index] != NULL){
			
			break;
		}
		
		size_available *= 2;
	}
	//split and create block until block created
	while(1){
		if(size_available == size_needed){
			struct header* curr = block_list[(int)(log((double)size_available)/log(2.0)) - BB_index];
			block_list[(int)(log((double)size_available)/log(2.0)) - BB_index] = curr->next;
			
			
			curr->free = '1'; //1 means it is no longer free.
			curr->next = NULL;
			return (char*) curr+13;
		}else{
			struct header* curr = block_list[(int)(log((double)size_available)/log(2.0)) - BB_index];
			char* temp = (char*) curr;
			struct header* currBuddy = (struct header*) (temp + size_available/2);
			currBuddy->next = curr->next;
			curr->next = currBuddy;
			
			curr->size = size_available/2;
			currBuddy->size = size_available/2;
			curr->free = '0';
			
			
			currBuddy->free = '0';
			//Set zero pointer equal to the what's next (whether its a block or null)
			block_list[(int)(log((double)size_available)/log(2.0)) - BB_index] = currBuddy->next;
			
			size_available /= 2;
			block_list[(int)(log((double)size_available)/log(2.0)) - BB_index] = curr;
			currBuddy->next = NULL;
			
		}
	}
	PrintList();
  return (void *)0;
}

int combine_blocks(){
	int final_index = (int)(log((double)MP_size)/log(2.0)) - BB_index;
	int index = 0;
	while(1){
		if(index+1 > final_index)
			break;
		//traverse list of sizes
		
		while(1){
			//traverse list of blocks
			struct header* curr = block_list[index];
			if(curr == NULL || curr->next == NULL){
				break;
			}else if(curr->next == (struct header*)((((char*)curr - MP) ^ curr->size)+MP)){
				
				//make sure the lower address block becomes the header
				if(curr > curr->next){
					struct header* currBuddyNext = curr->next->next;
					struct header* currBuddy = curr->next;
					struct header* currOriginal= curr;
					curr = currBuddy;
					
					
					curr->next = currOriginal;
					curr->next->next = currBuddyNext;
				}
				
				
				//consolidate
				curr->next = curr->next->next;
				curr->size = (curr->size)*2;
				block_list[index] = curr->next;
				//Put into the free list above.
				struct header* iter = block_list[index+1];
				while(1){
					if(iter == NULL){
						
						
						
						block_list[index+1] = curr;
						break;
					}else if(iter->next == NULL){
						iter->next = curr;
						break;
					}else if(iter == (struct header*)((((char*)curr - MP) ^ curr->size)+MP)){
						curr->next = iter->next;
						iter->next = curr;
						break;
					}
					iter = iter->next;
				}
			}
			curr = curr->next;
		}
		index++;
	}	
	return 0;
}

struct header* back_of_list(int size){
	struct header* curr = block_list[(int)(log((double)size)/log(2.0)) - BB_index];
	while(1){
		if(curr->next == NULL){
			return curr;
			
			
		}
		curr = curr->next;
	}
}



extern int my_free(Addr _a) {
  struct header* help = (struct header*) ((char*) _a - 13);
  help->free = '\0';
  if(block_list[(int)(log((double)(help->size))/log(2.0)) - BB_index] == NULL){
	  block_list[(int)(log((double)(help->size))/log(2.0)) - BB_index] = help;
	  
	  
	  
	  
  } else {
	  back_of_list(help->size)->next = help;
  }
  combine_blocks();
  if(block_list[((int)(log((double)MP_size)/log(2.0)) - BB_index)] != NULL){
	  
	  
  	//clean up
  	re_init();
  }
  PrintList();
  return 0;
}

