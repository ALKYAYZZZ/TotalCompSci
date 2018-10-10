
#ifndef table_h
#define table_h

#include "Record.h"
#include <map>
#include <vector>
#include <string>
#include <cstdarg>

class Table
{
	std::map<int, Record> records;
	std::vector<std::string> keys;
	int size = 0;

public:
	Table();

	Table(int n_args, ...);
	~Table();

	void addAttribute(std::string s);
	void deleteAttribute(std::string s);
	void insertRecord(Record& r);

	std::vector<std::string> getAttributes();
	unsigned int getSize();
	std::map<int, Record>::iterator recordsBegin();
	std::map<int, Record>::iterator recordsEnd();
	void setKey(const std::string& _key);

	static Table crossJoinTables(Table& t1, Table& t2);
	static Table naturalJoinTables(Table& t1, Table& t2);

	int countEntries(const std::string& attr);
	std::string min(const std::string& attr);
	std::string max(const std::string& attr);
};
#endif /* table_hpp */



