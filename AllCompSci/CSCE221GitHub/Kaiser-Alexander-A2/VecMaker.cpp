#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(){
	ofstream outputFile;
	int size = 100000;
	vector<int> v;
	vector<int> v2;
	vector<int> v3;
	int num = 0;
	
	outputFile.open("newvector100000rand.txt");
	v.push_back(size);
	for (int i = 0; i <= size; i++){
		num = (rand()%32767);
		v.push_back(num);
		}
	for (int i = 0; i < size + 2; i++){
		outputFile << v[i] << " ";
		}
	outputFile.close();
	
	outputFile.open("newvector100000inc.txt");
	v2.push_back(size);
	for (int i = 1; i <= size; i++){
		//num = (rand())%(101);
		v2.push_back(i);
		}
	for (int i = 0; i < size + 2; i++){
		outputFile << v2[i] << " ";
		}
	outputFile.close();
	
	outputFile.open("newvector100000dec.txt");
	v3.push_back(size);
	for (int i = size; i >= 0; i--){
		//num = (rand())%(101);
		v3.push_back(i);
		}
	for (int i = 0; i < size + 2; i++){
		outputFile << v3[i] << " ";
		}
	outputFile.close();
}