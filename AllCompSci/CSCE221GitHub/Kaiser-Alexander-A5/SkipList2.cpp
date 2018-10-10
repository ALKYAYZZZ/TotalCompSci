#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include "DoublyLinkedList.cpp"

void newLevel(vector<DoublyLinkedList*> &l, int &maxsize, int key){
	DoublyLinkedList* dll = new DoublyLinkedList();
	dll->insertFirst(key);
	l.push_back(dll);
}

void insertFirstRow(vector<DoublyLinkedList*> &l, int key){
	DoublyLinkedList* dll = l[0];
	DListNode* p = dll->getFirst();
	
	if (key > p->getElem()){
		while(p->getElem() < key && p->getNext() != NULL){
			p = p->getNext();
		}
		p->insert_before(key);
	}
	else
		p->insert_before(key);
}

int insertTopRows(vector<DoublyLinkedList*> &l, int key, int &maxsize){

	
	int passon = rand()%2;
	int newmaxsize = 0;
	while(passon == 1){
		newmaxsize++;
		if(maxsize < newmaxsize){
			maxsize = newmaxsize;
			newLevel(l, maxsize, key);
		}
		else{
			
			DoublyLinkedList* dll = l[newmaxsize];
			
			DListNode* p = dll->getFirst();
			
			if (key > p->getElem()){
				while(p->getElem() < key && p->getNext() != NULL){
					p = p->getNext();
				}
				p->insert_before(key);
			}
			else
				p->insert_before(key);
		}	
		
		passon = rand()%2;
	}
	
	return newmaxsize;


}

int insertComparisons(vector<DoublyLinkedList*> &l, int key, int &maxsize){
	int listsize = l.size() - 1;
	int elementholder = numeric_limits<int>::min();
	int comparisons = 1;
	while (listsize != -1){
		DoublyLinkedList* dll = l[listsize];
		DListNode* p = dll->getFirst()->getPrev();
		while(p->getElem() != elementholder){
			p = p->getNext();
		}
			if(p->getNext()->getElem() > key){
				comparisons++;
				listsize--;
				elementholder = p->getElem();
			}
			else{
				while(p->getNext()->getElem() < key){
					p = p->getNext();
					comparisons++;
				}
				comparisons++;
				listsize--;
				elementholder = p->getElem();
			}
			
	}
	return comparisons;
	
}

int find(vector<DoublyLinkedList*> &l, int key, int &maxsize){
	int listsize = l.size() - 1;
	int elementholder = numeric_limits<int>::min();
	int comparisons = 1;
	bool end = false;
	while (listsize != -1 && end != true){
		DoublyLinkedList* dll = l[listsize];
		DListNode* p = dll->getFirst()->getPrev();
		while(p->getElem() != elementholder){
			p = p->getNext();
		}
			if(p->getNext()->getElem() < key){
				while(p->getNext()->getElem() < key){
					p = p->getNext();
					comparisons++;
					if(p->getElem() == key){
						comparisons++;
						end = true;
						break;
					}
				}
				comparisons++;
				listsize--;
				elementholder = p->getElem();
			}
			else if (p->getNext()->getElem() > key){
				
				comparisons++;
				listsize--;
				elementholder = p->getElem();
			}
			else{
				comparisons++;
				break;
			}
	}
	return comparisons;
	
}

int searchRow(DoublyLinkedList* &dll, int key){
	bool statement = false;
	DListNode* p = dll->getFirst()->getPrev();
	while(p->getNext() != NULL){
		p = p->getNext();
		if(p->getElem() == key)
			statement = true;
		
	}

	return statement;
		
}


int deleteRows(vector<DoublyLinkedList*> &l, int key, int &maxsize){
	for(int i = l.size() - 1; i >= 0; i--){
		DoublyLinkedList* dll = l[i];
		DListNode* p = dll->getFirst()->getPrev();
		
		if(searchRow(dll,key) == true){
			while(p->getElem() != key && p->getNext() != NULL){
				p = p->getNext();
			}
				p = p->getNext();
				p->delete_before();
		}
		

	if(dll->isEmpty()){
		delete dll;
		l.pop_back();
		
	}
	}

}


int main(){
	srand(time(NULL));
	string file = "4p";
	char userinput;
	cout << "Does this file contain only characters less than 2^4? (y for yes, n for no) ";//Ask if you would like to print the tree
	cin >> userinput;
	cout << endl;
	int line;
	int lineno = 0;
	int maxsize = 0;
	int level = 0;
	int comparisoncount = 0;
	int deletecomparisons = 0;
	vector<DoublyLinkedList*> l;
	vector<int> numbers, levelvec, comparisonvec;
	ifstream in(file);
	if (!in){//Checks to see if the file exists
		cerr << "No File!";
		return -1;
	}
	cout << "Input: \n";
	while(in>>line){
		cout << line << endl;
		
		if (lineno == 0){
			comparisonvec.push_back(maxsize + 2);
			newLevel(l,maxsize,line);
			level = insertTopRows(l,line,maxsize);
		}
		else{
			comparisoncount = insertComparisons(l, line, maxsize);
			insertFirstRow(l, line);
			level = insertTopRows(l,line,maxsize);
		}
		numbers.push_back(line);
		levelvec.push_back(level);
		if(lineno != 0)
			comparisonvec.push_back(comparisoncount);
		lineno++;

	}
	cout << endl;
	if(userinput == 'y'){
		for(int i = 0; i < numbers.size(); i++){
			cout << "Inserting value " << numbers[i] << " at level " << levelvec[i] << " with " << comparisonvec[i] << " comparisons.\n";
		}
		
		
		cout << endl;
		cout << "-Infinity Infinity\n";
		for(int i = (l.size()-1); i >= 0; i--){
			cout << *l[i];
			cout << endl;
		}
	}
	else{
		for(int i = 0; i < numbers.size(); i++){
			cout << "Inserting value " << numbers[i] << " at level " << levelvec[i] << endl;
		}
		cout << endl;
	}
	
	cout << endl;
	vector<int> deletecomparisonvec(lineno + 1);
	
	for(int i = numbers.size() - 1; i >= 0; i--){
		deletecomparisons = find(l, numbers[i], maxsize);
		deletecomparisonvec[i] = deletecomparisons;
		deleteRows(l, numbers[i], maxsize);
		}
	if(userinput == 'y'){	
		for(int i = numbers.size() - 1; i >= 0; i--){
			cout << "Deleting value " << numbers[i] << " with " << deletecomparisonvec[i] << " comparisons.\n";
		}
	}
	else
	{	
		for(int i = numbers.size() - 1; i >= 0; i--){
			cout << "Deleting value " << numbers[i] << endl;
		}
	}
	
	double deleteavg = 0;
	double insertavg = 0;
	for(int i = 0; i <= numbers.size() - 1; i++){
		deleteavg = deleteavg + deletecomparisonvec[i];
		insertavg = insertavg + comparisonvec[i];
	}
	deleteavg = deleteavg / (lineno + 1);
	insertavg = insertavg / (lineno + 1);
	
	cout << "The average insert cost was " << insertavg;
	cout << endl;
	cout << "The average delete cost was " << deleteavg;
	cout << endl;
		
	
	
}