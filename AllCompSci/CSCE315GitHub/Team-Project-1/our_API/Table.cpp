//
// Table.cpp
// Created by Alex Kaiser on 9/21/2017.
// Team 44: Alex Kaiser, Dario Avendano, Suqian Wang
//
//asdf
#include "Table.h"

using namespace std;

Table::Table(vector<string> attribute_name): columns(attribute_name) {};

void Table::add_attribute(string attribute_name){
    this->columns.push_back(attribute_name);
}

void Table::delete_attribute(string attribute_name){
    for(int i = 0;i < this->columns.size(); i++){
		if(this->columns[i] == attribute_name){
			this->columns.erase(this->columns.begin() + i);
		}
	}
}

void Table::insert_record(Record record){
    this->rows.push_back(record);
}

vector<string> Table::get_attributes() const{
    return this->columns;
}

unsigned long int Table::get_size() const{
    return this->rows.size();
}

Record Table::return_record(int index){
    return this->rows.at(index);
}

void Table::set_key(string attribute_name){
	this->keys.push_back(attribute_name);
}

int Table::count(string attribute_name){
	int total = 0;
    for(int i = 0;i < this->columns.size(); i++){
		if(this->columns[i] == attribute_name){
			total = this->rows.size();
		}
	}
	return total;
}

string Table::max(string attribute_name){
    string max_string = " ";
	for(int i = 0;i < this->columns.size(); i++){
		if(this->columns[i] == attribute_name){
			for(int j = 0; j < this->rows.size(); j++){
				if(max_string == " "){
					max_string = this->rows[j].get_record_entry(i);
				}
				if(this->rows[j].get_record_entry(i) > max_string){
					max_string = this->rows[j].get_record_entry(i);
				}
			}
		}
	}
	return max_string;
}

string Table::min(string attribute_name){
    string min_string = " ";
	for(int i = 0;i < this->columns.size(); i++){
		if(this->columns[i] == attribute_name){
			for(int j = 0; j < this->rows.size(); j++){
				if(min_string == " "){
					min_string = this->rows[j].get_record_entry(i);
				}
				if(this->rows[j].get_record_entry(i) < min_string){
					min_string = this->rows[j].get_record_entry(i);
				}
			}
		}
	}
	return min_string;
}

void Table::set_columns(vector<string> columns_new){
	this->columns = columns_new;
}

void Table::set_rows(vector<Record> rows_new){
	this->rows = rows_new;
}

Table cross_join(Table table_1, Table table_2){
	Table temp;
	vector<string> columns_1 = table_1.columns_out();
	vector<string> columns_2 = table_1.columns_out();
	vector<Record> rows_1 = table_1.rows_out();
	vector<Record> rows_2 = table_2.rows_out();
    vector<string> new_columns;
	vector<Record> new_records(rows_1.size() * rows_2.size());
	for(int i = 0;i < columns_1.size(); i++){
		new_columns.push_back(columns_1[i]);
	}
	for(int i = 0;i < columns_2.size(); i++){
		new_columns.push_back(columns_2[i]);
	}
	
	int breaker = 0;
	int switch_rows_t1 = 0;
	int switch_rows_t2 = 0;
	while(breaker < (rows_1.size() * rows_2.size())){
		int total_record = rows_1[0].get_record_size() + rows_2[0].get_record_size();
		Record new_rec(total_record);
		for(int i = 0; i < rows_1[switch_rows_t1].get_record_size(); i++){
			new_rec.set_record_entry(i,rows_1[switch_rows_t1].get_record_entry(i));
		}
		int iterator_2 = 0;
		for(int j = rows_1[switch_rows_t1].get_record_size();j < new_rec.get_record_size(); j++){
			new_rec.set_record_entry(j,rows_2[switch_rows_t2].get_record_entry(iterator_2));
			iterator_2++;
		}
		
		if(switch_rows_t2 == rows_2.size() - 1){
			switch_rows_t1++;
			switch_rows_t2 = 0;
		}
		else{
			switch_rows_t2++;
		}
	new_records[breaker] = new_rec;
	breaker++;
	}
	
	temp.set_columns(new_columns);
	temp.set_rows(new_records);
    return temp;
}

Table natural_join(Table table_1, Table table_2){
	vector<string> columns_1 = table_1.columns_out();
	vector<string> columns_2 = table_1.columns_out();
	vector<Record> rows_1 = table_1.rows_out();
	vector<Record> rows_2 = table_2.rows_out();
	vector<string> keys_2 = table_2.keys_out();

	bool no_key = true;
	for(int i = 0; i < columns_1.size(); i++){
		for(int j = 0; j < keys_2.size(); j++){
			if(columns_1[i] == keys_2[j]){
				no_key = false;
			}
		}
	}
	if((keys_2.size() != 0) && (no_key == false)){
		Table temp;
		vector<string> new_columns;
		vector<Record> new_records(rows_1.size());
		for(int i = 0;i < columns_1.size(); i++){
			new_columns.push_back(columns_1[i]);
		}
		//Dont repeat attribute by finding key in second Table
		for(int i = 0;i < columns_2.size(); i++){
			bool not_in_vec = true;
			for(int j = 0; j < keys_2.size(); j++){
				if(keys_2[j] == columns_2[i]){
					not_in_vec = false;
				}
			}
			if(not_in_vec == true){
				new_columns.push_back(columns_2[i]);
			}
			
		}
		
		int breaker = 0;
		int switch_rows_t1 = 0;
		int switch_rows_t2 = -1;
		while(breaker < (rows_1.size())){
			int total_record = rows_1[0].get_record_size() + (rows_2[0].get_record_size() - keys_2.size());
			Record new_rec(total_record);
			for(int i = 0; i < rows_1[switch_rows_t1].get_record_size(); i++){
				new_rec.set_record_entry(i,rows_1[switch_rows_t1].get_record_entry(i));
			}
			int key_location = -1;
			int iterator_2 = 0;
			for(int j = rows_1[switch_rows_t1].get_record_size();j < new_rec.get_record_size(); j++){
				bool found = false;
				for(int h = 0; h < keys_2.size(); h++){
					if(columns_2[iterator_2] == keys_2[h]){
						key_location = iterator_2;
						found = true;
						goto skip;
					}
				}
				skip:
				if(found != true && key_location != -1){
					for(int h = 0; h < rows_2.size(); h++){
						if(rows_2[h].get_record_entry(key_location) == new_rec.get_record_entry(j - 1)){
							switch_rows_t2 = h;
						}
					}
					if(switch_rows_t2 != -1){
						new_rec.set_record_entry(j,rows_2[switch_rows_t2].get_record_entry(iterator_2));
					}
				}
				else{
					j--;
				}
				iterator_2++;
			}
			
		switch_rows_t1++;
		new_records[breaker] = new_rec;
		breaker++;
		}
		
		temp.set_columns(new_columns);
		temp.set_rows(new_records);
		return temp;
	}
	else{
		//cout << "Key Error!\n";
	}
}

