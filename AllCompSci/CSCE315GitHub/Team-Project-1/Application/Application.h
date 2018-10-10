//
//  Application.h
//  TP1_CP4
//
//  Created by Susan Wang on 10/16/17.
//  Copyright © 2017 Suqian Wang. All rights reserved.
//

#include "database.h"
#include "json.hpp"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#ifndef Application_h
#define Application_h

class Application {
private:
	//database database;

public:
	void import_json(string file_name);

	void combine_two_table(string table_1, string table_2);

	double compute_average(string table_name, string attribute);
	void compute_range(string table_name, string attribute);
	void count_records(string table_name);
	
	void display_by_table(string table_name);
	void display_a_record(string table_name, string record_key);

	// give summary information about all records in summary table concerned with one record 
	// specified by record_key in table "record_from"

	void display_summary(string from_table, string record_key, string summary_table);

	// business_table_name: the name business table recorded in database
	// business_key: the key specify the precise business record
	// summary_review_or_user: table_name, reviews or users.
	void display_summary_for_a_business(string business_table_name, string business_key, string summary_review_or_user);
    void display_compliments_for_a_business();
	//
};

#endif /* Application_h */

