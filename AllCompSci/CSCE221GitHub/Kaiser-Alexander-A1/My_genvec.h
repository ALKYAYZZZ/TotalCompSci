#include <ostream>
#include <iostream>
using namespace std;

template<typename T>

class My_genvec{
	
	//member variables
	int size, capacity;
	T* ptr;
	


public:	
	//member functions
	My_genvec();
	~My_genvec();
	My_genvec(const My_genvec& vec);
	My_genvec& operator=(const My_genvec& vec);
	int get_size() const;
	int get_capacity() const;
	T& operator[](int i) const;
	T& operator[](int i);
	bool is_empty() const;
	T& elem_at_rank(int r) const;
	void insert_at_rank(int r, T const& elem);
	void replace_at_rank(int r, const T& elem);
	void remove_at_rank(int r);

};

template<typename T>
My_genvec<T>::My_genvec()
	:capacity(0),size(0),ptr(NULL) {ptr = new T[capacity];}//Constructs all variables for My_genvec
	

template<typename T>	
My_genvec<T>::~My_genvec()//Deletes empty vectors to prevent memory leaks
	{if(ptr) delete[] ptr;}
		
template<typename T>
int My_genvec<T>::get_size() const//Returns size for My_genvec
	{ return size; }

template<typename T>
int My_genvec<T>::get_capacity() const//Returns capacity for My_genvec
	{ return capacity; }
	
template<typename T>
T& My_genvec<T>::operator[](int i)//Overloads [] operator to return element at location in vector
	{ return ptr[i]; }
	
template<typename T>
T& My_genvec<T>::operator[](int i) const//Overloads [] operator to return element at location in vector
	{ return ptr[i]; }
	
template<typename T>
bool My_genvec<T>::is_empty() const//Returns if the vector is empty or not
	{if (size == 0)
		return true;
	}
	
template<typename T>
My_genvec<T>& My_genvec<T>::operator=(const My_genvec& vec){//Overloads assignment operator to Set all elements in one vector equal to another equal in size
	for (int i=0; i <= capacity; i++)
		vec[i] = ptr[i];
	return *this;
}

template<typename T>
T& My_genvec<T>::elem_at_rank(int r) const//Returns the element at the specific location in a vector
	{ return ptr[r]; }
	
template<typename T>
void My_genvec<T>::insert_at_rank(int r, T const& elem){//Inserts element at location in vector
	if (r > size && size != 0)
		std::cout << "You cannot place a figure in that high of index!\n";//Throws exception of location is larger than vector size
	else{
	if (r >= size)
		capacity = max(r,capacity * 2);//Allocates more memory if capacity is not large enough
	if (size >= capacity)
		capacity = max(1,capacity * 2);
	for (int i = size - 1; i >= r ;i--)//Moves all existing elements above the inserted element up one location
		ptr[i+1]=ptr[i];
	ptr[r] = elem;//Sets element equal to the location in the vector
	if (ptr[r] != T())
		size++;	//Increases size by one if location is not empty
	}
}

template<typename T>
void My_genvec<T>::remove_at_rank(int r){//Removes an element at a location and moves all existing elements down one location
		for (int i = size-1; i >= r; i--)//Moves all elements down one location
		ptr[i-1]=ptr[i];
	if (r < size)
	size--;//Decreases size by one if location is not empty
	
}

template<typename T>
void My_genvec<T>::replace_at_rank(int r, T const& elem){//Replaces an element at a specific location with another element
	if (r >= size && size != 0)//Throws an exception if location does not exist
		std::cout << "You cannot place a figure in that high of index!\n";
	else{
	ptr[r] = elem;//Sets element equal to the location
	}
	
}

template<typename T>
std::ostream& operator<<(ostream& out, const My_genvec<T>& vec){//Overloads << operator to display all elements in a given vector
	for (int i = 0; i <= vec.get_size() - 1; i++)
		out << vec[i] << " ";
	return out;
}

template<typename T>
int find_max_index(My_genvec<T>& v, int size){//Finds index of the biggest element in the vector
	int location = 0;
	int tally = 0;
	T letter = v[0];
	
	for (int i = v.get_size()- size; i <= v.get_size() - 1; i++){//Goes throughout capacity of vector until final index is reached.
			if (v[i] > letter){//If next number is higher than the last, that number is stored and the location is moved up by one.
				letter = v[i];
				location = i;
				
			}	

	}	
	//cout << v[location] << " d";
	return location;
}

template<typename T>
void sort_max(My_genvec<T>& vec){//Finds index of the biggest element in the vector
		int i = 0;
	for (int i = 0; i  <= vec.get_size() - 1; i++){
		int max = find_max_index(vec, vec.get_size()-i);//Uses find_max_index to find largest 
		T k = vec[max];//Stores element at max index in k
		vec.replace_at_rank(max,vec[i]);//Swaps element at first position to the max element position
		vec.replace_at_rank(i,k);//Swaps element at max element position to the first position
	}
}
