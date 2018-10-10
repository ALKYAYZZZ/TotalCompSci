//
// Database.h
// Created by Dario Avendano on 9/24/2017.
// Team 44
// Modified by Suqian Wang on 9/24/2017(add comments, modified naming)
// Modified by Suqian Wang on 10/11/2017(add helper functions)
//

#ifndef DATABASE_DATABASE_H_
#define DATABASE_DATABASE_H_

#include "Table.h"

#include <string>
#include <vector>
#include <map>

using namespace std;


class Database {
private:

    map<string, Table> table_map;
    
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

