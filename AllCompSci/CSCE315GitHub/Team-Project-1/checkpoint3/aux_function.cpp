//
//  table_aux_function.cpp
//  TP1_CP3
//
//  Created by Susan Wang on 10/12/17.
//  Copyright © 2017 Suqian Wang. All rights reserved.
//

#include "aux_function.h"

#include <cctype>
#include <iomanip>
#include <iostream>
#include <algorithm>

// recursively free all allocated memory for nodes
void Cnd_Checker::release_root(Node* root) {
    if (root->left != NULL) {
        this->release_root(root->left);
    }
    
    if (root->right != NULL) {
        this->release_root(root->right);
    }
    
    delete root;
}

// set precedence for all possible logic operators
void Cnd_Checker::set_precedence() {
    this->precedence["("] = -1;
    this->precedence[")"] = -1;
    this->precedence[">"] = 1;
    this->precedence["="] = 1;
    this->precedence["<"] = 1;
    this->precedence[">="] = 1;
    this->precedence["<="] = 1;
    this->precedence["NOT"] = 2;
    this->precedence["AND"] = 3;
    this->precedence["OR"] = 4;;
}

// get boolean value for a given equation with comparator operator
template <typename T> bool Cnd_Checker::comparator(T entry, string cnd_op, T value) {
    
    if (cnd_op == ">") {
        return entry > value;
    }
    else if (cnd_op == "=") {
        return entry == value;
    }
    else if (cnd_op == "<") {
        return entry < value;
    }
    else if (cnd_op == ">=") {
        bool test = entry >= value;
        return test;
    }
    else if (cnd_op == "<=") {
        return entry <= value;
    }
    else if (cnd_op == "!=") {
        return entry != value;
    }
    return false;
}

// get boolean value for a leaf node, which is a directly equation;
vector<bool> Cnd_Checker::leaf_comparator(Node* leaf) {
    vector<bool> node_bool;
    vector<string> attributes = this->p_table->get_attributes();
    
    // find the column for comparator entry
    int pos = find(attributes.begin(), attributes.end(), leaf->left->field) - attributes.begin();
    
    if (pos >= attributes.size()) {
        return node_bool;
    }
    
    // get boolean values for each record
    for (int i = 0; i < this->p_table->get_size(); i++) {
        string entry = this->p_table->return_record(i).get_record_entry(pos);
        node_bool.push_back(this->comparator(entry, leaf->field, leaf->right->field));
    }
    
    return node_bool;
}

// get boolean value for a given node with logic operators
vector<bool> Cnd_Checker::node_logic(vector<bool>& left_bool, vector<bool>& right_bool, string logic) {
    vector<bool> result;
    
    // switch to corresponding logic operator
    if (logic == "NOT") {
        for (int i = 0; i< right_bool.size(); i++) {
            result.push_back(!right_bool.at(i));
        }
    }
    else if (logic == "AND") {
        for (int i = 0; i < left_bool.size(); i++) {
            result.push_back(left_bool.at(i) && right_bool.at(i));
        }
    }
    else if (logic == "OR") {
        for (int i = 0; i < left_bool.size(); i++) {
            result.push_back(left_bool.at(i) || right_bool.at(i));
        }
    }
    
    return result;
}

// get output vector from shunting yard algorithm
vector<string> Cnd_Checker::shunting_yard(string cnd_str) {
    
    vector<string> op_stack;
    vector<string> output;
    
    size_t current_index = 0;
    string remain_str;
    string start, end;
    
    string equation_no_space = cnd_str;
    equation_no_space.erase(remove(equation_no_space.begin(), equation_no_space.end(), ' '), equation_no_space.end());
    
    remain_str = equation_no_space.substr(current_index);
    
    // current index goes 1 step each iteration
    for (; current_index != remain_str.size(); ) {
        remain_str = remain_str.substr(current_index);
        
        //cout << "remain_str = " << remain_str << endl;
        
        int find_pos = int(remain_str.size());
        string find_op;
        
        // search for the most left operators in the remaining substring
        for (auto& substring : precedence) {
            int op_pos = int(remain_str.find(substring.first));
            if (op_pos <= find_pos && op_pos >= 0) {
                find_pos = op_pos;
                find_op = substring.first;
            }
        }
        
        // if opeartor is at first
        if (find_pos == 0) {
            //cout << "find at first: " << find_op << endl;
            current_index = find_op.size();
            
            // stack is empty
            if (op_stack.empty()) {
                op_stack.push_back(find_op);
            }
            // left parenthesis
            else if (find_op == "(") {
                op_stack.push_back(find_op);
            }
            // right parenthesis
            else if (find_op == ")") {
                while (op_stack.back() != "(") {
                    output.push_back(op_stack.back());
                    op_stack.pop_back();
                }
                op_stack.pop_back();
            }
            // the upper element is parenthesis or the current operator's presidence is higher
            else if (precedence[op_stack.back()] == -1 || precedence[find_op] < precedence[op_stack.back()]) {
                op_stack.push_back(find_op);
            }
            // the upper element is not parenthesis and the current operator's presidence is equal or lower
            else if (precedence[op_stack.back()] != -1 && precedence[find_op] >= precedence[op_stack.back()]) {
                output.push_back(op_stack.back());
                op_stack.pop_back();
                op_stack.push_back(find_op);
            }
        }
        // if operator is not the first
        else if (find_pos < remain_str.size()) {
            //cout << "find at other place" << find_op << ", the intermedia part is " << remain_str.substr(0, find_pos) << endl;
            current_index = find_pos;
            output.push_back(remain_str.substr(0, find_pos));
        }
        // if there is no operator in the remaining substring
        else {
            //cout << "no more operators, remain is " << remain_str.substr(0) << endl;
            current_index = remain_str.size();
            output.push_back(remain_str.substr(0));
        }
    }
    
    if (!op_stack.empty()) {
        reverse(op_stack.begin(), op_stack.end());
        output.insert(output.end(), op_stack.begin(), op_stack.end());
    }
    
    return output;
}

// create a comparator node tree from select command
Node* Cnd_Checker::create_comparator(string select_command) {
    
    vector<string> output = shunting_yard(select_command);
    vector<Node*> node_stack;
    vector<string> op_stack;
    
    string op_str;
    int current_index = 0;
    Node* node;
    
    // from begining of output, create node for each new element, including variale and operators
    // each time accounting a operator,
    // the last two push in elements will be poped to create tree branched
    while (current_index != output.size()) {
        
        // if it is an operator.
        if (precedence.find(output.at(current_index)) != precedence.end()) {
            
            op_str = output.at(current_index);
            
            // create node for operator;
            node = new Node(op_str);
            
            // pop the last element in stack and linked it with right pointer;
            Node* right_node = node_stack.back();
            op_stack.pop_back();
            node_stack.pop_back();
            node->right = right_node;
            
            // if operator is "NOT", left pointer remains NULL
            // otherwise pop the last element in stack and liked it with left pointer.
            if (op_str != "NOT") {
                Node* left_node = node_stack.back();
                op_stack.pop_back();
                node_stack.pop_back();
                node->left = left_node;
            }
            
            // push operator and corresponding node pointer.
            op_stack.push_back(op_str);
            node_stack.push_back(node);
        }
        
        // if it is a variable or value
        else {
            
            // create node and push
            node = new Node(output.at(current_index));
            op_stack.push_back(output.at(current_index));
            node_stack.push_back(node);
        }
        current_index++;
    }
    
    // set the root, which should be the last element in stack
    this->cnd_root = node_stack.back();
    return node_stack.back();
}

// recursively get the boolean value at a node
vector<bool> Cnd_Checker::node_is_true(Node* root) {
    vector<bool> left_bool;
    vector<bool> right_bool;
    
    // get boolean value from left node
    if (root->right != NULL) {
        if (this->precedence.find(root->right->field) != this->precedence.end()) {
            right_bool = this->node_is_true(root->right);
        }
        else {
            return leaf_comparator(root);
        }
    }
    
    // get boolean value from right node
    if (root->left != NULL) {
        left_bool = this->node_is_true(root->left);
    }
    
    return this->node_logic(left_bool, right_bool, root->field);
}
// helper function: check if a string is a fraction
bool is_number (const string& s) {
    try {
        stod(s);
    }
    catch(...) {
        return false;
    }
    return true;
}

// helper function: check if a string represent a date
bool is_date (const string& s) {
    Date date;
    try{
        date = convert_to_date(s);
    }
    catch(...) {
        return false;
    }
    return true;
}

// helper function: check if a string represent time
bool is_time (const string& s) {
    Time t;
    try{
        t = convert_to_time(s);
    }
    catch(...) {
        return false;
    }
    return true;
}

// split a string to a vector of strings by a character
vector<string> split_string (string s, char delimited) {
    stringstream ss (s);
    string token;
    vector<string> result;
    while (getline(ss, token, delimited)) {
        result.push_back(token);
    }
    return result;
}

// convert a string format date to Date structure
Date convert_to_date(string s) {
    vector<string> s_date_entry = split_string(s, '-');
    Date date;
    date.year = stod(s_date_entry.at(0));
    date.month = stod(s_date_entry.at(1));
    date.day = stod(s_date_entry.at(2));
    return date;
}

// count relative days comparing to 0000-00-00
int count_days(Date d) {
    return (d.year * 365 + d.month * 30 + d.day);
}

// convert a Date type variable to a string
string date_to_string(Date d) {
    stringstream ss;
    ss << setw(4) << setfill('0') << d.year << "-" << setw(2) << setfill('0') << d.month << "-" << setw(2) << setfill('0') << d.day;
    return ss.str();
}

// convert a string to Time structure
Time convert_to_time(string s) {
    vector <string> s_time_entry = split_string(s, ':');
    Time t;
    t.hour = stod(s_time_entry.at(0));
    t.minute = stod(s_time_entry.at(1));
    return t;
}

// count relative minutes comparing to 00:00
int count_minutes(Time t) {
    return (t.hour * 60 + t.minute);
}

// convert a Time type variable to a string
string time_to_string(Time t) {
    stringstream ss;
    ss << setw(2) << setfill('0') << t.hour << ":" << setw(2) << setfill('0') << t.minute;
    return ss.str();
}

// aux function for table printing
string center(const string s, const int w) {
    stringstream ss, spaces;
    // count excess room to pad
    int padding = int(w - s.size());
    for (int i = 0; i<padding / 2; ++i) {
        spaces << " ";
    }
    // format with padding
    ss << spaces.str() << s << spaces.str();
    // if odd #, add 1 space
    if (padding>0 && padding % 2 != 0) {
        ss << " ";
    }
    return ss.str();
}

// print table
void print_table(Table& table, string table_name) {
    vector<string> attribute = table.get_attributes();
    
    int entry_width = 13;
    int line_width = int((entry_width + 2) * attribute.size());
    
    cout << endl << endl;
    cout << center(table_name, line_width) << endl;
    cout << string(line_width, '=') << endl;
    
    for (string entry : attribute) {
        cout << center(entry, entry_width) << " | ";
    }
    cout << endl;
    
    cout << string(line_width, '=') << endl;
    
    for (int row = 0; row < table.get_size(); row++) {
        Record record = table.return_record(row);
        for (int col = 0; col < attribute.size(); col++) {
            cout << center(record.get_record_entry(col), entry_width) << " | ";
        }
        cout << endl << std::string(line_width, '-');
        cout << endl;
    }
    
    cout << endl << endl;
    
}

