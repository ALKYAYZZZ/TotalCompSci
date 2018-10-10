#ifndef database_h
#define database_h

#include "table.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string>

//Used for building the expression tree in query to be evaluated

struct Node {
    std::string payload;
    Node* left;
    Node* right;
    bool takeOpposite = false;
    Node(std::string data)
    {
        payload = data;
        left = NULL;
        right = NULL;
    }
    ~Node()
    {
        delete left;
        delete right;
    }
};

class database
{
private:
    std::map<std::string, Table> tables;
    
    //Support functions for Query to build the expression tree based on operator precedence
    //and evaluate it either lexicographically or numerically for each expression
    Node* constructExpTree(std::vector<std::string>& ops);
    std::vector<bool> eval(Node* r, Table& table);
    Table constructTable(std::vector<std::string> attr, std::vector<bool>& currentRecs, Table& tables);
    int precedence(std::string op);
    bool isOperator(std::string data);
    template<class T>
    std::vector<bool> evalIntOrString(std::vector<bool>& recs, std::string operation, T entry, T rhs, int index);
    
public:
    database();
    ~database();
    
    //modify database
    void addTable(Table& t1, std::string name);
    void deleteTable(std::string& name);
    
    //get database contents
    std::vector<std::string> listTables();
    std::vector<Table> getTables();
    
    Table query(std::vector<std::string> attr, std::string name, std::string where);
};

//Used to evaluate an expression in the expression tree either numerically or lexicographically
template<class T>
std::vector<bool> database::evalIntOrString(std::vector<bool>& recs, std::string operation, T entry, T rhs, int index)
{
    if (operation == "<") {
        if (entry < rhs)
        {
            recs.at(index) = 1;
        }
    }
    else if (operation == ">")
    {
        std::cout << "ENTRY: " << entry << " < " << "RHS:" << rhs << std::endl;
        if (entry > rhs)
        {
            recs.at(index) = 1;
        }
    }
    else if (operation == "=")
    {
        if (entry == rhs)
        {
            recs.at(index) = 1;
        }
    }
    else if (operation == "<=")
    {
        if (entry <= rhs)
        {
            recs.at(index) = 1;
        }
    }
    else if (operation == ">=")
    {
        if (entry >= rhs)
        {
            recs.at(index) = 1;
        }
    }
    else if (operation == "!=")
    {
        if (entry != rhs)
        {
            recs.at(index) = 1;
        }
    }
    return recs;
}

#endif /* database_hpp */
