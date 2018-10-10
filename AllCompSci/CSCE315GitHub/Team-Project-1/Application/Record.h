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
private:
    
public:
    std::vector<std::string> recVecVals;
    Record(int arg);
    ~Record();
    void Print();
    void Set(int index, std::string addMe);
    void Remove(int index);
    void addBlank(int spaces);
    Record MarryMe(Record r2, bool uniques);
    
    int getSize();
    std::string& operator[] (int input);
};

#endif
