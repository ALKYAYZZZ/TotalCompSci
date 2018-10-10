#include <iostream>
#include <vector>

using namespace std;
using std::vector;

int main(){
	int recur = 0;
	int right = 0;
	int left = 0;
	std::cout << "What is the right number of the vector?\n";
	std::cin >> right;
	std::cout << "What is the left number of the vector?\n";
	std::cin >> left;
	std::cout << "What is the number you are looking for?\n";
	int num = 0;
	std::cin >> num;
	vector<int> v;
	if (right >= left){
	for (int i = left; i <= right; i++){
		v.push_back(i);
		}
	}
	else{
		for (int i = left; i >= right; i--){
		v.push_back(i);
		}
	}
	//for (int i = 0; i < v.size();i++)
		//cout << v[i] << "\n";
	int middle = ((right + left) / 2) - 1;//Account for index change of -1
	//cout << v[middle] << "\n";
	if (v[middle] >= 1){
	
		while(v[middle] != num){
			if (right != left){
			middle = ((right + left) / 2);
			}
			cout << v[middle] << "\n";
			if (num > v[middle]){
				left = v[middle];
				recur++;
			}
			else if (num < v[middle]){
				right = v[middle];
				recur++;
			}
			else
				recur++;
		}
	}
	std::cout << "Comparisons - " << recur << "\n";
	/*
	for (int i = 1; i <= v.size(); i++){
		if (recur > v[i]){
			std::cout << "This vector isnt sorted!\n";
			break;
		}
		int recur = v[i];
		if (size == v[i]){
			std::cout << "Comparisons- " << i << "\n";
			break;
		}
		
	}
	*/
}