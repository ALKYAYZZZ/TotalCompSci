//
// Database.cpp
// Created by Suqian Wang on 10/11/2017.
// Team 44
//

#include "Database.h"
#include "aux_function.h"

#include <algorithm>

Database::Database(){
    //database constructor
}

void Database::add_table(Table table, string table_name){
    table_map[table_name] = table;
}

void Database::drop_table(string table_name){
    table_map.erase(table_name);
}

vector<string> Database::list_table(){
    vector<string> table_name;
    for (map<string, Table>::iterator iter = this->table_map.begin(); iter != table_map.end(); iter++) {
        table_name.push_back(iter->first);
    }
    
    return table_name;
}

vector<Table> Database::get_tables(){
    vector<Table> tables;
    for (auto& pair_table : this->table_map) {
        tables.push_back(pair_table.second);
    }
    return tables;
}


// Query function, implemented by Suqian Wang
// select_command can either be "*" or "<attribute_name_1>,<attribute_name_2>,...,<attribute_name_n>", no space in attribute name
// from_command is a single table name
// where_command is several conditionals concatenate by AND, OR, NOT, each comparison must be in a pair of parenthese
Table Database::Query(string select_command, string from_command, string where_command) {
    
    if (this->table_map.find(from_command) == this->table_map.end()) {
        return Table();
    }
    
    // copy from original table
    Table table_copy = this->table_map[from_command];
    
    // declare a condition checker set_precedence
    Cnd_Checker cnd_checker;
    cnd_checker.create_comparator(where_command);
    cnd_checker.set_table(table_copy);
    
    // get bool value for all record;
    vector<bool> record_check = cnd_checker.record_check();
    
    // search for select columns
    vector<int> select_column;
    vector<string> attribute = table_copy.get_attributes();
    vector<string> select_attribute;
    
    // if select all;
    if (select_command == "*") {
        select_column = vector<int>(attribute.size());
        for (int i = 0; i < attribute.size(); i++) {
            select_attribute.push_back(attribute.at(i));
            select_column[i] = i;
        }
    }
    // if select part of, split by ","
    else {
        string no_space = select_command;
        no_space.erase(remove(no_space.begin(), no_space.end(), ' '), no_space.end());
        vector<string> temp_attribute = split_string(no_space, ',');
        
        // find all select attributes and their original position in table
        for (int i = 0; i < attribute.size(); i++) {
            if (find(temp_attribute.begin(), temp_attribute.end(), attribute.at(i)) != temp_attribute.end()) {
                select_attribute.push_back(attribute.at(i));
                select_column.push_back(i);
            }
        }
    }
    
    int record_size = int(select_column.size());
    Table result_table(select_attribute);
    Record record(record_size);
    
    // generate select records
    for (int row = 0; row < table_copy.get_size(); row ++) {
        if (!record_check.at(row))
            continue;
        
        Record original_record = table_copy.return_record(row);
        int record_index = 0;
        for (int col : select_column) {
            record.set_record_entry(record_index, original_record.get_record_entry(col));
            record_index++;
        }
        
        result_table.insert_record(record);
    }
    
    return result_table;
}
