#include <cassert>
#include <cstring>
#include <sstream>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>

#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

//cout << "Please enter your command!/n";

struct course{//struct to hold all course info
	string  dept_code;
	string  course_number;
	string  course_name;
	string  section_number;
};

struct book{//struct to hold all book info
	string  isbn;
	string  book_name;
	string  author_name;
	course  book_course;
	string  date;
	string  edition;
	string  price;
	string  condition;
	string  req_or_opt;
};

vector<book> books;//global book vector
vector<course> courses;//global course vector

void book_init(book &book_input){//initalize book with empty values
	book_input.isbn = " ";
	book_input.book_name = " ";
	book_input.author_name = " ";
	book_input.book_course.dept_code = " ";
	book_input.book_course.course_number = " ";
	book_input.book_course.course_name = " ";
	book_input.book_course.section_number = " ";
	book_input.date = " ";
	book_input.edition = " ";
	book_input.price = " ";
	book_input.condition = " ";
	book_input.req_or_opt = " ";
}

void course_init(course &courses){//initialize courses with empty values
	courses.dept_code = " ";
	courses.course_number = " ";
	courses.course_name = " ";
	courses.section_number = " ";
}

void check_for_others(book &book_input){
	if(books.size() > 0){
			for(int i = 0; books.size() > i; i++){//go through books to see if a book with the given isbn already exists
				if(strcmp(books[i].isbn.c_str(),book_input.isbn.c_str()) == 0){
					book_input.book_name = books[i].book_name;
					book_input.author_name = books[i].author_name;
					//book_input.book_course = books[i].book_course;
				}
			}
		}
}

void check_for_courses(course &course_input){
	if(courses.size() > 0){
			for(int i = 0; courses.size() > i; i++){//go through books to see if a book with the given isbn already exists
				if(strcmp(courses[i].dept_code.c_str(),course_input.dept_code.c_str()) == 0){
					if(strcmp(courses[i].course_number.c_str(),course_input.course_number.c_str()) == 0){
					course_input.course_name = courses[i].course_name;
				}
			}
		}
	}
}
void restart(int &breaker){//function to restart program
	cout << "Would you like to continue to use the program? (y for yes, n for no)\n";//Ask user if they would like to continue the program.
	char decision;
	cin >> decision;
	if (decision == 'Y' || decision == 'y'){
		
	}
	else{
		breaker = 1;//Break out of while loop
	}
}

void d_command(vector<vector<std::string>> &cumuliative){
	int breaker = -1;
	bool return_state = false;
	
	
	if(books.size() > 0){//go through book vector to see if there is already a book with the given isbn
		for(int i = 0; books.size() > i; i++){
			if(strcmp(books[i].isbn.c_str(),cumuliative[0][1].c_str()) == 0){
				breaker = i;
				break;
			}
		}
	}
	while(return_state == false){
	begin:
		if(cumuliative[cumuliative.size() -1].size() >= 4){//check to see how many strings were entered in the command line
				book * temp;
				if(breaker == -1){//create new book if necessary
					temp = new book();
					book_init(*temp);
				}
				else{//reuse book if necessary
					temp = &books[breaker];
				}
					int placeholder = cumuliative.size() - 1;

					temp->isbn = cumuliative[placeholder][1];//set isbn
					if(cumuliative[placeholder][2] == "A" ||  cumuliative[placeholder][2] == "a"){
						temp->author_name = cumuliative[placeholder][3];
							if(cumuliative[cumuliative.size() -1].size() >= 4){//Check to see if author name is more than one word and concatenate if it is
								int traverse = 4;
								while(traverse <= cumuliative[cumuliative.size() -1].size() - 1){//concatenate author name
									temp->author_name = temp->author_name + " " + cumuliative[cumuliative.size() -1][traverse];
									traverse++;
								}
							}
					}
					else if(cumuliative[placeholder][2] == "E" ||  cumuliative[placeholder][2] == "e"){
						int integer;
						stringstream(cumuliative[placeholder][3]) >> integer;//cin to int to check to see if int is greater than 0
						if(integer > 0){
							temp->edition = cumuliative[placeholder][3];
						}
						else{
							cerr << "Edition is not positive.\n";
						}
					}
					else if(cumuliative[placeholder][2] == "D" ||  cumuliative[placeholder][2] == "d"){
						temp->date = cumuliative[placeholder][3];//change date of publication
					}
					else{
						cerr << "Command not recognized.\n";//error failsafe
					}
					
				if(breaker == -1){
					books.push_back(*temp);
					breaker = 1;
				}
				
			if(books.size() > 0 && return_state == false){
				for(int i = breaker; books.size() > i; i++){//go through books to see if a book with the given isbn already exists
					if(strncmp(books[i].isbn.c_str(),cumuliative[0][1].c_str(),16) == 0){
						if(cumuliative[placeholder][2] == "D" ||  cumuliative[placeholder][2] == "d"){
							if(strcmp(books[i].date.c_str()," ") == 0){
								breaker = i;
								goto begin;
							}
						}
						else if(cumuliative[placeholder][2] == "E" ||  cumuliative[placeholder][2] == "e"){
							if(strcmp(books[i].edition.c_str()," ") == 0){
								breaker = i;
								goto begin;
							}
						}
						else{
							if(strcmp(books[i].author_name.c_str()," ") == 0){
								breaker = i;
								goto begin;
							}
						}
						
					}
				}

			return_state = true;	
			}
				
			}
			else{
				cerr << "Not Enough Arguments!\n";
			}
	}
}

void m_command(vector<vector<std::string>> &cumuliative){
	int breaker = -1;
	bool return_state = false;
	
	if(books.size() > 0){
		for(int i = 0; books.size() > i; i++){//go through book vector to see if there is already a book with the given isbn
			if(strcmp(books[i].isbn.c_str(),cumuliative[0][1].c_str()) == 0){
				if(strcmp(books[i].price.c_str()," ") == 0){
					breaker = i;
					break;
				}
			}
		}
	}
	
	while(return_state == false){
	begin:
		if(cumuliative[cumuliative.size() -1].size() >= 4){//check to see if there are at least 4 strings input in the command line
			book * temp;
			if(breaker == -1){//create new book if necessary
				temp = new book();
				book_init(*temp);
			}
			else{
				temp = &books[breaker];
			}
				int placeholder = cumuliative.size() - 1;

				temp->isbn = cumuliative[placeholder][1];//set isbn
				temp->price = cumuliative[placeholder][2];//set price
				if(cumuliative[placeholder][3] == "N" || cumuliative[placeholder][3] == "U" || cumuliative[placeholder][3] == "R" || cumuliative[placeholder][3] == "E")
					temp->condition = cumuliative[placeholder][3];//set book type
				else{
					cerr << "That is not a valid condition!\n";//check to see if book type is valid.
				}
			check_for_others(*temp);
				
			if(breaker == -1){
				books.push_back(*temp);//add book onto book vector if it doesnt already exist
				breaker = 1;
			}
			if(books.size() > 0 && return_state == false){
				for(int i = breaker; books.size() > i; i++){//go through books to see if a book with the given isbn already exists
					if(strncmp(books[i].isbn.c_str(),cumuliative[0][1].c_str(),16) == 0){
						if(strcmp(books[i].price.c_str()," ") == 0){
							breaker = i;
							goto begin;
						}
					}

				}
			return_state = true;	
			}
		}
		else{
			cerr << "Not Enough Arguments!\n";
		}
	}
}

void c_command(vector<vector<std::string>> &cumuliative){
	int breaker_1 = -1;
	int breaker_2 = -1;
	int breaker_3 = -1;
	bool return_state = false;
	
	if(courses.size() > 0){
		for(int i = 0; courses.size() > i; i++){//go through all courses to see if there is already a course with the same code
			if(strcmp(courses[i].dept_code.c_str(),cumuliative[0][1].c_str()) == 0){
				for(int j = 0; courses.size() > j; j++){//go through all course numbers to see if it already exists
					if(strcmp(courses[i].course_number.c_str(),cumuliative[0][2].c_str()) == 0){
						breaker_1 = i;
						break;
					}
				}
			}
		}
	}

	while(return_state == false){
		if(cumuliative[cumuliative.size() -1].size() >= 4){//check to see if there are more than 4 strings input in the command line
			course * temp;
			if(breaker_1 == -1 && breaker_2 == -1){//create new course if one doesn't already exist
				temp = new course();
				course_init(*temp);
			}
			else{//use existing course if it already exists
				temp = &courses[breaker_1];
			}
				int placeholder = cumuliative.size() - 1;

				temp->dept_code = cumuliative[placeholder][1];//set department code
				temp->course_number = cumuliative[placeholder][2];//set course number
				temp->course_name = cumuliative[placeholder][3];//set course name
				if(cumuliative[cumuliative.size() -1].size() >= 4){//Check to see if course title is more than one word and concatenate if it is
					int traverse = 4;
					while(traverse <= cumuliative[cumuliative.size() -1].size() - 1){
						temp->course_name = temp->course_name + " " + cumuliative[cumuliative.size() -1][traverse];
						traverse++;
					}
				}
			check_for_courses(*temp);
			if(breaker_1 == -1 && breaker_2 == -1){//push course onto vector if it doesn't already exist.
				courses.push_back(*temp);
				breaker_1 = 1;
			}
			
		if(courses.size() > 0 && return_state == false){
			for(int i = 0; courses.size() > i; i++){//go through all courses to see if there is already a course with the same code
				if(strcmp(courses[i].dept_code.c_str(),cumuliative[0][1].c_str()) == 0){
					for(int j = 0; courses.size() > j; j++){//go through all course numbers to see if it already exists
						if(strcmp(courses[i].course_number.c_str(),cumuliative[0][2].c_str()) == 0){
							breaker_1 = i;
							break;
						}
					}
				}
			}
		return_state = true;
		}
		}
		else{
			cerr << "Not Enough Arguments!\n";
		}
	}
}

void a_command(vector<vector<std::string>> &cumuliative){
	int breaker = -1;
	int breaker_course = -1;
	bool return_state = false;
	bool return_course = false;
	
	
		if(books.size() > 0){
			for(int i = 0; books.size() > i; i++){//go through books to see if a book with the given isbn already exists
				if(strncmp(books[i].isbn.c_str(),cumuliative[0][1].c_str(),16) == 0){
					if(strcmp(books[i].book_course.section_number.c_str()," ") == 0){
						breaker = i;
						break;
					}
					else{
						if(strcmp(books[i].book_course.dept_code.c_str(),cumuliative[0][2].c_str()) == 0){
							if(strcmp(books[i].book_course.course_number.c_str(),cumuliative[0][3].c_str()) == 0){
								if(strcmp(books[i].book_course.section_number.c_str(),cumuliative[0][4].c_str()) == 0){
									breaker = i;
									break;
								}
							}
						}
					}
				}
			}
		}
	while(return_state == false && return_course == false){
		if(cumuliative[cumuliative.size() -1].size() >= 6){
				book * temp;
				if(breaker == -1){//create new book if necessary
					temp = new book();
					book_init(*temp);
				}
				else{//use existing book if it already exists
					temp = &books[breaker];
				}
				
					int placeholder = 0;

					temp->isbn = cumuliative[placeholder][1];//set isbn
					
					course * temp_course;
					if(courses.size() > 0){
						for(int i = 0; courses.size() > i; i++){//search through all courses to see if course already exists
							if(strcmp(courses[i].dept_code.c_str(), cumuliative[0][2].c_str()) == 0){
								for(int j = i; courses.size() > j; j++){
									if(strcmp(courses[j].course_number.c_str(),cumuliative[0][3].c_str()) == 0){
										//for(int k = j; courses.size() > k; k++){
											//if(strcmp(courses[j].section_number.c_str(),cumuliative[0][4].c_str()) == 0){
												breaker_course = i;
												break;
											//}
										//}
									}
								}
							}
						}
					}
					
					if(breaker_course == -1){//create new course if necessary
						temp_course = new course();
						course_init(*temp_course);
					}
					else{//use existing course if it already exists
						temp_course = &courses[breaker_course];
					}
					
					temp_course->dept_code = cumuliative[placeholder][2];//set dept code
					temp_course->course_number = cumuliative[placeholder][3];//set course number
					temp_course->section_number = cumuliative[placeholder][4];//set section number
					temp->book_course = *temp_course;
					
					if(cumuliative[placeholder][5] == "O" || cumuliative[placeholder][5] == "R")
						temp->req_or_opt = cumuliative[placeholder][5];//set optional or required
					else{
						cerr << "That is not a valid condition!\n";//check conditions for req or opt
					}
					
					check_for_others(*temp);
					
				if(breaker_course == -1){//push course onto course vector if it doesn't already exist
					courses.push_back(*temp_course);
					breaker_course = 1;
				}
					
				if(breaker == -1){//push book onto book vector if it doesn't already exist
					books.push_back(*temp);
					breaker = 1;
				}
				
			if(courses.size() > 0 && return_course == false){
				for(int i = breaker_course; courses.size() > i; i++){//search through all courses to see if course already exists
					if(strcmp(courses[i].dept_code.c_str(), cumuliative[0][2].c_str()) == 0){
						for(int j = i; courses.size() > j; j++){
							if(strcmp(courses[j].course_number.c_str(),cumuliative[0][3].c_str()) == 0){
								//for(int k = 0; courses.size() > k; k++){
									//if(strcmp(courses[j].section_number.c_str(),cumuliative[0][4].c_str()) == 0){
										breaker_course = i;
										break;
									//	}
									//}
								}
							}
						}
					}
				return_course = true;
				}
				
				
			if(books.size() > 0 && return_state == false){
				for(int i = breaker; books.size() > i; i++){//go through books to see if a book with the given isbn already exists
					if(strncmp(books[i].isbn.c_str(),cumuliative[0][1].c_str(),16) == 0){
						breaker = i;
						break;
						}
					}
			return_state = true;
			}
		}
				
				
			
			else{
				cerr << "Not Enough Arguments!\n";
			}
	}
}

void b_command(vector<vector<std::string>> &cumuliative){
	int breaker = -1;
	if(books.size() > 0){
		for(int i = 0; books.size() > i; i++){//go through book vector to see if given isbn already exists
			if(strcmp(books[i].isbn.c_str(),cumuliative[0][1].c_str()) == 0){
				breaker = i;
				break;
			}
		}
	}
	

		if(cumuliative[cumuliative.size() -1].size() >= 3){//check to see if there are at least 3 strings in the command line input
			book * temp;
			if(breaker == -1){//create new book if necessary
				temp = new book();
				book_init(*temp);
			}
			else{//reuse existing book if it already exists
				temp = &books[breaker];
			}
				int placeholder = cumuliative.size() - 1;

				temp->isbn = cumuliative[placeholder][1];//set isbn
				temp->book_name = cumuliative[placeholder][2];//set book name
				
				if(cumuliative[cumuliative.size() -1].size() >= 3){//Check to see if book title is more than one word and concatenate if it is
					int traverse = 3;
					while(traverse <= cumuliative[cumuliative.size() -1].size() - 1){//concatenate book name
						temp->book_name = temp->book_name + " " + cumuliative[cumuliative.size() -1][traverse];
						traverse++;
					}
				}
				
			check_for_others(*temp);
			
			if(breaker == -1){//if book doesnt already exist, push it onto the book vector
				books.push_back(*temp);
			}
			
			
		}
		else{
			cerr << "Not Enough Arguments!\n";
		}
}

void gc_command(vector<vector<std::string>> &cumuliative){
	course * temp_course;
	if(books.size() > 0){
		for(int i = 0; books.size() > i; i++){
			if(strcmp(books[i].book_course.dept_code.c_str(),cumuliative[0][1].c_str()) ==0){//go through all courses and find books with given department code
				for(int j = 0; books.size() > j; j++){
					if(strcmp(books[j].book_course.course_number.c_str(), cumuliative[i][2].c_str()) == 0){//go through all books with correct dept code and find correct course number
						cout << "Books for " << cumuliative[i][1] << " " << cumuliative[i][2] << endl;
						cout << "ISBN: " << books[j].isbn << endl;
						cout << "Name of Book: " << books[j].book_name << endl;
						cout << "Author: " << books[j].author_name << endl;
						cout << "N, U, E, or R: " << books[j].condition << endl;
						cout << "Section Number: " << books[j].book_course.section_number << endl;
						cout << "Price: $" << books[j].price << endl;
						cout << "Edition: " << books[j].edition << endl;
						cout << "Date: " << books[j].date << endl;
						cout << "Required or Optional: " << books[j].req_or_opt << endl;
						cout << endl;
					}
				}
			}
		}
	}
}

void gs_command(vector<vector<std::string>> &cumuliative){
	course * temp_course;
	if(books.size() > 0){
		for(int i = 0; books.size() > i; i++){
			if(strcmp(books[i].book_course.dept_code.c_str(), cumuliative[i][1].c_str()) == 0){//find correct dept code
				for(int j = 0; books.size() > j; j++){
					if(strcmp(books[j].book_course.course_number.c_str(), cumuliative[i][2].c_str()) == 0){//find correct course number
						for(int k = 0; books.size() > k; k++){
							if(strcmp(books[k].book_course.section_number.c_str(), cumuliative[i][3].c_str()) == 0){//find correct section number
								cout << "Books for " << cumuliative[i][1] << " " << cumuliative[i][2] << " Section " << cumuliative[i][3] << endl;
								cout << "ISBN: " << books[k].isbn << endl;
								cout << "Name of Book: " << books[k].book_name << endl;
								cout << "Author: " << books[k].author_name << endl;
								cout << "N, U, E, or R: " << books[k].condition << endl;
								cout << "Section Number: " << books[k].book_course.section_number << endl;
								cout << "Price: $" << books[k].price << endl;
								cout << "Edition: " << books[k].edition << endl;
								cout << "Date: " << books[k].date << endl;
								cout << "Required or Optional: " << books[k].req_or_opt << endl;
								cout << endl;
							}
						}
					}
				}
			}
		}
	}
}

void gb_command(vector<vector<std::string>> &cumuliative){
	course * temp_course;
	if(books.size() > 0){
		for(int i = 0; books.size() > i; i++){
			if(strcmp(books[i].isbn.c_str(), cumuliative[i][1].c_str()) == 0){//print all info given isbn
				cout << "Books for ISBN" << cumuliative[i][1] << endl;
				cout << "ISBN: " << books[i].isbn << endl;
				cout << "Name of Book: " << books[i].book_name << endl;
				cout << "Author: " << books[i].author_name << endl;
				cout << "N, U, E, or R: " << books[i].condition << endl;
				cout << "Section Number: " << books[i].book_course.section_number << endl;
				cout << "Price: $" << books[i].price << endl;
				cout << "Edition: " << books[i].edition << endl;
				cout << "Date: " << books[i].date << endl;
				cout << "Required or Optional: " << books[i].req_or_opt << endl;
				cout << endl;
			}
		}
	}
}

void pb_command(vector<vector<std::string>> &cumuliative){
	course * temp_course;
	if(books.size() > 0){
		cout << "Books--- " << endl;
		for(int i = 0; books.size() > i; i++){//print all books from book vector
				cout << "ISBN: " << books[i].isbn << endl;
				cout << "Name of Book: " << books[i].book_name << endl;
				cout << "Author: " << books[i].author_name << endl;
				cout << "N, U, E, or R: " << books[i].condition << endl;
				cout << "Section Number: " << books[i].book_course.section_number << endl;
				cout << "Price: $" << books[i].price << endl;
				cout << "Edition: " << books[i].edition << endl;
				cout << "Date: " << books[i].date << endl;
				cout << "Required or Optional: " << books[i].req_or_opt << endl;
				cout << endl;
			
		}
	}
}

void pc_command(vector<vector<std::string>> &cumuliative){
	course * temp_course;
	if(courses.size() > 0){
		cout << "Courses--- " << endl;
		for(int i = 0; courses.size() > i; i++){//print all courses from course vector
				cout << "Dept Code: " << courses[i].dept_code << endl;
				cout << "Course Number: " << courses[i].course_number << endl;
				cout << "Section Number " << courses[i].section_number << endl;
				cout << "Course Name: " << courses[i].course_name << endl;
				cout << endl;
		}
	}
}

void py_command(vector<vector<std::string>> &cumuliative){
	course * temp_course;
	if(cumuliative[cumuliative.size() -1].size() == 2){
		if(books.size() > 0){
			cerr << "Books Published Past the Date " << cumuliative[0][1] << endl;
			for(int i = 0; books.size() > i; i++){
				int input_month, random_month;
				int input_year, random_year;
				sscanf(cumuliative[0][1].c_str(),"%2d/%4d",&random_month,&random_year);//scan string into month and year integers
				sscanf(books[i].date.c_str(),"%2d/%4d",&input_month,&input_year);//scan string into month and year integers
					if(random_year <= input_year){//print all books that have a greater date 
						if(random_month <= input_month){
						cout << "ISBN: " << books[i].isbn << endl;
						cout << "Name of Book: " << books[i].book_name << endl;
						cerr << endl;
						}
					}
			}
		}
	}
	else{
		cerr << "Argument data invalid.\n";
	}
}

void pd_command(vector<vector<std::string>> &cumuliative){
	course * temp_course;
	if(cumuliative[cumuliative.size() -1].size() == 2){
		if(books.size() > 0){
			cerr << "Books in the department of " << cumuliative[0][1] << endl;
			for(int i = 0; books.size() > i; i++){
				if(strcmp(books[i].book_course.dept_code.c_str(),cumuliative[0][1].c_str()) == 0){//print all books within a given department using for loop through all books
					cout << "ISBN: " << books[i].isbn << endl;
					cout << "Name of Book: " << books[i].book_name << endl;
					cerr << endl;
				}
			}
		}
	}
	else{
		cerr << "Argument data invalid.\n";
	}
}

void pm_command(vector<vector<std::string>> &cumuliative){
	course * temp_course;
	if(books.size() > 0){
		double max = stod(books[0].price);
		double min = stod(books[0].price);
		double total_max = 0;
		double total_min = 0;
		vector<string> total_section;
		if(cumuliative[cumuliative.size() -1].size() == 2){
			if(books.size() > 0){
				for(int i = 0; books.size() > i; i++){//loop through all books
					if(strcmp(books[i].price.c_str()," ") != 0 || strcmp(books[i].book_course.dept_code.c_str()," ") != 0){//check to see if book has no price
						double new_book;
						new_book = stod(books[i].price);
						if(books[i].book_course.dept_code == cumuliative[0][1]){
							if(new_book > max){//set max if greater than current book
								max = new_book;
							}
							else if(new_book < min){//set min if lower than current book
								min = new_book;
							}
							
							if(books[i].req_or_opt == "R"){
								total_min = total_min + stod(books[i].price);//continuously add total price to mininum if required
							}
							total_max = total_max + stod(books[i].price);//continuously add total price to maximum for all
							
							if(total_section.size() == 0){
								total_section.push_back(books[i].book_course.section_number);//push section onto vector if empty
							}
							else{
								for(int j = 0; j < total_section.size(); j++){//search book vector for section number
										if(strcmp(total_section[j].c_str(), books[i].book_course.section_number.c_str()) != 0){
											total_section.push_back(books[i].book_course.section_number);
											break;
										}
								}
							}
							
						}
					}
				}
			}
			double average_min, average_max;
			average_max = total_max / total_section.size(); 
			average_min = total_min / total_section.size();
			cout << "Max: $" << max << endl;
			cout << "Min: $" << min << endl;
			cout << "Average Min: $" << average_min << endl;
			cout << "Average Max: $" << average_max << endl;
			cout << endl;
		}
		else{
			cerr << "Argument data invalid.\n";
		}
	}
}


int main (int argc, char * argv[]){
	int breaker = -1;
	
	vector<vector<std::string>> cumuliative;
	
	//book * first = new book();
	//book_init(first);
	//books.push_back(first);
	
	while(breaker == -1){							//Create while loop so user can exit program when necessary
		cout << "Please input now!\n";
		std::string line = " ";
		if ( getline(std::cin, line) )
		{

			vector<std::string> command;
			std::istringstream str(line);			//Use istream to separate the variables.
			string value;
			str >> value;
			command.push_back( value );
			char separator;
			
			while ( str >> value ) 
			{
				command.push_back( value );
			}
			
		cumuliative.push_back(command);
		//command line prompts with their respective functions.
		}
		int size = cumuliative.size() - 1;
		if(cumuliative[size][0] == "b" || cumuliative[size][0] == "B"){
			b_command(cumuliative);
		}
		else if(cumuliative[size][0] == "d" || cumuliative[size][0] == "D"){
			d_command(cumuliative);
		}
		else if(cumuliative[size][0] == "m" || cumuliative[size][0] == "M"){
			m_command(cumuliative);
		}
		else if(cumuliative[size][0] == "c" || cumuliative[size][0] == "C"){
			c_command(cumuliative);
		}
		else if(cumuliative[size][0] == "a" || cumuliative[size][0] == "A"){
			a_command(cumuliative);
		}
		else if(cumuliative[size][0] == "gc" || cumuliative[size][0] == "GC"){
			gc_command(cumuliative);
		}
		else if(cumuliative[size][0] == "gs" || cumuliative[size][0] == "GS"){
			gs_command(cumuliative);
		}
		else if(cumuliative[size][0] == "gb" || cumuliative[size][0] == "GB"){
			gb_command(cumuliative);
		}
		else if(cumuliative[size][0] == "pb" || cumuliative[size][0] == "PB"){
			pb_command(cumuliative);
		}
		else if(cumuliative[size][0] == "pc" || cumuliative[size][0] == "PC"){
			pc_command(cumuliative);
		}
		else if(cumuliative[size][0] == "py" || cumuliative[size][0] == "PY"){
			py_command(cumuliative);
		}
		else if(cumuliative[size][0] == "pd" || cumuliative[size][0] == "PD"){
			pd_command(cumuliative);
		}
		else if(cumuliative[size][0] == "pm" || cumuliative[size][0] == "PM"){
			pm_command(cumuliative);
		}
	/*	
	for(int i = 0; cumuliative.size() > i; i++){
		for(int j = 0; cumuliative[i].size() > j; j++){
				cout << cumuliative[i][j] << endl;
		}
	}
	*/
	
	restart(breaker);
	cin.ignore(256,'\n');							//Empty Cin Buffer
	cumuliative.clear();
	}
}