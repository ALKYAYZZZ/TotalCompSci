#ifndef database_h
#define database_h

#include "table.h"
#include <string>
#include <vector> 
#include <map>

class database
{
	std::map<std::string, Table> tables;

public:

	database();
	~database();

	void addTable(Table& t1);
	void deleteTable(std::string& name);

	std::vector<std::string> listTables();
	std::vector<Table> getTables();

	Table query(std::vector<std::string> attr, std::string name, std::string where);
};

#endif /* database_hpp */
