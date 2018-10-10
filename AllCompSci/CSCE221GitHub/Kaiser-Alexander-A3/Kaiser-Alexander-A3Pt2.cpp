#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <string>
#include <regex>
#include <stdexcept>
using namespace std;

//class LinkedList{
    struct Node {//Create Node struct
        string name, address, course;//Constructor for strings of Node Class
		Node(){name = ""; address = ""; course = "";}//
    };


int main() {
try{ 
	ifstream in("Peer-TeacherHTMLONLY.html");//Ifstream for .html file of peer teacher web page.
	if (!in){
		cerr << "no file\n";
		return -1;
	}
	int lineno = 0;
	string line;
	int j = 0;
	
	cout << "Name Regex Pattern: \\w+\\s\\w+(-)?\\w*(?=</h3>)\n";
	cout << "Address Regex Pattern: \\w*(\\W*)?(\\d*)?@\\w*(.\\w*)?(?:.edu|.com)\n";
	cout << "Course Regex Pattern: \\w{4}\\s\\d{3}(?=\\s-\\s\\d{3}|</li>)\n\n";
	
	std::vector<std::list<Node>> a;
	smatch name, address, course;
	std::vector<string> namevec, addressvec, coursevec;
	std::vector<int> linecounter,namelinecounter;
	while(getline(in,line)){
	
		lineno++;
		regex pattern1 ("\\w+\\s\\w+(-)?\\w*(?=</h3>)");
		regex pattern2 ("\\w{4}\\s\\d{3}(?=\\s-\\s\\d{3}|</li>)");
		regex pattern3 ("\\w*(\\W*)?(\\d*)?@\\w*(.\\w*)?(?:.edu|.com)");
		if(regex_search(line,name,pattern1)){
			namevec.push_back(name[0]);
			namelinecounter.push_back(lineno);
		}
		if(regex_search(line,address,pattern3))
			addressvec.push_back(address[0]);
		if(regex_search(line,course,pattern2)){
			coursevec.push_back(course[0]);
			linecounter.push_back(lineno);
		}
	}
		/*
		cerr << namevec.size();
		cerr << addressvec.size();
		cerr << coursevec.size();
		cerr << coursevec[0];
		cerr << linecounter.size();
		*/
		for(int i = 0; i < namevec.size();i++){
		std::list<Node> r;
		if((linecounter[j] - namelinecounter[i]) != 4 && i != 0){
			Node n;
			n.name = namevec[i];
			n.address = addressvec[i];
			n.course = "No Course Listed";
			r.push_back(n);
		}
		else{
			if((linecounter[j] - linecounter[j-1]) != 1){
			Node n;
			n.name = namevec[i];
			n.address = addressvec[i];
			n.course = coursevec[j];
			r.push_back(n);
			j++;
			}
			while((linecounter[j] - linecounter[j-1]) == 1){
			Node n;
			n.name = namevec[i];
			n.address = addressvec[i];
			n.course = coursevec[j];
			r.push_back(n);
			j++;
			}
		}
		a.push_back(r);
		}

	
		
	int k = 0;
	for(int i = 0; i < a.size(); i++){
		list<Node> x =  a.at(i);
		if(x.size() != 0){
			while(!x.empty()){
			Node y = x.back();
			cout << "Name: " << y.name << "\n";
			cout << "Email: " << y.address << "\n";
			cout << "Course: " << y.course << "\n\n";
			x.pop_back();
			k++;
			}
		}
		}
		
		
		cout << "-----------------------------------------------------------------------------------------------\n";
	
		std::vector<std::list<string>> s;
		std::list<string> list121,list111,list110,list221,list206,list222,list313,list312,list315;
		regex pattern121 ("CSCE 121");
		regex pattern221 ("CSCE 221");
		regex pattern111 ("CSCE 111");
		regex pattern110 ("CSCE 110");
		regex pattern222 ("CSCE 222");
		regex pattern206 ("CSCE 206");
		regex pattern312 ("CSCE 312");
		regex pattern315 ("CSCE 315");
		regex pattern313 ("CSCE 313");
	for(int i = 0; i < a.size(); i++){
		list<Node> x = a.at(i);
		if(x.size() != 0){
			
			while(!x.empty()){
			Node y = x.back();
			if(regex_search(y.course,name,pattern121)){
				list121.push_back(y.name);
			}
			if(regex_search(y.course,name,pattern111)){
				list111.push_back(y.name);
			}
			if(regex_search(y.course,name,pattern110)){
				list110.push_back(y.name);
			}
			if(regex_search(y.course,name,pattern221)){
				list221.push_back(y.name);
			}
			if(regex_search(y.course,name,pattern206)){
				list206.push_back(y.name);
			}
			if(regex_search(y.course,name,pattern222)){
				list222.push_back(y.name);
			}
			if(regex_search(y.course,name,pattern312)){
				list312.push_back(y.name);
			}
			if(regex_search(y.course,name,pattern313)){
				list313.push_back(y.name);
			}
			if(regex_search(y.course,name,pattern315)){
				list315.push_back(y.name);
			}
			x.pop_back();	
		}
	}
	}
	s.push_back(list110);
	s.push_back(list111);
	s.push_back(list121);
	s.push_back(list206);
	s.push_back(list221);
	s.push_back(list222);
	s.push_back(list312);
	s.push_back(list313);
	s.push_back(list315);
	
	
	
	for(int i = 0; i < s.size(); i++){
		list<string> x =  s.at(i);
		if(i == 0)
			cout << "Number of Peer Teachers in CSCE 110: " << x.size() << "\n";
		if(i == 1)
			cout << "Number of Peer Teachers in CSCE 111: " << x.size() << "\n";
		if(i == 2)
			cout << "Number of Peer Teachers in CSCE 121: " << x.size() << "\n";
		if(i == 3)
			cout << "Number of Peer Teachers in CSCE 206: " << x.size() << "\n";
		if(i == 4)
			cout << "Number of Peer Teachers in CSCE 221: " << x.size() << "\n";
		if(i == 5)
			cout << "Number of Peer Teachers in CSCE 222: " << x.size() << "\n";
		if(i == 6)
			cout << "Number of Peer Teachers in CSCE 312: " << x.size() << "\n";
		if(i == 7)
			cout << "Number of Peer Teachers in CSCE 313: " << x.size() << "\n";
		if(i == 8)
			cout << "Number of Peer Teachers in CSCE 315: " << x.size() << "\n";
		while(!x.empty()){
		cout << "Name: " << x.back() << "\n";
		x.pop_back();
		}
		cout << "\n";
	}
	
}

catch(out_of_range &error){
	cerr << "Error: " << error.what() << endl;
}	

}