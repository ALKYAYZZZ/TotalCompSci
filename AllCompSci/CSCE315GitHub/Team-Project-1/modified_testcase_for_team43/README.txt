The original test case couldn't compile

reason: 
1. The addTable function in database class should take two attributes, a table and a table name according to the project instruction.
Their header file and your API documentation only have one. 
2. Regarding constructor of table class that is suppose to take a list of attributes name
 They used "int n_args, ..." which is not allowed to.
 
 Database_test_modified.cpp was created by Suqian Wang according to their modified header files and LIB file
