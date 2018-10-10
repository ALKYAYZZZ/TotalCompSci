//
//  aux_function.h
//  TP1_CP3
//
//  Created by Susan Wang on 10/12/17.
//  Copyright © 2017 Suqian Wang. All rights reserved.
//

#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "Table.h"

using namespace std;

#ifndef aux_function_h
#define aux_function_h

/////////////////////////
// Data structure
/////////////////////////

// Data structure: represent date information
typedef struct Date {
    int year;
    int month;
    int day;
} Date;

// Time structure: represent date information
typedef struct Time {
    int hour;
    int minute;
} Time;

class Node {
public:
    Node(string set) { this->field = set; this->left = NULL; this->right = NULL; };
    
    string field;
    Node* left;
    Node* right;
};

class Cnd_Checker {
public:
    Node* cnd_root;
    Table* p_table;
    
    map<string, int> precedence;
    
    Cnd_Checker() { cnd_root = NULL; this->set_precedence(); }
    ~Cnd_Checker() { this->release_root(this->cnd_root); }
    
    void release_root(Node* root);
    
    void set_precedence();
    void set_table(Table& table) { this->p_table = &table; };
    
    vector<string> shunting_yard(string cnd_str);
    template<typename T> bool comparator(T entry, string cnd_op, T value);
    vector<bool> leaf_comparator(Node* leaf);
    vector<bool> node_logic(vector<bool>& left_bool, vector<bool>& right_bool, string logic);
    
    Node* create_comparator(string select_command);
    vector<bool> node_is_true(Node* root);
    vector<bool> record_check() { return this->node_is_true(this->cnd_root); };
};

/////////////////////////
// Helper functions
/////////////////////////
// helper function: determine if a string is a number or a date or time
bool is_number (const string& s);
bool is_date (const string& s);
bool is_time (const string& s);

// helper function: convert a variable of any type to a string
template <typename T>
string convert_to_string(T x) {
    stringstream ss;
    ss << x;
    string result;
    ss >> result;
    return result;
}

// convert a Date type variable to a string
string date_to_string(Date d);

// convert a Time type variable to a string
string time_to_string(Time t);

// split a string to a vector of strings by a character
vector<string> split_string (string s, char delimited);

// convert a string to Date structure
Date convert_to_date(string s);

// count relative days comparing to 0000-00-00
int count_days(Date d);

// convert a string to Time structure
Time convert_to_time(string s);

// count relative minutes comparing to 00:00
int count_minutes(Time t);

// show table
void print_table(Table& table, string table_name);
#endif /* aux_function_h */
