/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/linked_list2.cpp                                                */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include <iostream>
#include <string.h>
#include <climits>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include "linked_list2.h"

linked_list2::linked_list2()
{
	
}

void linked_list2::Init(int M, int b, int t)
{
	mem_size = M;
	block_size = b;
	max_data_size = M/b;
	num_tiers = t;
	initialized = true;
	int data_count = 0;
	
	head_pointer = (char **)malloc(t*sizeof(char*));
	free_pointer = new node*[t];
	free_data_pointer = new node*[t];
	front_pointer = new node*[t];
	
	for(int i = 0; i < t; i++){
		head_pointer[i] = (char *)malloc(M/t);
		free_data_pointer[i] = (node *)head_pointer[i];
		front_pointer[i] = (node *)head_pointer[i];
		free_pointer[i] = front_pointer[i];
	}
	
	//char * tmp = this->head_pointer;
	
	for(int i = 0; i < t; i++){
	char * tmp = this->head_pointer[i];
		while(tmp <= head_pointer[i] + (M/t) - b){
			node* newnode = (node*)tmp;
			newnode->next = (node*)(tmp + b);
			newnode->key = 0;
			newnode->value_len = 0;
			tmp += b;
		}
		node * tmp_last = ((node *)tmp)->next;
		tmp_last = NULL;
	}
}


void linked_list2::Destroy()
{
	free(head_pointer);
}

void linked_list2::Insert(int k,char * data_ptr, int data_len)
{
	int max = INT_MAX;
	int div = max/num_tiers;
	int counter = 0;
	bool front_init = true;
	
	while(div <= k){
		div = div + div;
		counter++;
	}
	
	if(free_pointer[counter] == front_pointer[counter]){
		front_init = false;
		//std::cerr << "CAOAISUDFFNM,AO";
	}
	
	node * beginning = this->free_pointer[counter];
	node * free_temp = this->free_data_pointer[counter];

	if(16 + data_len > this->block_size){
		std::cout << "This string is too long!\n";
	}
	else if(max_data_size == 0){
		std::cout << "The list is full!\n";
	}
	else{
			
			if(front_pointer[counter] != NULL){
				if(free_data_pointer[counter]->key == -1){
					std::cerr << free_pointer[counter]->key;
					node * trav = front_pointer[counter];
					while(trav->key != -1){
						trav = trav->next;
					}
					free_data_pointer[counter] = trav;
					
					node * temp = free_pointer[counter];
					beginning = free_data_pointer[counter];
					free_data_pointer[counter] = temp;
					beginning->key = k;
					beginning->value_len = data_len;
					memcpy((char *)beginning + sizeof(node),(void*)data_ptr, data_len);
					if(max_data_size == mem_size/block_size){
						front_pointer[counter] = beginning;
					}
					while(trav != free_pointer[counter]){
						trav = trav->next;
						if(trav->key == -1){
							free_data_pointer[counter] = trav;
							break;
						}
					}
				}
				else{
					beginning = free_temp;
					free_pointer[counter] = free_pointer[counter]->next;
					free_data_pointer[counter] = free_data_pointer[counter]->next;
					beginning->key = k;
					beginning->value_len = data_len;
					memcpy((char *)beginning + sizeof(node),(void*)data_ptr, data_len);
					if(front_init == false){
						front_pointer[counter] = beginning;
					}
				}
				max_data_size--;
				
			}
				
	}

}

int linked_list2::Delete(int delete_key)
{
	int max = INT_MAX;
	int div = max/num_tiers;
	int counter = 0;
	bool front_init = true;
	
	while(div <= delete_key){
		div = div + div;
		counter++;
	}
	
	if(free_pointer[counter] == front_pointer[counter]){
		front_init = false;
		//std::cerr << "CAOAISUDFFNM,AO";
	}
	
	
	bool breakbool = true;
	node * beginning = this->front_pointer[counter];
	node * free_temp = this->free_pointer[counter];
	node * free_data_temp = this->free_data_pointer[counter];
	if(front_init == false){
		std::cout << "There is nothing to delete!\n";
	}
	else if(beginning->key == delete_key){
				node * temp = beginning->next;
				node * free_next = free_pointer[counter]->next;
				beginning->next = free_data_pointer[counter];
				free_data_pointer[counter] = beginning;
				free_data_pointer[counter]->next = temp;
				beginning->key = -1;
				beginning->value_len = 0;
				memcpy((char *)beginning + sizeof(node),NULL,0);
				
				max_data_size++;
	}
	else{
		while(beginning->next != NULL){
			if(beginning->next->key == delete_key){
				node * temp = beginning->next->next;
				beginning->next->next = free_data_pointer[counter];
				free_data_pointer[counter] = beginning->next;
				free_data_pointer[counter]->next = temp;
				beginning->next->key = -1;
				beginning->next->value_len = 0;
				memcpy((char *)beginning + sizeof(node),NULL,0);

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

node* linked_list2::Lookup(int lookup_key)
{
	int max = INT_MAX;
	int div = max/num_tiers;
	int counter = 0;
	
	while(div <= lookup_key){
		div = div + div;
		counter++;
	}
	
	node * pointer = this->front_pointer[counter];
	while(pointer->key != lookup_key){
		if((pointer == free_pointer[counter]) || ((pointer->next == NULL) && (pointer->key != lookup_key))){
			return NULL;
		}
		else{
			pointer = pointer->next;
		}
	}
	
	return pointer;
}

void linked_list2::PrintList()
{
	
	/* IMPORTANT NOTE!
	 * 
	 * In order for the script that will grade your assignment to work 
	 * (i.e. so you get a grade higher than a 0),
	 * you need to print out each member of the list using the format below.  
	 * Your print list function should be written as a while loop that prints 
	 * the tier of the list and then each node underneath it.  
	 * the following four lines exactly for each node and nothing else.  If
	 * you have any difficulties, talk to your TA and he will explain it further. 
	 * 
	 * The output lines that you should use are provided so that you will know
	 * exactly what you should output.  
	 */ 
	
	for(int i = 0; i < num_tiers; i++){
		std::cout << "Tier " << i + 1 << std::endl;
		
		node * pointer = this->front_pointer[i];
		while(pointer != free_pointer[i]){
		if(pointer->key != -1){
			std::cout << "Node: " << std::endl;
			std::cout << " - Key: " << pointer->key << std::endl;
			std::cout << " - Data: " << (char *)pointer + sizeof(node) << std::endl;
		}
		pointer = pointer->next;
		}
	}
	
	/* Short example:
	 *   - Assume a list with two tiers each with two elements in each tier.  
	 *     the print out would appear as follows
	 * 
	 * Tier 0
	 * Node: 
	 *  - Key: 1
	 *  - Data: Hello
	 * Node:
	 *  - Key: 2
	 *  - Data: World!
	 * Tier 1
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

int linked_list2::Find_tier(int key)
{

}

/* Getter Functions */
char** linked_list2::getHeadPointer()
{

}

node** linked_list2::getFrontPointer()
{

}

node** linked_list2::getFreePointer()
{

}

node** linked_list2::getFreeDataPointer()
{

}

int linked_list2::getBlockSize()
{

}

int linked_list2::getMemSize()
{

}

int linked_list2::getMaxDataSize()
{

}

int linked_list2::getNumTiers()
{

}

int linked_list2::getInitialized()
{

}

/* Setter Functions */
void linked_list2::setHeadPointer(char** new_head_pointer)
{

}

void linked_list2::setFrontPointer(node** new_front_pointer)
{

}

void linked_list2::setFreePointer(node** new_free_pointer)
{

}

void linked_list2::setFreeDataPointer(node** new_free_data_pointer)
{

}

void linked_list2::setBlockSize(int new_block_size)
{

}

void linked_list2::setMemSize(int new_mem_size)
{

}

void linked_list2::setMaxDataSize(int new_max_data_size)
{

}

void linked_list2::setNumTiers(int new_num_tiers)
{

}

void linked_list2::setInitialized(bool new_initialized)
{

}
