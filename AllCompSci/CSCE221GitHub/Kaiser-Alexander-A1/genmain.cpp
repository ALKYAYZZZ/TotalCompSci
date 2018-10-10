
// genmain.cpp
//Tests all functionality of the My_genvec class.

#include <iostream>
#include <stdexcept>
#include "My_genvec.h"

class T{
	
};

int main(){
try{
    My_genvec<int>* intVector = new My_genvec<int>;// define an object intVector of My_genvec type
    intVector->insert_at_rank(0,1313);// insert 1 at the rank 0 into the vector intVector
	cout << "The elements of the vector are " << *intVector << "\n";// use the overloaded operator << to display vector elements
    cout << "The size of the vector is " << intVector->get_size() << "\n";// display the vector intVector size

	intVector->insert_at_rank(0,2);// insert 2 at the rank 0 into the vector intVector
	cout << "The elements of the vector are " << *intVector << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << intVector->get_size() << "\n";// display the vector intVector size

	intVector->insert_at_rank(10,3);// insert 3 at the rank 10 into the vector intVector
	cout << "The elements of the vector are " << *intVector << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << intVector->get_size() << "\n";// display the vector intVector size

	intVector->remove_at_rank(2);// remove a character at the rank 2 from the vector intVector
	cout << "The elements of the vector are " << *intVector << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << intVector->get_size() << "\n";// display the vector intVector size

	intVector->replace_at_rank(2, 4);// replace a character at the rank 2 by the character 4 
	cout << "The elements of the vector are " << *intVector << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << intVector->get_size() << "\n";// display the vector intVector size
	
	My_genvec<int>* intVector1 = intVector;// create a copy intVector1 of the vector intVector using a copy constructor
	cout << "The elements of the vector 1 are " << *intVector1 << "\n";// use the overloaded operator << to display the vector intVector1
    intVector1->replace_at_rank(2,5);// replace a character at the rank 2 of the vector intVector1 with the number 5
	cout << "The elements of the vector 1 are " << *intVector1 << "\n";// use the overloaded operator << to display vector intVector1 elements

	My_genvec<int>* intVector2 = new My_genvec<int>; //define an object intVector2 of My_intVectorec type
	intVector2->insert_at_rank(0, 6);// insert 6 at the rank 0 into the vector intVector2
	cout << "The elements of the vector 2 are " << *intVector2 << "\n";// use the overloaded operator << to display vector elements
    cout << "The size of the vector 2 is " << intVector2->get_size() << "\n";// display the vector intVector2 size

    intVector2 = intVector1;// test the assignment operator and copy the vector intVector1 to intVector2 
    cout << "The elements of the vector 2 are " << *intVector2 << "\n";// use the overloaded operator << to display vector intVector2
    cout << "The size of the vector 2 is " << intVector2->get_size() << "\n";// display the vector intVector2 size

    cout << "The highest index of vector 2 is index " << find_max_index(*intVector2, intVector2->get_size()) << "\n";// test the function find_max_index using intVector2
	sort_max(*intVector2);
	cout << "The sorted max index of vector 2 is " << *intVector2 << "\n";// test the function sort_max using intVector2

	intVector2->replace_at_rank(14, 12);// replace in the vector intVector2 a character at the rank 14 with 12
	
	
	
	
	cout << "---------------------------------------------------------------------\n";
	
	
	
	
	My_genvec<double>* intDouble = new My_genvec<double>;// define an object intDouble of My_intVectorec type
	//My_genintVectorec<int> = intDouble;
    intDouble->insert_at_rank(0,280.69);// insert 280.69 at the rank 0 into the vector intDouble
	cout << "The elements of the vector are " << *intDouble << "\n";// use the overloaded operator << to display vector elements
    cout << "The size of the vector is " << intDouble->get_size() << "\n";// display the vector intDouble size

	intDouble->insert_at_rank(0,-12.78965);// insert -12.78965 at the rank 0 into the vector intDouble
	cout << "The elements of the vector are " << *intDouble << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << intDouble->get_size() << "\n";// display the vector intDouble size

	intDouble->insert_at_rank(10,4876.07564);// insert 4876.07564 at the rank 10 into the vector intDouble
	cout << "The elements of the vector are " << *intDouble << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << intDouble->get_size() << "\n";// display the vector intDouble size

	intDouble->remove_at_rank(2);// remove a character at the rank 2 from the vector intDouble
	cout << "The elements of the vector are " << *intDouble << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << intDouble->get_size() << "\n";// display the vector intDouble size

	intDouble->replace_at_rank(2, .8966);// replace a character at the rank 2 by the double .8966 
	cout << "The elements of the vector are " << *intDouble << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << intDouble->get_size() << "\n";// display the vector intDouble size
	
	My_genvec<double>* intDouble1 = intDouble;// create a copy intDouble1 of the vector intDouble using a copy constructor
	cout << "The elements of the vector 1 are " << *intDouble1 << "\n";// use the overloaded operator << to display the vector intDouble1
    intDouble1->replace_at_rank(2,9073.0);// replace a character at the rank 2 of the vector intDouble1 with the character double 9073.0
	cout << "The elements of the vector 1 are " << *intDouble1 << "\n";// use the overloaded operator << to display vector intDouble1 elements

	My_genvec<double>* intDouble2 = new My_genvec<double>; //define an object intDouble2 of My_intDoubleec type
	intDouble2->insert_at_rank(0, 89.6);// insert 89.6 at the rank 0 into the vector intDouble2
	cout << "The elements of the vector 2 are " << *intDouble2 << "\n";// use the overloaded operator << to display vector elements
    cout << "The size of the vector 2 is " << intDouble2->get_size() << "\n";// display the vector intDouble2 size

    intDouble2 = intDouble1;// test the assignment operator and copy the vector intDouble1 to intDouble2 
    cout << "The elements of the vector 2 are " << *intDouble2 << "\n";// use the overloaded operator << to display vector intDouble2
    cout << "The size of the vector 2 is " << intVector2->get_size() << "\n";// display the vector intVector2 size

    cout << "The highest index of vector 2 is index " << find_max_index(*intDouble2, intDouble2->get_size()) << "\n";// test the function find_max_index using intDouble2
	sort_max(*intDouble2);
	cout << "The sorted max index of vector 2 is " << *intDouble2 << "\n";// test the function sort_max using intDouble2

	intDouble2->replace_at_rank(14, 8728.9);// replace in the vector intDouble2 a double at the rank 14 with 8728.9
	
	cout << "----------------------------------------------------------------------\n";

	
	My_genvec<short>* shortVector = new My_genvec<short>;// define an object shortVector of My_shortVectorec type
	//My_genshortVectorec<int> = shortVector;
    shortVector->insert_at_rank(0, -7);// insert -7 at the rank 0 into the vector shortVector
	cout << "The elements of the vector are " << *shortVector << "\n";// use the overloaded operator << to display vector elements
    cout << "The size of the vector is " << shortVector->get_size() << "\n";// display the vector shortVector size
	shortVector->insert_at_rank(0,5);// insert 5 at the rank 0 into the vector shortVector
	cout << "The elements of the vector are " << *shortVector << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << shortVector->get_size() << "\n";// display the vector shortVector size

	shortVector->insert_at_rank(10,12);// insert 12 at the rank 10 into the vector shortVector
	cout << "The elements of the vector are " << *shortVector << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << shortVector->get_size() << "\n";// display the vector shortVector size

	shortVector->remove_at_rank(2);// remove a character at the rank 2 from the vector shortVector
	cout << "The elements of the vector are " << *shortVector << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << shortVector->get_size() << "\n";// display the vector shortVector size

	shortVector->replace_at_rank(2, -9);// replace a character at the rank 2 by the short -9 
	cout << "The elements of the vector are " << *shortVector << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << shortVector->get_size() << "\n";// display the vector shortVector size
	
	My_genvec<short>* shortVector1 = shortVector;// create a copy shortVector1 of the vector shortVector using a copy constructor
	cout << "The elements of the vector 1 are " << *shortVector1 << "\n";// use the overloaded operator << to display the vector shortVector1
    shortVector1->replace_at_rank(2,-56);// replace a character at the rank 2 of the vector shortVector1 with the short -56
	cout << "The elements of the vector 1 are " << *shortVector1 << "\n";// use the overloaded operator << to display vector shortVector1 elements

	My_genvec<short>* shortVector2 = new My_genvec<short>; //define an object shortVector2 of My_shortVectorec type
	shortVector2->insert_at_rank(0, 8);// insert 8 at the rank 0 into the vector shortVector2
	cout << "The elements of the vector 2 are " << *shortVector2 << "\n";// use the overloaded operator << to display vector elements
    cout << "The size of the vector 2 is " << shortVector2->get_size() << "\n";// display the vector shortVector2 size

    shortVector2 = shortVector1;// test the assignment operator and copy the vector shortVector1 to shortVector2 
    cout << "The elements of the vector 2 are " << *shortVector2 << "\n";// use the overloaded operator << to display vector shortVector2
    cout << "The size of the vector 2 is " << shortVector2->get_size() << "\n";// display the vector shortVector2 size

    cout << "The highest index of vector 2 is index " << find_max_index(*shortVector2, shortVector2->get_size()) << "\n";// test the function find_max_index using shortVector2
	sort_max(*shortVector2);
	cout << "The sorted max index of vector 2 is " << *shortVector2 << "\n";// test the function sort_max using shortVector2
	
	shortVector2->replace_at_rank(14, 0);// replace in the vector shortVector2 a character at the rank 14 with 0
	
	
	
	
	cout << "----------------------------------------------------------------------\n";
	
	
	
	
	
	My_genvec<char const*>* stringVector = new My_genvec<char const*>;// define an object stringVector of My_genvec type
    stringVector->insert_at_rank(0, "hello");// insert "hello" at the rank 0 into the vector stringVector
	cout << "The elements of the vector are " << *stringVector << "\n";// use the overloaded operator << to display vector elements
    cout << "The size of the vector is " << stringVector->get_size() << "\n";// display the vector stringVector size

	stringVector->insert_at_rank(0, "no");// insert "no" at the rank 0 into the vector stringVector
	cout << "The elements of the vector are " << *stringVector << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << stringVector->get_size() << "\n";// display the vector stringVector size

	stringVector->insert_at_rank(10,"greater");// insert "greater" at the rank 10 into the vector stringVector
	cout << "The elements of the vector are " << *stringVector << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << stringVector->get_size() << "\n";// display the vector stringVector size

	stringVector->remove_at_rank(2);// remove a character at the rank 2 from the vector stringVector
	cout << "The elements of the vector are " << *stringVector << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << stringVector->get_size() << "\n";// display the vector stringVector size

	stringVector->replace_at_rank(2, "Yes");// replace a character at the rank 2 by the character string "Yes" 
	cout << "The elements of the vector are " << *stringVector << "\n";// use the overloaded operator << to display vector elements
	cout << "The size of the vector is " << stringVector->get_size() << "\n";// display the vector stringVector size
	
	My_genvec<char const*>* stringVector1 = stringVector;// create a copy stringVector1 of the vector stringVector using a copy constructor
	cout << "The elements of the vector 1 are " << *stringVector1 << "\n";// use the overloaded operator << to display the vector stringVector1
    stringVector1->replace_at_rank(2,"yes");// replace a character at the rank 2 of the vector stringVector1 with the string "yes"
	cout << "The elements of the vector 1 are " << *stringVector1 << "\n";// use the overloaded operator << to display vector stringVector1 elements

	My_genvec<char const*>* stringVector2 = new My_genvec<char const*>; //define an object stringVector2 of My_stringVectorec type
	stringVector2->insert_at_rank(0, "hi");// insert "hi" at the rank 0 into the vector stringVector2
	cout << "The elements of the vector 2 are " << *stringVector2 << "\n";// use the overloaded operator << to display vector elements
    cout << "The size of the vector 2 is " << stringVector2->get_size() << "\n";// display the vector stringVector2 size

    stringVector2 = stringVector1;// test the assignment operator and copy the vector stringVector1 to stringVector2 
    cout << "The elements of the vector 2 are " << *stringVector2 << "\n";// use the overloaded operator << to display vector stringVector2
    cout << "The size of the vector 2 is " << stringVector2->get_size() << "\n";// display the vector stringVector2 size

    cout << "The highest index of vector 2 is index " << find_max_index(*stringVector2, stringVector2->get_size()) << "\n";// test the function find_max_index using stringVector2
	sort_max(*stringVector2);
	cout << "The sorted max index of vector 2 is " << *stringVector2 << "\n";// test the function sort_max using stringVector2

	stringVector2->replace_at_rank(14, "me");// replace in the vector stringVector2 a character at the rank 14 with "me"
	
}

catch(exception &error){
	cerr << "Error: " << error.what() << endl;
}
}
