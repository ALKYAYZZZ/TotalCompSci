
// main.cpp
//Tests all functionality of the My_vec class.

#include <iostream>
#include <stdexcept>
#include "My_vec.h"

int main(){
try{
    My_vec* v = new My_vec;// define an object v of My_vec type
    v->insert_at_rank(0,'B');// insert 'B' at the rank 0 into the vector v
	cout << "The elements of the vector are " << *v << "\n";// use the overloaded operator << to display vector elements
    cout << "The size of the vector is " << v->get_size() << "\n";// display the vector v size

	v->insert_at_rank(0,'A');// insert 'A' at the rank 0 into the vector v
	cout << "The elements of the vector are " << *v << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << v->get_size() << "\n";// display the vector v size

	v->insert_at_rank(10,'D');// insert 'D' at the rank 10 into the vector v
	cout << "The elements of the vector are " << *v << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << v->get_size() << "\n";// display the vector v size

	v->remove_at_rank(2);// remove a character at the rank 2 from the vector v
	cout << "The elements of the vector are " << *v << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << v->get_size() << "\n";// display the vector v size

	v->replace_at_rank(2, 'E');// replace a character at the rank 2 by the character 'E' 
	cout << "The elements of the vector are " << *v << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << v->get_size() << "\n";// display the vector v size
	
	My_vec* v1 = v;// create a copy v1 of the vector v using a copy constructor
	cout << "The elements of the vector 1 are " << *v1 << "\n";// use the overloaded operator << to display the vector v1
    v1->replace_at_rank(2, 'Y');// replace a character at the rank 2 of the vector v1 with the character 'Y'
	cout << "The elements of the vector 1 are " << *v1 << "\n";// use the overloaded operator << to display vector v1 elements

	My_vec* v2 = new My_vec;// define an object v2 of My_vec type
	v2->insert_at_rank(0, 'K');// insert 'K' at the rank 0 into the vector v2
	cout << "The elements of the vector 2 are " << *v2 << "\n";// use the overloaded operator << to display vector elements
    cout << "The size of the vector 2 is " << v2->get_size() << "\n";// display the vector v2 size

    v2 = v1;// test the assignment operator and copy the vector v1 to v2 
    cout << "The elements of the vector 2 are " << *v2 << "\n";// use the overloaded operator << to display vector v2
    cout << "The size of the vector 2 is " << v2->get_size() << "\n";// display the vector v2 size

    cout << "The highest index of vector 2 is index " << find_max_index(*v2, v2->get_size()) << "\n";// test the function find_max_index using v2
	sort_max(*v2);
	cout << "The sorted max index of vector 2 is " << *v2 << "\n";// test the function sort_max using v2

	v2->replace_at_rank(14,'S');// replace in the vector v2 a character at the rank 14 with 'S'

}

catch(out_of_range &error){
	cerr << "Error: " << error.what() << endl;
}

}
