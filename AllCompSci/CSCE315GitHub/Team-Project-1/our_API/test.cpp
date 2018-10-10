#include "Database.h"
#include "Record.h"
#include "Table.h"
#include <assert.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <typeinfo>

using namespace std;

int main() {



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
	catch (exception e)
	{

	}

	// checking that an error is thrown when the index 6 is accessed
	try
	{
		r.get_record_entry(6);
		cout << "Entry with index 6 accessed with no exceptions thrown\n";
	}
	catch (exception e)
	{

	}

	// checking that entry with negative index cannot be written to
	try
	{
		r.set_record_entry(-1, s);
		cout << "Entry with index -1 written to with no exceptions thrown\n";
	}
	catch (exception e)
	{

	}

	cout << "If no output was shown, record class is working as intended\n";

	/* ---------------- END TESTS FOR RECORD CLASS ---------------- */

	/*START TESTS FOR TABLE CLASS */
	cout << "Table Tests"<<endl;
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
	catch (exception e) {

	}

	emptyTable.set_key("Name");

	//with only one record in emptyTable, these functions should return 1
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
	vector<string> initialCrossNat{ "Column 1","Column 2" };
	
	Record crossRecord1(2);
	crossRecord1.set_record_entry(0,"a");
	crossRecord1.set_record_entry(1,"b");
	
	Record crossRecord2(2);
	crossRecord2.set_record_entry(0,"a");
	crossRecord2.set_record_entry(1,"c");
	
	Table crossFirst(initialCrossNat);
	crossFirst.insert_record(crossRecord1);
	Table crossSecond(initialCrossNat);
	crossSecond.insert_record(crossRecord2);
	Table natFirst(initialCrossNat);
	natFirst.insert_record(crossRecord1);
	Table natSecond(initialCrossNat);
	natSecond.insert_record(crossRecord2);
	natSecond.set_key("Column 1");
	natSecond.set_key("Column 2");
	//Not sure how calling for the join table operations works
	//cross should not merge columns whereas natural SHOULD merge columns when matching
	
	vector<string> crossTest{ "Column 1", "Column 2", "Column 1", "Column 2" };
	try {
		Table crossTable = cross_join(crossFirst, crossSecond);
		assert(crossTable.get_attributes() == crossTest);
		assert(crossTable.get_size() == 1);
	}
	catch (exception e) {
		cout << "Something is wrong with cross_join()\n";
	}
	
	
	vector<string> natTest{ "Column 1", "Column 2" };
	try {
		Table natTable = natural_join(natFirst, natSecond);
		vector<string> get_stuff = natTable.get_attributes();
		assert(natTable.get_attributes() == natTest);
		assert(natTable.get_size() == 1);
	}
	catch (exception e) {
		cout << "Something is wrong with natural join\n";
	}
	

	/*END TESTS FOR TABLE CLASS*/
	
	/***** START TESTS FOR DATABASE CLASS *****/
    cout<<"Database Tests"<<endl;

	Database d;
	Table t;

	// Create a test table
	d.add_table(t, "tbl");

	// Ensure list table returns a stirng


	assert(typeid(decltype(d.list_table())) == typeid(decltype(vector<string>())));

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
	queryTestRecord1.set_record_entry(0,"United States");
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
	Table testDatabaseQuery = d.Query("*", "Locations", "Country = United States");
	vector<string> vectorDatabaseQuery{ "Country","State","City" };
	assert(testDatabaseQuery.get_attributes() == vectorDatabaseQuery);
	assert(testDatabaseQuery.get_size() == 1);

	// test to make sure less than all of the record can be taken from a query
	Table testDatabaseQuery2 = d.Query("State", "Locations", "Country = United States");
	vector<string> test2Attributes;
	test2Attributes.push_back("State");
	assert(testDatabaseQuery2.get_attributes() == test2Attributes);
	assert(testDatabaseQuery2.get_size() == 1);
	
	
	// Table to test if selecting a single attribute works
	Table testTable;
	testTable.add_attribute("Country");
	assert(d.Query("Country", "Locations", "Country = United States").get_attributes().front() == testTable.get_attributes().front());

	/***** END TESTS FOR DATABASE CLASS *****/
	system("pause");
	
	return 0;
}