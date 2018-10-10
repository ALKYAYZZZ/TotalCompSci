#ifndef Record_h
#define Record_h

#include <stdio.h>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>

class Record
{
	int size = 0;
	std::vector<std::string> values;

public:
	Record(int arg);
	~Record();

	int getSize();
	std::string& operator[] (int input);
};

#endif 