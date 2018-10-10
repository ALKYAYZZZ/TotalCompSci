//

//
#include "table.h"
#include "database.h"
#include <string>
#include <assert.h>
#include <vector>


using namespace std;

void test_database(database &temp){

    Table *table1 = new Table();
    Table *table2 = new Table();
    Table *table3 = new Table();

    string name1;
    string name2;
    string name3;


    temp.addTable(*table1 , name1);
    //testing add function

    assert(temp.listTables()[0] == name1);
    assert(&temp.getTables()[0] == table1);

    temp.addTable(*table2 , name2);
    temp.addTable(*table3 , name3);

    temp.deleteTable(name2);
    //testing delete function

    assert(temp.listTables()[1] == name3);
    assert(&temp.getTables()[1] == table3);






}
int main(){

    Database db = new Database();
    test_database(db);
    
}
