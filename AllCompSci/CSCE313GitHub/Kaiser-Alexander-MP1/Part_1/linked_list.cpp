/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/linked_list.cpp                                                 */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include "linked_list.h"


/* Constructor */
linked_list::linked_list()
{

}


void linked_list::Init(int M, int b)
{
	mem_size = M;
	block_size = b;
	max_data_size = M/b;
	initialized = true;
	int data_count = 0;
	
	this->head_pointer = (char *)malloc(M);
	
	
	this->free_data_pointer = (node *)this->head_pointer;
	this->front_pointer = (node *)this->head_pointer;
	this->free_pointer = front_pointer;
	
	
	char * tmp = this->head_pointer;
	
	while(tmp <= head_pointer + M - b){
		node* newnode = (node*)tmp;
		newnode->next = (node*)(tmp + b);
		newnode->key = 0;
		newnode->value_len = 0;
		tmp += b;
	}
	node * tmp_last = ((node *)tmp)->next;
	tmp_last = NULL;
}

void linked_list::Destroy()
{
	std::free(head_pointer);
} 

/* Insert an element into the list with a given key, given data element, and with a given length*/
void linked_list::Insert (int k, char * data_ptr, int data_len)
{
	
	node * beginning = this->free_pointer;
	node * free_temp = this->free_data_pointer;
	
	if(16 + data_len > this->block_size){
		std::cout << "This string is too long!\n";
	}
	else if(max_data_size == 0){
		std::cout << "The list is full!\n";
	}
	else{
		
			if(front_pointer != NULL){
				if(free_data_pointer->key == -1){
					std::cerr << free_pointer->key;
					node * trav = front_pointer;
					while(trav->key != -1){
						trav = trav->next;
					}
					free_data_pointer = trav;
					
					node * temp = free_pointer;
					beginning = free_data_pointer;
					free_data_pointer = temp;
					beginning->key = k;
					beginning->value_len = data_len;
					std::memcpy((char *)beginning + sizeof(node),(void*)data_ptr, data_len);
					if(max_data_size == mem_size/block_size){
						front_pointer = beginning;
					}
					while(trav != free_pointer){
						trav = trav->next;
						if(trav->key == -1){
							free_data_pointer = trav;
							break;
						}
					}
				}
				else{
					beginning = free_temp;
					free_pointer = free_pointer->next;
					free_data_pointer = free_data_pointer->next;
					beginning->key = k;
					beginning->value_len = data_len;
					std::memcpy((char *)beginning + sizeof(node),(void*)data_ptr, data_len);
					if(max_data_size == mem_size/block_size){
						front_pointer = beginning;
					}
				}
				max_data_size--;
				
				
				
				
			}
				
	}
	

	
}


int linked_list::Delete(int delete_key)
{
	bool breakbool = true;
	node * beginning = this->front_pointer;
	node * free_temp = this->free_pointer;
	node * free_data_temp = this->free_data_pointer;
	if(max_data_size == mem_size/block_size){
		std::cout << "There is nothing to delete!\n";
	}
	else if(beginning->key == delete_key){
				node * temp = beginning->next;
				node * free_next = free_pointer->next;
				beginning->next = free_data_pointer;
				free_data_pointer = beginning;
				free_data_pointer->next = temp;
				beginning->key = -1;
				beginning->value_len = 0;
				std::memcpy((char *)beginning + sizeof(node),NULL,0);
				
				max_data_size++;
	}
	else{
		while(beginning->next != NULL){
			if(beginning->next->key == delete_key){
				node * temp = beginning->next->next;
				beginning->next->next = free_data_pointer;
				free_data_pointer = beginning->next;
				free_data_pointer->next = temp;
				beginning->next->key = -1;
				beginning->next->value_len = 0;
				std::memcpy((char *)beginning + sizeof(node),NULL,0);

				max_data_size++;
				breakbool = false;
				break;
			}
			else{
				beginning = beginning->next;
			}
		}
		if(breakbool == true){
			std::cout << "This key is not in this list!\n";
		}
	}		

}

/* Iterate through the list, if a given key exists, return the pointer to it's node */
/* otherwise, return NULL                                                           */
struct node* linked_list::Lookup(int lookup_key)
{
	node * pointer = this->front_pointer;
	while(pointer->key != lookup_key){
		if((pointer == free_pointer) || ((pointer->next == NULL) && (pointer->key != lookup_key))){
			return NULL;
		}
		else{
			pointer = pointer->next;
		}
	}
	
	return pointer;
}

/* Prints the list by printing the key and the data of each node */
void linked_list::PrintList()
{
	
	/* IMPORTANT NOTE!
	 * 
	 * In order for the script that will grade your assignment to work 
	 * (i.e. so you get a grade higher than a 0),
	 * you need to print out each member of the list using the format below.  
	 * Your print list function should be written as a while loop that prints
	 * the following three lines exactly for each node and nothing else.  If
	 * you have any difficulties, talk to your TA and he will explain it further.  
	 * 
	 * The output lines that you should use are provided so that you will know
	 * exactly what you should output.  
	 */ 
	node * pointer = this->front_pointer;
	while(pointer != free_pointer){
	if(pointer->key != -1){
		std::cout << "Node: " << std::endl;
		std::cout << " - Key: " << pointer->key << std::endl;
		std::cout << " - Data: " << (char *)pointer + sizeof(node) << std::endl;
	}
	pointer = pointer->next;
	}
	
	/* Short example:
	 *   - Assume that you have a list with 4 elements.  
	 *     Your output should appear as follows
	 * 
	 * Node:
	 *  - Key: 1
	 *  - Data: Hello
	 * Node:
	 *  - Key: 2
	 *  - Data: World!
	 * Node:
	 *  - Key: 3
	 *  - Data: Hello
	 * Node:
	 *  - Key: 4
	 *  - Data: World!
	 * 
	 * ^^ Your output needs to exactly match this model to be counted as correct.  
	 * (With the exception that the values for key and data will be different 
	 * depending on what insertions you perform into your list.  The values provided
	 * here are for pedagogical purposes only)
	 */
}

/* Getter Functions */
char* linked_list::getHeadPointer()
{
	return this->head_pointer;
}

node* linked_list::getFrontPointer()
{
	return this->front_pointer;
}

node* linked_list::getFreePointer()
{
	return this->free_pointer;
}

node* linked_list::getFreeDataPointer()
{
	return this->free_data_pointer;
}

int linked_list::getBlockSize()
{
	return this->block_size;
}

int linked_list::getMemSize()
{
	return this->mem_size;
}

int linked_list::getMaxDataSize()
{
	return this->max_data_size;
}

bool linked_list::getInitialized()
{
	return this->initialized;
}

/* Setter Functions */
void linked_list::setHeadPointer(char *new_pointer)
{
	head_pointer = new_pointer;
}

void linked_list::setFrontPointer(node* new_pointer)
{
	front_pointer = new_pointer;
}

void linked_list::setFreePointer(node* new_pointer)
{
	free_pointer = new_pointer;
}

void linked_list::setFreeDataPointer(node* new_pointer)
{
	free_data_pointer = new_pointer;
}

void linked_list::setBlockSize(int new_block_size)
{
	block_size = new_block_size;
}

void linked_list::setMemSize(int new_mem_size)
{
	mem_size = new_mem_size;
}

void linked_list::setMaxDataSize(int new_max_data_size)
{
	max_data_size = new_max_data_size;
}

void linked_list::setInitialized(bool new_initialized)
{
	initialized = new_initialized;
}
