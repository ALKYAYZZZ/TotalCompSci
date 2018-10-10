//
// Database.h
// Created by Dario Avendano on 9/24/2017.
// Team 44: Alex Kaiser, Dario Avendano, Suqian Wang
//

#ifndef DATABASE_DATABASE_H_
#define DATABASE_DATABASE_H_
#include "Table.h"
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>

using namespace std;


class Database {
    map<string,Table> database;

public:
    //constructor
    Database();
    
    // table operations
    void add_table(Table table, string table_name);
    void drop_table(string table_name);
    vector<string> list_table();
    vector<Table> get_tables();
    
    // query operation
    Table Query(string select_command , string from_command , string where_command);
    
    
};


#endif /*DATABASE_DATABASE_H_*/
