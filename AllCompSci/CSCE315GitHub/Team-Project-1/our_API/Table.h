//
// Table.h
// Created by Alex Kaiser on 9/21/2017.
// Team 44: Alex Kaiser, Dario Avendano, Suqian Wang
//

#ifndef TABLE_H_
#define TABLE_H_

#include <string>
#include <vector>
#include <stdlib.h>

#include "Record.h"

using namespace std;

class Table{
    // attributes
    vector<string> columns;
    // records
    vector<Record> rows;
	// keys
	vector<string> keys;
public:
    // constructors
    Table() {};
    Table(vector<string> attribute_name);
    
    // attribute operations
    void add_attribute(string attribute_name);
    void delete_attribute(string attribute_name);
    vector<string> get_attributes() const;
    
    // record operations
    void insert_record(Record record);
    unsigned long int get_size() const;
    Record return_record(int index);
    
    // key operations
    void set_key(string attribute_name);
    
    // computation functions
    int count(string attribute_name);
    string max(string attribute_name);
    string min(string attribute_name);
    
    void set_columns(vector<string> columns);
    void set_rows(vector<Record> rows);

    vector<string> columns_out()  {return columns;}
    vector<Record> rows_out()  {return rows;}
    vector<string> keys_out()  {return keys;}
};

// join tables operations
Table cross_join(Table table_1, Table table_2);
Table natural_join(Table table_1, Table table_2);

#endif /* TABLE_H_ */
