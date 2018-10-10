#include "table.h"
#include "Record.h"
#include <assert.h>


void test_table(Table &temp){
	temp.addAttribute("Dog");
	temp.addAttribute("Cat");
	
	assert(temp.countEntries() == 2);
	Table temp2 = new Table("Boolean", "String", "Integer");
	
	vector<string> vec_string = temp2.getAttributes();
	for(int i = 0; i < 3; i++){
		assert(vec_string[i] == "Boolean" || "String" || "Integer");
	}
	
	Table temp3 = crossJoinTables(temp, temp2);
	
	vector<string> vec_string_2 = temp3.getAttributes();
	for(int i = 0; i < 5; i++){
		assert(vec_string_2[i] == "Boolean" || "String" || "Integer" || "Dog" || "Cat");
	}
	
}

void test_table_2(Table &temp){
	Record rec(3);
	rec[2] = "John";
	
	temp.insertRecord(rec);
	
	assert(temp.getSize() == 1);
	
	temp.addAttribute("Eleven");
	
	Table temp_table("One", "Two", "Three", "Four");
	
	temp_table.setKey("Numbers");
	
	assert(temp_table.countEntries("One") == 1);
	
	temp_table.addAttribute("Five");
	
	temp_table.deleteAttribute("Two");
	
	vector<string> list_of_attributes = temp_table.getAttributes();
	
	for(int i = 0; i < list_of_attributes.size(); i++){
		assert(list_of_attributes[i] != "Two");
	}
	
	temp_table.addAttribute("Numbers");
	
	Table natural = naturalJoinTables(temp, temp_table);
	
	vector<string> vec = natural.getAttributes();
	
	for(int i = 0; i < vec.size(); i++){
		assert(natural[i] == "One" || "Three" || "Four" || "Five" || "Numbers");
	}
}

int main(){
	Table temp = new Table();
	Table temp2 = new Table();
	
	test_table(temp);
	test_table_2(temp2);
}