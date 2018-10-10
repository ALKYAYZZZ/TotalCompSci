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
	BB_size = _basic_block_size;
	BB_index = (int)(log((double)BB_size)/log(2.0));
	MP_size = _length;
	MP = (char*)calloc(_length,1);
	block_list = (struct header**)calloc((int)(log((double)MP_size)/log(2.0)) - BB_index + 1,sizeof(struct header*));
	struct header* begin_block = (struct header*) MP;
	begin_block->free = '0';
	begin_block->size = MP_size;
	begin_block->next = NULL;
	block_list[(int)(log((double)MP_size)/log(2.0)) - BB_index] = begin_block;
	return 0;
}

void re_init(){
	memset(MP, '\0', sizeof(MP));
	struct header* start_block = (struct header*) MP;
	start_block->free = '0';
	start_block->size = MP_size;
	start_block->next = NULL;
	block_list[(int)(log((double)MP_size)/log(2.0)) - BB_index] = start_block;
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
		printf("Size: %d      \n Num Free: %d \n", b_size, blockCount);
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
	int size_needed = BB_size;
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
			struct header* current_block = block_list[(int)(log((double)size_available)/log(2.0)) - BB_index];
			block_list[(int)(log((double)size_available)/log(2.0)) - BB_index] = current_block->next;
			current_block->free = '1'; //1 means it is no longer free.
			current_block->next = NULL;
			return (char*) current_block+13;
		}else{
			struct header* current_block = block_list[(int)(log((double)size_available)/log(2.0)) - BB_index];
			char* temp = (char*) current_block;
			struct header* this_buddy = (struct header*) (temp + size_available/2);
			this_buddy->next = current_block->next;
			current_block->next = this_buddy;
			current_block->size = size_available/2;
			this_buddy->size = size_available/2;
			current_block->free = '0';
			this_buddy->free = '0';
			//Set zero pointer equal to the what's next (whether its a block or null)
			block_list[(int)(log((double)size_available)/log(2.0)) - BB_index] = this_buddy->next;
			
			size_available /= 2;
			block_list[(int)(log((double)size_available)/log(2.0)) - BB_index] = current_block;
			this_buddy->next = NULL;
		}
	}
  return (void *)0;
}

int consolidate_blocks(){
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
  struct header* tbd = (struct header*) ((char*) _a - 13);
  tbd->free = '\0';
  if(block_list[(int)(log((double)(tbd->size))/log(2.0)) - BB_index] == NULL){
	  block_list[(int)(log((double)(tbd->size))/log(2.0)) - BB_index] = tbd;
  } else {
	  back_of_list(tbd->size)->next = tbd;
  }
  consolidate_blocks();
  if(block_list[((int)(log((double)MP_size)/log(2.0)) - BB_index)] != NULL){
  	//clean up
  	re_init();
  }
  return 0;
}

