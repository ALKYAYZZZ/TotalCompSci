//
// Database.cpp
// Created by Dario Avendano on 9/24/2017.
// Team 44: Alex Kaiser, Dario Avendano, Suqian Wang
//

#include "Database.h"

Database::Database(){
    //database constructor
}

void Database::add_table(Table table, string table_name){
    database[table_name] = table;
}

void Database::drop_table(string table_name){
    database.erase(table_name);
}

vector<string> Database::list_table(){
    vector<string> tables_name;
    for(auto& it: database){
        tables_name.push_back(it.first);
    }

    return tables_name;
}
vector<Table> Database::get_tables(){
    vector<Table> tables;
    for(auto& it: database){
        tables.push_back(it.second);
    }
    return tables;
}

Table Database::Query(string select_command , string from_command , string where_command){
    Table table;
    return table;
}
