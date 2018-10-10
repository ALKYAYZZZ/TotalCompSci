/* ------------------------------------------------------------------------ */
/* Developer: Andrew Kirfman, Dr. Betatti                                   */
/* Project: CSCE-313 Machine Problem 2                                      */
/*                                                                          */
/* File: ./my_allocator.c                                                   */
/* ------------------------------------------------------------------------ */

/*--------------------------------------------------------------------------*/
/* Standard Library Includes                                                */
/*--------------------------------------------------------------------------*/

#include <stdlib.h>
#include <cstdint>
#include <math.h>

/*--------------------------------------------------------------------------*/
/* User Defined Includes                                                    */
/*--------------------------------------------------------------------------*/

#include "my_allocator.h"
#include "ackerman.h"

/*--------------------------------------------------------------------------*/
/* Functions For Module MyAllocator                                         */
/*--------------------------------------------------------------------------*/

int total_alloc_mem;

unsigned int MyAllocator::higher_two(unsigned int number)
{
	number--;
	number |= number >> 1;
	number |= number >> 2;
	number |= number >> 4;
	number |= number >> 8;
	number |= number >> 16;
	number++;
	return number;
}

unsigned int MyAllocator::lower_two(unsigned int number)
{
	
	number |= number >> 1;
	number |= number >> 2;
	number |= number >> 4;
	number |= number >> 8;
	number |= number >> 16;
	return number - (number >> 1);
}

bool MyAllocator::isPowerOfTwo(unsigned int x)
{
	if((x & (x - 1) == 0) && (x != 0)){
		return true;
	}
	else 
		return false;
}

MyAllocator::MyAllocator(unsigned int _basic_block_size,
	unsigned int _mem_size)
{
	init_allocator(_basic_block_size, _mem_size);
}

unsigned int MyAllocator::init_allocator(unsigned int _basic_block_size,
	unsigned int _mem_size)
{
	total_alloc_mem = 0;
	struct header *address;
	unsigned int temp;
	unsigned int number_of_blocks;
	mem_size = higher_two(_mem_size);
	basic_block_size = _basic_block_size;
	initialized = true;
	memory_array = new std::map<int, Addr>;
	if(isPowerOfTwo(_mem_size) == true){
		temp = _mem_size + sizeof(header);
		number_of_blocks = temp/_basic_block_size;
	}
	else{
		temp = higher_two(_mem_size + sizeof(header));
		number_of_blocks = temp/_basic_block_size;
	}

	
	head_pointer = (char*)malloc(temp);
	address = (header*)head_pointer;
	int block_power = higher_two(_basic_block_size);
	int count = 1;
	while(temp > block_power){
		count++;
		temp = temp / 2;
	}
	num_tiers = count;
	address->next = NULL;
	address->block_size = higher_two(_mem_size);
	address->in_use = false;
	(*memory_array)[higher_two(_mem_size)] = address;
	int temp_key = lower_two(_mem_size - 1);
	while(temp_key >= block_power){
		(*memory_array)[temp_key] = NULL;
		temp_key--;
		temp_key = lower_two(temp_key);
	}
	
	printf("Memory size is %d and block size is %d.\n",mem_size,basic_block_size);
	std::cout << std::endl;
}

void MyAllocator::release_allocator()
{
	free(head_pointer);
	delete(memory_array);
}


Addr MyAllocator::my_malloc(unsigned int _length)
{	
	struct header *temp_block;
	unsigned int temp;
	temp_block = (header*)head_pointer;
	if(_length <= basic_block_size){
		_length = basic_block_size - sizeof(header);
	}
	
	temp = _length + sizeof(header);
	if(isPowerOfTwo(temp) == true){
		temp = _length + sizeof(header);
	}
	else{
		temp = higher_two(_length + sizeof(header));
	}
	
	printf("Allocating block of size: %d\n", temp);
	
	if(total_alloc_mem + temp > mem_size){
		printf("No More Memory Available!\n");
		return NULL;
	}
	else{
		int base_size = temp;
		struct header *level_block = (header*)(*memory_array)[temp];
		/*
		if(level_block == NULL){
			
			while(temp_block->block_size > temp){
				split_block(temp_block);
			}
			total_alloc_mem = total_alloc_mem + temp_block->block_size;
			temp_block->in_use = true;
			return temp_block + 1;
		}
		else{
		*/
		int funz = 128;
		struct header *yesz = (header*)(*memory_array)[funz];
		while(yesz == NULL){
			funz = higher_two(funz + 1);
			yesz = (header*)(*memory_array)[funz];
		}
		if((char*)yesz->next < head_pointer || (char*)yesz->next > (head_pointer + mem_size)){
			std::cerr << "what" << std::endl;
			yesz->next = NULL;
		}
			while(temp <= mem_size){
			level_block = (header*)(*memory_array)[temp];
			//std::cerr << level_block->next->block_size << std::endl;
			
			if(level_block != NULL){
					while((level_block->in_use == true) && level_block->next != NULL){
						std::cerr << "OMGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"<< std::endl;
						level_block = level_block->next;

					}
					
					if(level_block->next == NULL){
						std::cerr << "RAWR" << std::endl;
						if(level_block->in_use == false){
							//std::cerr << level_block->block_size << std::endl;
							if(level_block->block_size != base_size){
								while(level_block->block_size > base_size){
									split_block(level_block);
									//std::cerr << level_block->block_size << std::endl;
								}
							
							}
							level_block->in_use = true;
							total_alloc_mem = total_alloc_mem + level_block->block_size;
							temp = level_block->block_size;
							std::cerr << "Level Block: "<< level_block << std::endl;
							std::cerr << "Level Block Next: "<< level_block->next << std::endl;
							struct header *no = (header*)(*memory_array)[temp];
							int fun = 128;
							struct header *yes = (header*)(*memory_array)[fun];
							while(yes == NULL){
								fun = higher_two(fun + 1);
								yes = (header*)(*memory_array)[fun];
							}
							std::cerr << "Header: " << (header*)head_pointer << std::endl;
							std::cerr << "Temp: " << temp << std::endl;
							std::cerr << "Map Location: " << no << std::endl;
							std::cerr << "FUN: " << fun << std::endl;
							std::cerr << "Map Location Bottom: " << yes << std::endl;
							std::cerr << "Map Location Bottom Next: " << yes->next << std::endl;
							if((char*)yes->next < head_pointer || (char*)yes->next > (head_pointer + mem_size)){
								std::cerr << "what" << std::endl;
								yes->next = NULL;
							}
							/*
							if(level_block->next == NULL){
								(*memory_array)[temp] = NULL;
								printf("%d is now null.\n",temp);
							}
							else{
								std::cerr << "FUUUUCKKKKKKKKKKKKKKKKKKKKKKKKKK" << std::endl;
								(*memory_array)[temp] = level_block->next;
							}
							*/
							std::cerr << "FUUUUCKKKKKKKKKKKKKKKKKKKKKKKKKK" << std::endl;
							(*memory_array)[temp] = level_block->next;

							return level_block + 1;
						}
						else{
							temp = higher_two(temp + 1);
						}
					}
					else{
						std::cerr << "Else: " << std::endl;
						level_block->in_use = true;
						total_alloc_mem = total_alloc_mem + level_block->block_size;
						struct header *no = (header*)(*memory_array)[temp];
						int fun = 128;
						struct header *yes = (header*)(*memory_array)[fun];
						while(yes == NULL){
							fun = higher_two(fun + 1);
							yes = (header*)(*memory_array)[fun];
						}
						std::cerr << "Header: " << (header*)head_pointer << std::endl;
						std::cerr << "Temp: " << temp << std::endl;
						std::cerr << "Map Location: " << no << std::endl;
						std::cerr << "FUN: " << fun << std::endl;
						std::cerr << "Map Location Bottom: " << yes << std::endl;
						std::cerr << "Level Block Next" << level_block->next << std::endl;
						if((char*)yes->next > head_pointer || (char*)yes->next < (head_pointer + mem_size)){
							yes->next = NULL;
						}
						(*memory_array)[temp] = level_block->next;

						return level_block + 1;
					}
				}
				else{
				temp = higher_two(temp + 1);
				}
			}
			
		}
	}
//}

bool MyAllocator::split_block(Addr start_address)
{
	struct header *temp = (header*)start_address;
	int first_size = temp->block_size;
	std::cerr << "First Size: " << first_size << std::endl;
	int size = lower_two(first_size - 1);
	std::cerr << "Lower Size: " << size << std::endl;
	struct header *first_half = (header*)start_address;
	first_half->next = (header*)((char*)start_address + size);
	if((*memory_array)[size] == NULL){
		(*memory_array)[size] = (header*)start_address;
	}
	std::cerr << "Start Address: " << (header*)start_address << std::endl;
	std::cerr << "Start Address Next: " << (header*)first_half->next << std::endl;
	/*
	else{
		struct header *not_null = (header*)((*memory_array)[size]);
		while((char*)not_null != (char*)first_half){
			not_null = (header*)((char*)not_null + size);
		}
		not_null = (header*)((char*)not_null - size);
		not_null->next = first_half;
	}
	*/

	first_half->block_size = size;

	first_half = first_half->next;
	std::cerr << "Second Half: " << (header*)first_half << std::endl;
	first_half->block_size = size;
	first_half->next = NULL;
	std::cerr << "Start Address Next Next: " << (header*)first_half->next << std::endl;
	//std::cerr << "difference: " << (char*)(start_address) - (char*)head_pointer << std::endl;
	int test_num;
	if(mem_size != first_size){
		test_num = higher_two(first_size + 1); 
	}
	else{
		test_num = mem_size;
	}
	bool superbool = false;
	struct header *test_new = (header*)start_address;
	struct header *test_two = (header*)(*memory_array)[first_size];
	struct header *block_above;
	if(((((char*)start_address + first_size) - (char*)head_pointer) % test_num == 0) && test_new->in_use != true){

		(*memory_array)[first_size] = NULL;
		printf("%d is now null.\n",first_size);
	}
	else{
		while((char*)test_new <= (head_pointer + mem_size)){
			if(test_new != NULL && test_new != (header*)start_address){
				if(test_new->in_use != true && test_new->block_size == first_size){
					if(superbool == false){
						std::cerr << "Here I am!" << std::endl;
						(*memory_array)[first_size] = test_new;
						block_above = (header*)(*memory_array)[first_size];
						block_above->next = NULL;
						superbool = true;
					}
					else{
						std::cerr << "Hear me rd 2" << std::endl;
						block_above->next = test_new;
						test_new->next = NULL;
						block_above = test_new;
					}	
				}
			}
			test_new = (header*)((char*)test_new + first_size);
		}
		if(superbool == false){
			(*memory_array)[first_size] = NULL;
			printf("%d is now null.\n",first_size);
		}
		/*
		if(superbool == true){
			(*memory_array)[first_size] = test_new;
			struct header *block_above = (header*)(*memory_array)[first_size];
			block_above->next = NULL;
		}
		else{
			(*memory_array)[first_size] = NULL;
			printf("%d is now null.\n",first_size);
		}
		*/
	}
	
	
}

bool MyAllocator::are_buddies(Addr start_address1, Addr start_address2)
{
	struct header *temp1 = (header*)start_address1 - 1;
	struct header *temp2 = (header*)start_address2 - 1;
	if(temp1->block_size == temp2->block_size){
		int size = log2(temp1->block_size);
		int tempone = (char*)temp1 - head_pointer;
		int temptwo = (char*)temp2 - head_pointer;
		if(tempone - temptwo == temp1->block_size || temptwo - tempone == temp1->block_size){
			return true;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
}

bool MyAllocator::combine_blocks(Addr start_address1, Addr start_address2)
{
	if(are_buddies(start_address1,start_address2) == true){
		//std::cerr << "GOD";
		struct header *temp1 = ((header*)((char*)start_address1 - sizeof(header))) ;
		struct header *temp2 = ((header*)((char*)start_address2 - sizeof(header))) ;
		struct header *new_block;
		int size = temp1->block_size;
		int total = 0;
		std::cerr << "Mem Array: " << (header*)(*memory_array)[size] << std::endl;
		std::cerr << "Temp 1: " << temp1 << std::endl;
		std::cerr << "Temp 2: " << temp2 << std::endl;
		if((header*)(*memory_array)[size] == temp1 || (header*)(*memory_array)[size] == temp2){
			std::cerr << "DOGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG";
			struct header *block = ((header*)(*memory_array)[size]);
			if((char*)temp1 > (char*)temp2){
				if(temp1->next != NULL){
					std::cerr << "OMGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG" << std::endl;
					(*memory_array)[size] = temp1->next;
				}	
				else{
					(*memory_array)[size] = NULL;
					printf("%d is now null.\n",size);
				}
			}
			else{
				if(temp2->next != NULL){
					std::cerr << "OMGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG" << std::endl;
					(*memory_array)[size] = temp2->next;
				}	
				else{
					(*memory_array)[size] = NULL;
					printf("%d is now null.\n",size);
				}
			}
		}
		/*
		else{
			struct header *block = (header*)(*memory_array)[size];
			std::cerr << block;
			if((char*)temp1 > (char*)temp2){
				if(block != temp2){
					while(block->next != temp2){
						block = block->next;
					}
				}
				if(temp1->next != NULL){
					block->next = temp1->next;
				}	
				else{
					(*memory_array)[size] = NULL;
					printf("%d is now null.\n",size);
				}
			}
			else{
				if(block != temp1){
					while(block->next != temp1){
						block = block->next;
					}
				}
				if(temp2->next != NULL){
					block->next = temp2->next;
				}	
				else{
					(*memory_array)[size] = NULL;
					printf("%d is now null.\n",size);
				}
			}
		}
		*/
		if((char*)start_address1 > (char*)start_address2){
			struct header *start = ((header*)start_address1) - 1;
			start->block_size = 0;
			start->in_use = false;
			start->next = NULL;
			int size = temp1->block_size;
			int new_size = higher_two(size + 1);
			new_block = ((header*)((char*)start_address2 - sizeof(header)));
			//(*memory_array)[higher_two(size + 1)] = new_block;
			new_block->block_size = higher_two(size + 1);
		}
		else{
			struct header *start = ((header*)start_address2) - 1;
			start->block_size = 0;
			start->in_use = false;
			start->next = NULL;
			size = temp1->block_size;
			int new_size = higher_two(size + 1);
			new_block = ((header*)((char*)start_address1 - sizeof(header)));
			//(*memory_array)[higher_two(size + 1)] = new_block;
			new_block->block_size = higher_two(size + 1);
		}
		struct header *test_block = new_block;
		bool count = true;
		bool setbool = true;
		bool skipbool = true;
		if(test_block == NULL){
			(*memory_array)[higher_two(size + 1)] = test_block;
			skipbool = false;
		}
		else{
			int i = 0;
			struct header* prev = (header*)((char*)test_block + higher_two(size + 1));
			while((char*)test_block >= (char*)head_pointer){
				if(test_block != NULL){
					//std::cerr << "Test Block: "<< test_block->block_size << std::endl;
					//std::cerr << "Iteration: " << i << std::endl;
					if(test_block->block_size == new_block->block_size){
						std::cerr << new_block->block_size << std::endl;
						std::cerr << (char*)new_block - (char*)test_block << std::endl;
						if((((char*)new_block - (char*)test_block) % new_block->block_size == 0) && (((char*)new_block - (char*)test_block) != 0)){
							if(test_block->in_use == false){
								if((char*)test_block < head_pointer){
									std::cerr << "NOOOOO" << std::endl;
									break;
								}
								else{
									if(setbool == true){
										test_block->next = (header*)(new_block);
										prev = test_block;
										setbool = false;
									}
									else{
										test_block->next = prev;
										prev = test_block;
									}
									(*memory_array)[higher_two(size + 1)] = test_block;
									skipbool = false;
									std::cerr << "DAMN";
								}
							}
							
							
						}
					}
				}
				test_block = (header*)((char*)test_block - higher_two(size + 1));
				i++;
			}
		}
		
		std::cerr << std::endl;
		if(skipbool == true){
			(*memory_array)[higher_two(size + 1)] = new_block;
		}
		struct header *save_block = NULL;
		total = (char*)new_block - (char*)head_pointer;
		test_block = (header*)new_block;
		bool breakbool = false;
		while(total <= mem_size){
			if(test_block != NULL){
				//std::cerr << "Test Block: "<< test_block->block_size << std::endl;
				if(test_block->block_size == new_block->block_size){
					std::cerr << new_block->block_size << std::endl;
					if((((char*)test_block - (char*)new_block) % new_block->block_size == 0) && (((char*)new_block - (char*)test_block) != 0)){
						if(test_block->in_use == false){
							if((char*)test_block > (head_pointer + mem_size)){
								save_block->next = NULL;
							}
							else{
								if(save_block != NULL){
									save_block->next = test_block;
									test_block->next = NULL;
								}
								else{
									new_block->next = (header*)(test_block);
									test_block->next = NULL;
								}
							}
							save_block = test_block;
							breakbool = true;
							std::cerr << "DAMNn";
						}
					}
				}
			}
			test_block = (header*)((char*)test_block + higher_two(size + 1));
			total = total +  higher_two(size + 1);
		}
		if(breakbool == false){
			new_block->next = NULL;
		}
		return true;
	}
	else{
		return false;
	}
}

Addr MyAllocator::find_unused_buddy(Addr sibling_block)
{
	struct header *temp = ((header*)((char*)sibling_block - sizeof(header)));
	struct header *temp1 = temp + 1;
	struct header *temp2 = temp - 1;
	//std::cerr << temp->block_size << std::endl;
	//std::cerr << temp1->block_size << std::endl;
	//std::cerr << temp2->block_size << std::endl;
	unsigned int size = log2(temp->block_size);
	struct header *new_temp = (header*)(((char*)sibling_block + temp->block_size - sizeof(header)));
	if(new_temp != NULL && (char*)new_temp >= head_pointer){
		if(new_temp->block_size == temp->block_size){
			if(new_temp->in_use == false)
				return (char*)new_temp + sizeof(header);
			else{
				std::cerr << "Not in use" << std::endl;
				return NULL;
			}
		}
		else{
			std::cerr << "Not Right Size" << std::endl;
			return NULL;
		}
	}
	else{
			new_temp = (header*)((char*)sibling_block - temp->block_size - sizeof(header));
			if(new_temp != NULL && (char*)new_temp <= (head_pointer + mem_size)){
				if(new_temp->block_size == temp->block_size){
					if(new_temp->in_use == false)
						return (char*)new_temp + sizeof(header);
					else
						std::cerr << "Not in use" << std::endl;
						return NULL;
				}
					return NULL;
			}
			else{
				std::cerr << "not right size" << std::endl;
				return NULL;
			}
	}
}


bool MyAllocator::my_free(Addr _a)
{
	
	struct header *temp = ((header*)((char*)_a - sizeof(header)));
	bool breakbool = true;
	int orig_size = temp->block_size;
	int last_size = 0;
	
	if(_a != NULL){
		printf("Freeing block of size: %d\n", temp->block_size);
		int fun = 128;
		struct header *yes = (header*)(*memory_array)[fun];
		while(yes == NULL){
			fun = higher_two(fun + 1);
			yes = (header*)(*memory_array)[fun];
		}
		std::cerr << "Header: " << (header*)head_pointer << std::endl;
		std::cerr << "Map Location: " << temp << std::endl;
		std::cerr << "FUN: " << fun << std::endl;
		std::cerr << "Map Location Bottom: " << yes << std::endl;
		if(temp->in_use == true){
			std::cerr << "TRUE IN USE" << std::endl;
			while(find_unused_buddy(_a) != NULL && temp->block_size != mem_size){
				
				temp->in_use = false;
				Addr buddy = find_unused_buddy(_a);
				struct header *new_bud = (header*)buddy - 1;
				std::cerr << "Temp 1: " << temp->block_size << std::endl;
				
				combine_blocks(_a,buddy);
				
				
				if(last_size != 0){
					struct header* fixer = (header*)(*memory_array)[last_size];
					if(fixer != NULL){
						if(fixer->next != NULL){
							if(fixer->next->block_size != fixer->block_size){
								fixer->next = NULL;
							}
						}
					}
				}
				
				std::cerr << "Temp: " << temp->block_size << std::endl;
				std::cerr << "New: " << new_bud->block_size << std::endl;
				std::cerr << "Dist: " << (char*)new_bud - (char*)temp << std::endl;
				
				last_size = temp->block_size;
				breakbool = false;
				std::cerr << (header*)_a - 1 << std::endl;
			}
			temp->in_use = false;
			if(breakbool == true){
				std::cerr << "No Buddy Found!" << std::endl;
				total_alloc_mem = total_alloc_mem - orig_size;
				struct header *rover = (header*)head_pointer;
				struct header *structure = (header*)(*memory_array)[temp->block_size];
				bool skipme = false;
				bool skipme2 = false;
				//(*memory_array)[temp->block_size] = temp;
				//std::cerr << temp->next;
				//temp->next = NULL;
				if(structure == NULL){
				std::cerr << "HI" << std::endl;
				(*memory_array)[temp->block_size] = temp;
				//std::cerr << temp->next;
				temp->next = NULL;
				}
				else{
					while((char*)rover <= ((char*)head_pointer + mem_size)){
						
						if(rover != NULL){
							if((rover->block_size == temp->block_size) && (rover->in_use == false)){
								std::cerr << "HELPME" << std::endl;
								if(((char*)rover < (char*)temp)){
									if((*memory_array)[temp->block_size] != rover && skipme2 == false){
										std::cerr << "HElp" << std::endl;
										(*memory_array)[temp->block_size] = rover;
										structure = (header*)(*memory_array)[temp->block_size];
										structure->next = NULL;
										skipme = true;
									}
									else{
										std::cerr << "lol" << std::endl;
										structure->next = rover;
										rover->next = NULL;
										structure = rover;
										std::cerr << rover << std::endl;
										std::cerr << rover->block_size << std::endl;
										std::cerr << structure->next << std::endl;
										skipme = true;
										skipme2 = true;
									}
								}
								else{
									if(skipme == false){
										std::cerr << "HElpp" << std::endl;
										(*memory_array)[temp->block_size] = temp;
										structure = (header*)(*memory_array)[temp->block_size];
										structure->next = NULL;
									}
									else{
										std::cerr << "lolol" << std::endl;
										structure->next = rover;
										rover->next = NULL;
										structure = rover;
										std::cerr << rover << std::endl;
										std::cerr << rover->block_size << std::endl;
										std::cerr << structure->next << std::endl;
										skipme = true;
									}
								}
								
							}
						}
						rover = (header*)((char*)rover + temp->block_size);
						
					}
				}
				
				
				
			}
			else{
				std::cerr << "Block Size: "<< temp->block_size << std::endl;
				total_alloc_mem = total_alloc_mem - orig_size;	
				struct header *rover = (header*)head_pointer;
				struct header *structure = (header*)(*memory_array)[temp->block_size];
				bool skipme = false;
				bool skipme2 = false;
				//(*memory_array)[temp->block_size] = temp;
				//std::cerr << temp->next;
				//temp->next = NULL;
				if(structure == NULL){
				std::cerr << "HI" << std::endl;
				(*memory_array)[temp->block_size] = temp;
				//std::cerr << temp->next;
				temp->next = NULL;
				}
				else{
					while((char*)rover <= ((char*)head_pointer + mem_size)){
						
						if(rover != NULL){
							if((rover->block_size == temp->block_size) && (rover->in_use == false)){
								std::cerr << "HELPME" << std::endl;
								if(((char*)rover < (char*)temp)){
									if((*memory_array)[temp->block_size] != rover && skipme2 == false){
										std::cerr << "HElp" << std::endl;
										(*memory_array)[temp->block_size] = rover;
										structure = (header*)(*memory_array)[temp->block_size];
										structure->next = NULL;
										skipme = true;
									}
									else{
										std::cerr << "lol" << std::endl;
										structure->next = rover;
										rover->next = NULL;
										structure = rover;
										std::cerr << rover << std::endl;
										std::cerr << rover->block_size << std::endl;
										std::cerr << structure->next << std::endl;
										skipme = true;
										skipme2 = true;
									}
								}
								else{
									if(skipme == false){
										std::cerr << "HElpp" << std::endl;
										(*memory_array)[temp->block_size] = temp;
										structure = (header*)(*memory_array)[temp->block_size];
										structure->next = NULL;
									}
									else{
										std::cerr << "lolol" << std::endl;
										structure->next = rover;
										rover->next = NULL;
										structure = rover;
										std::cerr << rover << std::endl;
										std::cerr << rover->block_size << std::endl;
										std::cerr << structure->next << std::endl;
										skipme = true;
									}
								}
								
							}
						}
						rover = (header*)((char*)rover + temp->block_size);
						
					}
				}
				//if(yes->next != NULL){
				//	yes->next = NULL;
				//}
				/*
				while((char*)rover <= (char*)temp){
					
					if(rover != NULL){
						if(rover->block_size == temp->block_size && temp->in_use == false){
							std::cerr << "Hello" << std::endl;
							if((*memory_array)[temp->block_size] != rover){
								(*memory_array)[temp->block_size] = rover;
								structure = (header*)(*memory_array)[temp->block_size];
								structure->next = NULL;
							}
							else{
								structure->next = rover;
								rover->next = NULL;
								structure = rover;
							}
						}
					}
					rover = (header*)((char*)rover + temp->block_size);
					
				}
				*/
				
							
				
			}
			
			
		}
		else{
			printf("This block is already free!\n");
		}
			
	}
	else{
		printf("This block doesnt exist!\n");
		return false;
	}
}

void MyAllocator::print_array()
{
	int total = 0;
	int temp = mem_size;
	struct header *temp_block;
	while(temp >= basic_block_size){
		temp_block = (header*)(*memory_array)[temp];
		printf("Free List For %d:\n",temp);
		if(temp_block == NULL){
			temp = temp / 2;
			continue;
		}
		if((char*)temp_block->next < head_pointer || (char*)temp_block->next > (head_pointer + mem_size)){
				temp_block->next = NULL;
		}
			while(temp_block->next != NULL){
				if(temp_block->in_use == false){
					if(temp_block->block_size != temp){
						temp_block->block_size = temp;
					}
					printf("There is a %d size free block.\n", temp_block->block_size);
					
					total = total + temp_block->block_size;
				}
				else{
					printf("There is a %d size taken block.\n", temp_block->block_size);
				}
			if((char*)temp_block->next->next < head_pointer || (char*)temp_block->next->next > (head_pointer + mem_size)){
				temp_block->next->next = NULL;
			}
			temp_block = temp_block->next;
			}
		if(temp_block->in_use == false){
			if(temp_block->block_size != temp){
				temp_block->block_size = temp;
			}
			printf("There is a %d size free block.\n", temp_block->block_size);
			total = total + temp_block->block_size;
		}
		temp = temp / 2;
	}
	int total_alloc = mem_size - total;
	printf("There is %d free memory in total.\n", total);
	printf("There is %d memory allocated in total.\n", total_alloc);
	printf("There is %d memory allocated in total alloc.\n", total_alloc_mem);
	struct header* header_new = (header*)head_pointer;
	printf("There is %d memory allocated in header.\n", header_new->block_size);
	std::cout << std::endl;
}

/* Getter Functions */
char *MyAllocator::getHeadPointer()
{
	return head_pointer;
}

unsigned int MyAllocator::getBasicBlockSize()
{
	return basic_block_size;
}

unsigned int MyAllocator::getMemSize()
{
	return mem_size;
}

unsigned short int MyAllocator::getNumTiers()
{
	return num_tiers;
}

bool MyAllocator::getInitialized()
{
	return initialized;
}
	
std::map<int, Addr> *MyAllocator::getMemoryArray()
{
	return memory_array;
}
