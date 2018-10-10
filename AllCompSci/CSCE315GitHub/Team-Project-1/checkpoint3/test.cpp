// minor modification to the test case
// all catch block add reference to the error

#include <assert.h>
#include <iostream>
#include <stdexcept>

#include "Database.h"
#include "Record.h"
#include "Table.h"
#include "aux_function.h"

using namespace std;

int test_main() {
    
    
    
    /* ---------------- TESTS FOR RECORD CLASS ---------------- */
    
    cout << "Beginning tests for record class\n";
    
    // testing constructor with specified size
    Record r(5);
    assert(r.get_record_size() == 5);
    
    // checking that all entries default to the empty string
    for (int i = 0; i < r.get_record_size(); i++)
    {
        assert(r.get_record_entry(i) == "");
    }
    
    // checking that setting and getting an entry within the range of the record works
    string s = "test";
    for (int i = 0; i < r.get_record_size(); i++)
    {
        r.set_record_entry(i, s);
        assert(r.get_record_entry(i) == s);
    }
    
    // checking that an error is thrown when the index -1 is accessed
    try
    {
        r.get_record_entry(-1);
        cout << "Entry with index -1 accessed with no exceptions thrown\n";
    }
    catch (exception& e)
    {
        //        cerr << "Out of range error: " << e.what() << endl;
    }
    
    // checking that an error is thrown when the index 6 is accessed
    try
    {
        r.get_record_entry(6);
        cout << "Entry with index 6 accessed with no exceptions thrown\n";
    }
    catch (exception& e)
    {
        
    }
    
    // checking that entry with negative index cannot be written to
    try
    {
        r.set_record_entry(-1, s);
        cout << "Entry with index -1 written to with no exceptions thrown\n";
    }
    catch (exception& e)
    {
        
    }
    
    cout << "If no output was shown, record class is working as intended\n";
    
    /* ---------------- END TESTS FOR RECORD CLASS ---------------- */
    
    /*START TESTS FOR TABLE CLASS */
    
    Table emptyTable;
    vector<string> vectorFilledTable{ "Name" , "Age" };
    Table filledTable(vectorFilledTable);
    
    //ex. U1, U2, U3...
    filledTable.add_attribute("Classification");
    emptyTable.add_attribute("Name");
    filledTable.delete_attribute("Age");
    
    /*Create two tables, add/delete attributes.
     Should result in attributes matching the vectors*/
    vector<string> testVectorTable1{ "Name", "Classification" };
    vector<string> testVectorTable2{ "Name" };
    assert(filledTable.get_attributes() == testVectorTable1);
    if (filledTable.get_attributes() == testVectorTable1) cout << "filledTable.get_attributes passed\n";
    assert(emptyTable.get_attributes() == testVectorTable2);
    if (emptyTable.get_attributes() == testVectorTable2) cout << "emptyTable.get_attributes passed\n";
    /*Create records to work for table testing
     insert record into table and then test the size function matches up*/
    Record recordEmptyTable(1);
    recordEmptyTable.set_record_entry(0, "1");
    emptyTable.insert_record(recordEmptyTable);
    
    assert(filledTable.get_size() == 0);
    if (filledTable.get_size() == 0) cout << "filledTable.get_size passed\n";
    assert(emptyTable.get_size() == 1);
    if (emptyTable.get_size() == 1) cout << "emptyTable.get_size passed\n";
    
    //emptyTable has a record of size 1, should return the vector of size 1
    try
    {
        Record testTableRecord = emptyTable.return_record(0);
        assert(testTableRecord.get_record_size() == 1);
        if (testTableRecord.get_record_size() == 1) cout << "testTableRecord.get_record_size passed\n";
    }
    catch (exception& e) {
        
    }
    
    emptyTable.set_key("Name");
    
    //with only one record in emptyTable, these functions should return 1
    
    // *** MODIFICATION: the only attribute for emptyTable is "Name", changed all "Numbers" here to "Name" ***
    assert(emptyTable.count("Name") == 1);
    assert(emptyTable.max("Name") == "1");
    assert(emptyTable.min("Name") == "1");
    
    //try table count/min/max with several values
    vector<string> vectorFunctionTable{ "Classification", "Number" };
    Table functionTable(vectorFunctionTable);
    Record firstRecordFunction(2);
    firstRecordFunction.set_record_entry(0, "U2");
    firstRecordFunction.set_record_entry(1, "20");
    Record secondRecordFunction(2);
    secondRecordFunction.set_record_entry(0, "U4");
    secondRecordFunction.set_record_entry(1, "40");
    
    // *** MODIFICATION: team 45 forgot to insert the record in the table ***
    functionTable.insert_record(firstRecordFunction);
    functionTable.insert_record(secondRecordFunction);
    
    //Check with varying string
    assert(functionTable.count("Classification") == 2);
    assert(functionTable.max("Classification") == "U4");
    assert(functionTable.min("Classification") == "U2");
    
    //Check with varying int
    assert(functionTable.count("Number") == 2);
    assert(functionTable.max("Number") == "40");
    assert(functionTable.min("Number") == "20");
    
    //create 2 tables for each type of joining
    vector<string> initialCrossNat{ "Column1","Column2" };
    vector<string> secondCrossNat{ "Column1","Column3" };
    
    Record crossRecord1(2);
    crossRecord1.set_record_entry(0,"a");
    crossRecord1.set_record_entry(1,"b");
    
    Record crossRecord2(2);
    crossRecord2.set_record_entry(0,"a");
    crossRecord2.set_record_entry(1,"c");
    
    Table crossFirst(initialCrossNat);
    crossFirst.insert_record(crossRecord1);
    Table crossSecond(secondCrossNat);
    crossSecond.insert_record(crossRecord2);
    Table natFirst(initialCrossNat);
    natFirst.insert_record(crossRecord1);
    Table natSecond(secondCrossNat);
    natSecond.insert_record(crossRecord2);
    
    //Not sure how calling for the join table operations works
    //cross should not merge columns whereas natural SHOULD merge columns when matching
    // there should be no space for any attribute entry
    vector<string> crossTest{ "Column1", "Column2", "Column1", "Column3" };
    try {
        Table crossTable;
        crossTable.cross_join(crossFirst, crossSecond);
        assert(crossTable.get_attributes() == crossTest);
        assert(crossTable.get_size() == 1);
    }
    catch (exception e) {
        cout << "Something is wrong with cross_join()\n";
    }
    
    // CHANGE use new natTest
    vector<string> natTest{ "Column1", "Column2", "Column3" };
    
    // CHANGE: first set key, there is no multiple key
    natFirst.set_key("Column1");
    natSecond.set_key("Column1");
    try {
        Table natTable;
        natTable.natural_join(natFirst, natSecond);
        assert(natTable.get_attributes() == natTest);
        assert(natTable.get_size() == 1);
    }
    catch (exception e) {
        cout << "Something is wrong with natural join\n";
    }
    
    /*END TESTS FOR TABLE CLASS*/
    
    /***** START TESTS FOR DATABASE CLASS *****/
    Database d;
    Table t;
    
    // Create a test table
    d.add_table(t, "tbl");
    
    // Ensure list table returns a stirng
    // CHANGE: use typeid to ensure the type
    assert(typeid(d.list_table()) == typeid(vector<string>));
    
    // Ensure the first element in tableNames is "tbl"
    vector<string> tableNames = d.list_table();
    assert(tableNames.at(0) == "tbl");
    
    // Ensure the first element tables vector is t
    //assert(d.get_tables().at(0) == t);
    Table testDatabaseTable = d.get_tables().at(0);
    assert(testDatabaseTable.get_size() == t.get_size());
    assert(testDatabaseTable.get_attributes() == t.get_attributes());
    
    // Ensure get tables is empty after calling drop table
    d.drop_table("tbl");
    assert(d.get_tables().empty() == true);
    
    // Tests query function
    // Test table should have attributes: Country, State and City
    Table locations;
    locations.add_attribute("Country");
    locations.add_attribute("State");
    locations.add_attribute("City");
    
    Record queryTestRecord1(3);
    queryTestRecord1.set_record_entry(0,"United_States");
    queryTestRecord1.set_record_entry(1,"Texas");
    queryTestRecord1.set_record_entry(2,"Rockwall");
    
    Record queryTestRecord2(3);
    queryTestRecord2.set_record_entry(0,"Other");
    queryTestRecord2.set_record_entry(1,"a");
    queryTestRecord2.set_record_entry(2,"b");
    
    locations.insert_record(queryTestRecord1);
    locations.insert_record(queryTestRecord2);
    
    d.add_table(locations, "Locations");
    
    // Basic query to produce a table of locations in the United States
    //assert(d.Query("*", "Locations", "Country = United States") == Table());
    Table testDatabaseQuery = d.Query("*", "Locations", "Country = United_States");
    vector<string> vectorDatabaseQuery{ "Country","State","City" };
    assert(testDatabaseQuery.get_attributes() == vectorDatabaseQuery);
    assert(testDatabaseQuery.get_size() == 1);
    
    // test to make sure less than all of the record can be taken from a query
    Table testDatabaseQuery2 = d.Query("State", "Locations", "Country = United_States");
    vector<string> test2Attributes;
    test2Attributes.push_back("State");
    assert(testDatabaseQuery2.get_attributes() == test2Attributes);
    assert(testDatabaseQuery2.get_size() == 1);
    
    
    // Table to test if selecting a single attribute works
    Table testTable;
    testTable.add_attribute("Country");
    assert(d.Query("Country", "Locations", "Country = United_States").get_attributes().front() == testTable.get_attributes().front());
    
    /***** END TESTS FOR DATABASE CLASS *****/
    
    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
// Below are test cases I added by myself to fully test complex functions like the min, max, count functions of table class, cross join, natural join, query function
int test_min_max_count() {
    vector<string> v = {"time", "date", "value", "string"};
    Table t(v);
    Record r1(4);
    Record r2(4);
    Record r3(4);
    Record r4(4);

    r1.set_record_entry(0, "10:00");
    r2.set_record_entry(0, "08:00");
    r3.set_record_entry(0, "16:00");
    r4.set_record_entry(0, "04:00");

    r1.set_record_entry(1, "2000-01-01");
    r2.set_record_entry(1, "1998-01-01");
    r3.set_record_entry(1, "2017-05-06");
    r4.set_record_entry(1, "2017-05-23");

    r1.set_record_entry(2, "10");
    r2.set_record_entry(2, "10.8");
    r3.set_record_entry(2, "9.6");
    r4.set_record_entry(2, "11");

    r1.set_record_entry(3, "a");
    r2.set_record_entry(3, "b");
    r3.set_record_entry(3, "d");
    r4.set_record_entry(3, "c");

    t.insert_record(r1);
    t.insert_record(r2);
    t.insert_record(r3);
    t.insert_record(r4);

    assert(t.count("time") == 4);
    assert(t.max("time") == "16:00");
    assert(t.min("time") == "04:00");

    assert(t.count("date") == 4);
    assert(t.max("date") == "2017-05-23");
    assert(t.min("date") == "1998-01-01");

    assert(t.count("value") == 4);
    assert(t.max("value") == "11");
    assert(t.min("value") == "9.6");

    assert(t.count("string") == 4);
    assert(t.max("string") == "d");
    assert(t.min("string") == "a");

    cout << "count, min, max function test passed" << endl;
    return 0;
}

Table create_table_1() {
    vector<string> v = { "Name", "Age", "Qualify" };
    Table table(v);
    Record r1(3);
    Record r2(3);
    Record r3(3);
    Record r4(3);
    
    r1.set_record_entry(0, "John");
    r2.set_record_entry(0, "Alice");
    r3.set_record_entry(0, "Bob");
    r4.set_record_entry(0, "Claire");
    
    r1.set_record_entry(1, "13");
    r2.set_record_entry(1, "22");
    r3.set_record_entry(1, "34");
    r4.set_record_entry(1, "25");
    
    r1.set_record_entry(2, "Y");
    r2.set_record_entry(2, "N");
    r3.set_record_entry(2, "Y");
    r4.set_record_entry(2, "Y");
    
    table.insert_record(r1);
    table.insert_record(r2);
    table.insert_record(r3);
    table.insert_record(r4);
    
    table.set_key("Name");
    return table;
}

Table create_table_2() {
    vector<string> v = {"time", "date", "value", "string"};
    Table table(v);
    Record r1(4);
    Record r2(4);
    Record r3(4);
    Record r4(4);
    
    r1.set_record_entry(0, "10:00");
    r2.set_record_entry(0, "08:00");
    r3.set_record_entry(0, "16:00");
    r4.set_record_entry(0, "04:00");
    
    r1.set_record_entry(1, "2000-01-01");
    r2.set_record_entry(1, "1998-01-01");
    r3.set_record_entry(1, "2017-05-06");
    r4.set_record_entry(1, "2017-05-23");
    
    r1.set_record_entry(2, "10");
    r2.set_record_entry(2, "10.8");
    r3.set_record_entry(2, "9.6");
    r4.set_record_entry(2, "11");
    
    r1.set_record_entry(3, "a");
    r2.set_record_entry(3, "b");
    r3.set_record_entry(3, "d");
    r4.set_record_entry(3, "c");
    
    table.insert_record(r1);
    table.insert_record(r2);
    table.insert_record(r3);
    table.insert_record(r4);
    
    return table;
}

Table create_table_3() {
    vector<string> v = { "Gender", "Name", "State" };
    Table table(v);
    Record r1(3);
    Record r2(3);
    Record r3(3);
    Record r4(3);
    
    r1.set_record_entry(1, "John");
    r2.set_record_entry(1, "Alice");
    r3.set_record_entry(1, "Bob");
    r4.set_record_entry(1, "Claire");
    
    r1.set_record_entry(0, "M");
    r2.set_record_entry(0, "F");
    r3.set_record_entry(0, "M");
    r4.set_record_entry(0, "F");
    
    r1.set_record_entry(2, "NY");
    r2.set_record_entry(2, "CA");
    r3.set_record_entry(2, "DC");
    r4.set_record_entry(2, "TX");
    
    table.insert_record(r1);
    table.insert_record(r2);
    table.insert_record(r3);
    table.insert_record(r4);
    
    table.set_key("Name");
    return table;
}

void query_test() {
    string cnd_NAME_BIRTH = "Name <= John AND (Qualify==Y OR Age>= 25)";
    string cnd_NOT = "NOT (Qualify = Y)";
    
    Database database;
    Table test_table = create_table_1();
    
    database.add_table(test_table, "test_table");
    vector<string> list_table = database.list_table();
    
    string select_command = "Name,Qualify";
    string from_command = "test_table";
    string where_command = cnd_NOT;
    
    Table select_table = database.Query(select_command, from_command, where_command);
    
    print_table(test_table, (string)"Test Table");
    cout << "select command is " << select_command << endl;
    cout << "from command is " << from_command << endl;
    cout << "where command is " << where_command << endl;
    print_table(select_table, (string)"Select Table");
}

void cross_join_test() {
    Table table_1 = create_table_1();
    Table table_2 = create_table_2();
    Table cross_join_table;
    cross_join_table.cross_join(table_1, table_2);
    
    print_table(table_1, "Table 1");
    print_table(table_2, "Table 2");
    print_table(cross_join_table, "Cross Join Table");
}

void natural_join_test() {
    Table table_1 = create_table_1();
    Table table_2 = create_table_3();
    Table natural_join_table;
    natural_join_table.natural_join(table_1, table_2);
    
    print_table(table_1, "Table 1");
    print_table(table_2, "Table 2");
    print_table(natural_join_table, "Natural Join Table");
}

int main() {
    
    test_main();
    
    query_test();
    cross_join_test();
    natural_join_test();
    cout << "all tests passed" << endl;
    
}

