//
//  Database_test.cpp
//  test
//
//  Created by Suqian Wang on 10/1/17.
//

#include "table.h"
#include "database.h"
#include "Record.h"
#include <string>
#include <assert.h>
#include <vector>

using namespace std;

Record test_record_1();
Record test_record_2();
Record test_record_3();
Record test_record_4();
void test_cross_natural_join();
Record test_record_no_name();
bool is_equal_reocrd(Record& record_1, Record& record_2);

Table create_table_1() {
    Table table;
    
    table.addAttribute("Make");
    table.addAttribute("Model");
    table.addAttribute("Seats");
    table.addAttribute("Paint");
    
    Record r_1(4);
    r_1[0] = "Honda";
    r_1[1] = "Odyssey";
    r_1[2] = "Cloth";
    r_1[3] = "Standard";
    
    Record r_2(4);
    r_2[0] = "Dodge";
    r_2[1] = "Caravan";
    r_2[2] = "Leather";
    r_2[3] = "Standard";
    
    table.insertRecord(r_1);
    table.insertRecord(r_2);
    
    return table;
}

Table create_table_2() {
    Table table;
    
    table.addAttribute("Name");
    table.addAttribute("Party");
    table.addAttribute("State");
    table.addAttribute("YearsInCongress");
    table.addAttribute("Trivial");
    
    Record record_1 = test_record_1();
    table.insertRecord(record_1);
    
    Record record_2 = test_record_2();
    table.insertRecord(record_2);
    
    Record record_3 = test_record_3();
    table.insertRecord(record_3);
    
    Record record_4 = test_record_4();
    table.insertRecord(record_4);
    
    return table;
}

void test_table() {
    Table table;
    
    // add attrbute
    table.addAttribute("Name");
    table.addAttribute("Party");
    table.addAttribute("State");
    table.addAttribute("YearsInCongress");
    table.addAttribute("Trivial");
    
    vector<string> attribute_list = table.getAttributes();
    for (int i = 0; i < 4; i++) {
        assert(attribute_list.at(i) == "Name" || "Party" || "State" || "YearsInCongress" || "Trivial" && "Failed");
    }
    
    table.deleteAttribute("Trivial");
    
    attribute_list = table.getAttributes();
    for (int i = 0; i < 4; i++) {
        assert(attribute_list.at(i) == "Name" || "Party" || "State" || "YearsInCongress" && "Failed");
    }
    
    Record record_1 = test_record_1();
    table.insertRecord(record_1);
    
    Record record_2 = test_record_2();
    table.insertRecord(record_2);
    
    Record record_3 = test_record_3();
    table.insertRecord(record_3);
    
    Record record_4 = test_record_4();
    table.insertRecord(record_4);
    
    Record record_no_name = test_record_no_name();
    table.insertRecord(record_no_name);
    
    // test count entries and amount of records
    assert(table.countEntries("Name") == 4 && "Failed");
    assert(table.countEntries("Party") == 5 && "Failed");
    assert(table.getSize() == 5 && "Failed");
    
    // test min and max from one specific attribute
    string test_min = table.min("YearsInCongress");
    string test_max = table.max("YearsInCongress");
    assert(test_min == "0" && "Failed");
    assert(test_max == "20" && "Failed");
    
    table.setKey("Name");
    
    table.deleteAttribute("YearsInCongress");
    
    // test iterator
    map<int, Record>::iterator begin_iter = table.recordsBegin();
    map<int, Record>::iterator end_iter= table.recordsEnd();
    
    assert(is_equal_reocrd(begin_iter->second, record_1) && "Failed");
    assert(is_equal_reocrd(end_iter->second, record_no_name) && "Failed");
    
    // test cross join and natural join
    test_cross_natural_join();
}

void test_database() {
    database db;
    
    Table table_1 = create_table_1();
    Table table_2 = create_table_2();
    
    db.addTable(table_1);
    db.addTable(table_2);
    
    vector<string> list_table = db.listTables();
    assert(list_table.size() == 2 && "Failed");
    
    // TODO: there is no "name" filed in Table class, this cause dabtabase with no name to search
    string name = "table_1";
    db.deleteTable(name);
    
    assert(list_table.size() == 1 && "Failed");
    
    std::vector<Table> get_table = db.getTables();
    
    assert(get_table.size() == 1 && "Failed");
    
    
    // test query
    vector<string> attributes;
    attributes.push_back("Party");
    attributes.push_back("State");
    attributes.push_back("YearsInCongress");
    string where = "YearsInCongress < 15";
    Table q_table = db.query(attributes, "table_1", where);
    
    assert(q_table.getAttributes().size() == 3 && "Failed");
    assert(q_table.getSize() == 3 && "Failed");
    
    
}

void test_cross_natural_join() {
    Table table_1;
    table_1.addAttribute("Make");
    table_1.addAttribute("Model");
    table_1.addAttribute("Seats");
    
    Record r_1(3);
    r_1[0] = "Honda";
    r_1[1] = "Odyssey";
    r_1[2] = "Cloth";
    
    Record r_2(3);
    r_2[0] = "Dodge";
    r_2[1] = "Caravan";
    r_2[2] = "Leather";
    
    table_1.insertRecord(r_1);
    table_1.insertRecord(r_2);
    
    Table table_2;
    table_2.addAttribute("Model");
    table_2.addAttribute("Seats");
    table_2.addAttribute("Paint");
    
    Record r_3(3);
    r_2[0] = "Caravan";
    r_3[1] = "Cloth";
    r_3[2] = "Standard";
    
    Record r_4(3);
    r_1[0] = "Odyssey";
    r_4[1] = "Leather";
    r_4[2] = "Standard";
    
    table_2.insertRecord(r_3);
    table_2.insertRecord(r_4);
    
    
    // test table cross join and natural join
    Table cross_join_table = table_1.crossJoinTables(table_1, table_2);
    Table natural_join_table = table_1.naturalJoinTables(table_1, table_2);
    
    // cross join table will have four attributes
    vector<string> attribute_list = cross_join_table.getAttributes();
    for (int i = 0; i < 4; i++) {
        assert(attribute_list.at(i) == "Dodge" || "Caravan" || "Leather" || "Standard" && "Failed");
    }
    
    // natural join table will have two attributes
    attribute_list = natural_join_table.getAttributes();
    for (int i = 0; i < 2; i++) {
        assert(attribute_list.at(i) == "Caravan" || "Leather" && "Failed");
    }
}

// helper function
Record test_record_1() {
    Record record(4);
    assert(record.getSize() == 4 && "Failed");
    for (int i = 0; i < record.getSize(); i++) {
        assert(record[i].empty() && "Failed");
    }
    record[0] = "Jill Smith";
    record[1] = "Republican";
    record[2] = "NY";
    record[3] = "5";
    
    assert(record[1] == "Jill Smith" && "Failed");
    assert(record[3] == "5" && "Failed");
    
    return record;
}

Record test_record_2() {
    Record record(4);
    
    record[0] = "Joe Adams";
    record[1] = "Democrat";
    record[2] = "NJ";
    record[3] = "0";
    
    assert(record[2] == "NJ" && "Failed");
    
    return record;
}

Record test_record_3() {
    Record record(4);
    
    record[0] = "Sue Jones";
    record[1] = "Democrat";
    record[2] = "CT";
    record[3] = "9";
    return record;
}

Record test_record_4() {
    Record record(4);
    
    record[0] = "Jim Brown";
    record[1] = "Republican";
    record[2] = "PA";
    record[3] = "15";
    return record;
}

Record test_record_no_name() {
    Record record(3);
    record[0] = "Democrat";
    record[1] = "TX";
    record[2] = "20";
    return record;
}

bool is_equal_reocrd(Record& record_1, Record& record_2) {
    if (record_1.getSize() != record_2.getSize()) {
        return false;
    }
    
    for (int i = 0; i < record_1.getSize(); i++) {
        if (record_1[i] != record_2[i])
            return false;
    }
    
    return true;
}


int main(){
    test_table();
}
