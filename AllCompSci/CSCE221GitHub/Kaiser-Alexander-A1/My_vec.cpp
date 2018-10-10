#include "My_vec.h"
#include <utility>
#include <iostream>
My_vec::My_vec()
	:capacity(0),size(0),ptr(NULL) {ptr = new char[capacity];}//Constructs all variables for My_vec
	
My_vec::~My_vec()//Deletes empty vectors to prevent memory leaks
	{if(ptr)
		delete[] ptr;
		}
int My_vec::get_size() const//Returns size for My_vec
	{ return size; }

int My_vec::get_capacity() const//Returns capacity for My_vec
	{ return capacity; }
	
char& My_vec::operator[](int i)//Overloads [] operator to return element at location in vector
	{ return ptr[i]; }
	
char& My_vec::operator[](int i) const//Overloads [] operator to return element at location in vector
	{ return ptr[i]; }
	
bool My_vec::is_empty() const //Returns if the vector is empty or not
	{if (size == 0)
		return true;
	}
My_vec& My_vec::operator=(const My_vec& vec){//Overloads assignment operator to Set all elements in one vector equal to another equal in size
	for (int i=0; i <= capacity; i++)
		vec[i] = ptr[i];
	return *this;
}

char& My_vec::elem_at_rank(int r) const//Returns the element at the specific location in a vector
	{ return ptr[r]; }
	
void My_vec::insert_at_rank(int r, const char& elem){//Inserts element at location in vector
	if (r > size)
		std::cout << "You cannot place a figure in that high of index!\n";//Throws exception of location is larger than vector size
	else{
	if (r >= size)
		capacity = max(r,capacity * 2);
	if (size >= capacity)
		capacity = max(1,capacity * 2);//Allocates more memory if capacity is not large enough
	for (int i = size - 1; i >= r ;i--)//Moves all existing elements above the inserted element up one location
		ptr[i+1]=ptr[i];
	ptr[r] = elem;//Sets element equal to the location in the vector
	if (ptr[r] != '\0')
		size++;	//Increases size by one if location is not empty
	}
}

void My_vec::remove_at_rank(int r){//Removes an element at a location and moves all existing elements down one location
	for (int i = size-1; i >= r; i--)//Moves all elements down one location
		ptr[i-1]=ptr[i];
	if (ptr[r] != '\0')
	size--;//Decreases size by one if location is not empty
}

void My_vec::replace_at_rank(int r, const char& elem){//Replaces an element at a specific location with another element
	if (ptr[r] == '\0')//Throws an exception if location does not exist
		std::cout << "You cannot place a figure in that high of index!\n";
	else{
	if (ptr[r] != '\0' && elem == '\0')//Checks to see if space is empty
		size--;
	if (ptr[r] == '\0' && elem != '\0'){
		size++;
	}
	ptr[r] = elem;//Sets element equal to the location
	}
}

std::ostream& operator<<(ostream& out, const My_vec& vec){//Overloads << operator to display all elements in a given vector
	for (int i = 0; i <= vec.get_capacity(); i++)
		out << vec[i];
	return out;
}

int find_max_index(My_vec& v, int size){//Finds index of the biggest element in the vector
	int location = 0;
	int tally = 0;
	int letter = 1;
	
	for (int i = v.get_size()- size; i <= v.get_capacity(); i++){//Goes throughout capacity of vector until final index is reached.
			if (v[i] > letter){//If next letter is higher than the last, that letter is stored and the location is moved up by one.
				letter = int(v[i]);
				location = i;
				
			}	

	}	
	return location;
}

void sort_max(My_vec& vec){//Sorts the elements in a decending order
		int i = 0;
	for (int i = 0; i  <= vec.get_size() - 1; i++){
		int max = find_max_index(vec, vec.get_size()-i);//Uses find_max_index to find largest 
		char k = vec[max];//Stores element at max index in k
		vec.replace_at_rank(max,vec[i]);//Swaps element at first position to the max element position
		vec.replace_at_rank(i,k);//Swaps element at max element position to the first position
	}
}