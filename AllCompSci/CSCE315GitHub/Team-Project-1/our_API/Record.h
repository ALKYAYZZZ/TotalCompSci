//
// Record.h
// Created by Suqian Wang on 9/22/2017.
// Team 44: Alex Kaiser, Dario Avendano, Suqian Wang
//

#ifndef RECORD_H_
#define RECORD_H_

#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

class Record {
private:
    // an individual record
    vector<string> vec_record_;
    // number of attributes in a record
    int record_size_;
public:
    // constructors
    Record() {};
    Record(int record_size);
    
    // destructor
    ~Record() {};
    
    // get number of entries in a record
    int get_record_size() const;
    
    // get entry content
    string get_record_entry(int index) const;
    
    // set entry content
    void set_record_entry(int index, string record_entry);
};

#endif /* RECORD_H_ */
