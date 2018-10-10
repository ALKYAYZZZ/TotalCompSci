#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdexcept>


using namespace std;


void MakeNode(vector<int> &v1, long long int aData, vector<int> &v2)
{
    v1[0] = aData;//Push the key into v1
	v2[0] = 1;//Push its search cost into v2
}

void setleft(vector <int>&v1, long long int currIndex, long long int aData, vector<int> &v2, long long int &heightleft,long long int &highestindex)
{
	currIndex = 2 * currIndex + 1;//Change the current index to the index that describes a node to the left of the given element
	v1[currIndex] = aData;
	if(currIndex >= highestindex)//Check to see if the current index is higher than the highest index parsed thusfar to calculate average search cost when printing
		highestindex = currIndex;
	heightleft = int((log(currIndex + 1)/log(2)) + 1);//Change the height of the left side of the tree
	v2[currIndex] = heightleft;//Change the current index to the height of the left side of the tree
}

void setright(vector<int> &v1, long long int currIndex, long long int aData, vector<int> &v2, long long int &heightright, long long int &highestindex)
{
	currIndex = 2 * currIndex + 2;//Change the current index to the index that describes a node to the right of the given element
    v1[currIndex] = aData;
	if(currIndex >= highestindex)//Check to see if the current index is higher than the highest index parsed thusfar to calculate average search cost when printing
		highestindex = currIndex;
	heightright = int((log(currIndex + 1)/log(2)) + 1);//Change the height of the right side of the tree
	v2[currIndex] = heightright;//Change the current index to the height of the right side of the tree
}

void Insert(vector<int> &v1, long long int aData, vector<int> &v2, long long int &heightleft, long long int &heightright, long long int &highestindex)
{
	
    if(v1.size() == 0)//Check if the vector exists
    {
        cout<<"Note is not made yet. MakeNode first..."<<endl;
        return;
    }
    long long int currentIdx = 0;
	int place = 0;
    while ( currentIdx < v1.size() )//Go throughout all indicies of vector greater than the current index
    {

        if(aData <= v1[currentIdx])//If the new element is less than the original element, place in the corresponding vector space that describes the left of the given element
        {
			
            if( v1[(2 * currentIdx) + 1] == 0)
            {
                setleft(v1, currentIdx, aData, v2, heightleft, highestindex);//If the space is open, place the element in the given space 
                break;
            }
            else
			{
                currentIdx = 2 * currentIdx + 1;//If the space is not open change the index to the next element to the left of the parsed element
			}
        }
		
        else
        {
            if(v1[(2 * currentIdx) + 2] == 0)//If the new element is greater than the original element, place in the corresponding vector space that describes the right of the given element
            {
                setright(v1, currentIdx, aData, v2, heightright,highestindex);//If the space is open, place the element in the given space 
                break;
            }
            else
			{
				currentIdx = 2 * currentIdx + 2;//If the space is not open change the index to the next element to the right of the parsed element
			}

        }
		
    }

	for(int i = 0; i <= highestindex; i++){
		if(v2[i] == 0)
			v2[i] = int((log(i + 1)/log(2))+ 1);//If the index is higher than the previous current index, set highestindex to the current index
	}
}



void Print(vector <int> &v1, long long int Idx, vector<int> &v2, long long int &highestindex){
	double size = 0;
	for(int i = 0; i < v1.size(); i++){
	if(v1[i] == 0)//Skip over blank nodes
		continue;
	cout<<v1[i]<<"["<<v2[i]<<"] ";//Print all the keys from v1 and all the search costs from v2
	if(v1[i] != 0)
		size = size + 1;//Sum up the number of nodes 
	}
	cout << endl;
	cout << "The total number of nodes is " << size << "\n";//Print total number of nodes
	double sum = 0;
	for (int i = 0; i <= highestindex; i++){
		if(v1[i] != 0)
			sum = sum + v2[i];//Sum up all the search costs of the non-blank nodes
	}
	double average = sum / size;
	cout << "The average search cost is " << average << "\n";//Output the average search cost
		
}

void PrintTree(vector <int> &v1, long long int Idx, vector<int> &v2, long long int &highestindex){
	for (int j = 1; j <= v2[highestindex]; j++){
		for(int i = 0; i < v1.size(); i++){
				if(v1[i] == 0 && v2[i] != 0 && v2[i] == j)//Go through vector and print out an X everytime there is a zero in the key location.
					cout << "X" << " ";
				else if(v2[i] != 0 && v2[i] == j){//Print all other numbers and start a new line everytime a new search cost begins
					cout << v1[i] << " ";
				}
		}
		cout << endl;
	}
	
}

void Remove(vector<int> &v1, long long int aData, vector<int> &v2, long long int &heightleft, long long int &heightright, long long int &highestindex, string file){
	int userkey;
	cout << "Enter the key of the node that you'd like to remove: ";//User input for the key of the element to be removed.
	cin >> userkey;
	long long int vecsize = (1e6);
	vector<int> v3(vecsize);//Create two new vectors to re-parse through the file, skipping over the chosen element to be removed.
	vector<int> v4(vecsize);
	int line;
	int lineno = 0;
	ifstream in(file);
	while(in>>line){//Re-parse through file
		if(line == userkey){
			continue;
			}
		if (lineno == 0){
			MakeNode(v3,line,v4);
			v4.push_back(1);
		}
		else{	
			Insert(v3,line, v4,heightleft,heightright, highestindex);
		}
		lineno++;
	}
	v1.clear();//Delete elements in v1 and v2 to free memory for the transformed vectors
	v2.clear();
	v1 = v3;//set v1 and v2 to their counterparts to copy the data of the new tree that does not contain the removed node.
	v2 = v4;
	
}
	

int main (){
try{
	string file = "2p";//File that you want to use to make binary search tree
	char userinput;
	cout << "Does this file contain only characters less than 2^4? (y for yes, n for no) ";//Ask if you would like to print the tree
	cin >> userinput;
	cout << endl;
	ifstream in(file);
	if (!in){//Checks to see if the file exists
		cerr << "No File!";
		return -1;
	}
	long long int vecsize = (1e8);
	int line;
	vector<int> v1(vecsize);//Create two vectors corresponding to the key and searchcost of each node
	vector<int> v2(vecsize);
	int lineno = 0;
	long long int heightleft = 1, heightright = 1; 
	long long int highestindex = 0;
	cout << "Input: \n";
	while(in>>line){//Parse through file and insert element in corresponding vector space using the insert function
		cout << line << endl;
		if (lineno == 0){
			MakeNode(v1,line,v2);
			v2.push_back(1);
		}
		else
			Insert(v1,line, v2,heightleft,heightright, highestindex);
		lineno++;
	}
	cout << endl;
	Print(v1,0, v2,highestindex);//Print using print function
	if(userinput == 'y')//If user wants to print the tree, use the PrintTree and Remove functions to do said task for any file smaller than 2^4-1
	{
	PrintTree(v1,0,v2, highestindex);
	Remove(v1,0,v2,heightleft,heightright,highestindex,file);
	Print(v1,0,v2,highestindex);
	PrintTree(v1,0,v2, highestindex);
	}
}

catch(out_of_range &error){
	cerr << "Error: " << error.what() << endl;
}
}