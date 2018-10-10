//
// Table.cpp
// Created by Suqian Wang on 10/11/2017.
// Team 44
//

#include <stdexcept>
#include <map>

#include "Table.h"
#include "aux_function.h"

// create an empty table with a list of attributes name
Table::Table(vector<string> attribute_name): columns(attribute_name) {};

// add a column to the end of the table with new attribute and NULL entries
void Table::add_attribute(string attribute_name){
    // if the attribute already exist, ignore this operation
    for (int i = 0; i < this->columns.size(); i++) {
        if (this->columns.at(i) == attribute_name) {
            throw runtime_error("attribute already exist");
        }
    }
    this->columns.push_back(attribute_name);
    for (int i = 0; i < this->rows.size(); i++) {
        this->rows.at(i).set_record_entry(int(this->columns.size()-1), NULL);
    }
}

// delete a column of the table with the given attribute name
void Table::delete_attribute(string attribute_name){
    for (int i = 0;i < this->columns.size(); i++){
        if(this->columns.at(i) == attribute_name){
            this->columns.erase(this->columns.begin()+i);
            for (int j = 0; j < this->rows.size(); j++) {
                this->rows.erase(this->rows.begin()+i);
            }
            return;
        }
    }
    // if the attribute does not exist, throw an error
    throw runtime_error("attribute does not exist");
}

// add a record to the table
void Table::insert_record(Record record){
    this->rows.push_back(record);
}

// returns a list of the attributes for the table in order
vector<string> Table::get_attributes() const{
    return this->columns;
}

// returns the number of records in the table
unsigned long int Table::get_size() const{
    return this->rows.size();
}

// returns the 'index'th record of the table
Record Table::return_record(int index){
    if ((index < 0) || (index > this->rows.size()-1)) {
        throw out_of_range("this record does not exist");
    }
    return this->rows.at(index);
}

// set an attribute of the table as a key
void Table::set_key(string attribute_name){
    for (int i = 0; i < this->columns.size(); i++) {
        if (this->columns.at(i) == attribute_name) {
            this->key = attribute_name;
            return;
        }
    }
    throw runtime_error("attribute does not exist");
}

// counts non-null entries of a specific column
int Table::count(string attribute_name){
    int null_entries = 0;
    for (int i = 0;i < this->columns.size(); i++){
        if (this->columns.at(i) == attribute_name){
            for (int j = 0; j < this->rows.size(); j++) {
                if (this->rows.at(j).get_record_entry(i).empty()) {
                    null_entries++;
                }
            }
            return (int(this->rows.size()) - null_entries);
        }
    }
    throw runtime_error("attribute does not exist");
}

// find the max entry
string Table::max(string attribute_name){
    double dmax = 0.0;
    Date date_max {0, 0, 0};
    Time tmax = {0, 0};
    string smax = "";
    for (int i = 0; i < this->columns.size(); i++) {
        if (this->columns.at(i) == attribute_name) {
            string first_entry = this->rows.at(0).get_record_entry(i);
            // map the type of the value of this attribute to an integer
            int type_code = -1;
            if (is_number(first_entry) == true) {
                type_code = 0;
            }
            if (is_date(first_entry) == true) {
                type_code = 1;
            }
            if (is_time(first_entry) == true) {
                type_code = 2;
            }
            // compare every entry of the column and find the max entry
            for (int j = 0; j < this->rows.size(); j++) {
                switch(type_code) {
                    case(0): {// type is number
                        double d_entry = stod(rows.at(j).get_record_entry(i).c_str());
                        if (d_entry > dmax) {
                            dmax = d_entry;
                        }
                        break;
                    }
                    case(1): {// type is date
                        Date date_entry = convert_to_date(rows.at(j).get_record_entry(i));
                        if (count_days(date_entry) > count_days(date_max)) {
                            date_max = date_entry;
                        }
                        break;
                    }
                    case(2): {// type is time
                        Time t_entry = convert_to_time(rows.at(j).get_record_entry(i));
                        if (count_minutes(t_entry) > count_minutes(tmax)) {
                            tmax = t_entry;
                        }
                        break;
                    }
                    default: {// type is string
                        string s_entry = rows.at(j).get_record_entry(i);
                        if (s_entry > smax) {
                            smax = s_entry;
                        }
                        break;
                    }
                }

            }
            // return the max entry in a form of string
            switch(type_code) {
                case(0): {
                    return convert_to_string(dmax);
                }
                case(1): {
                    return date_to_string(date_max);
                }
                case(2): {
                    return time_to_string(tmax);
                }
                default: {
                    return smax;
                }
            }
        }
    }
    throw runtime_error("attribute does not exist");
}

// find the min entry
string Table::min(string attribute_name){
    double dmin = 1.7e+308;
    Date date_min {3000, 0, 0};
    Time tmin = {23, 59};
    string smin = "zzzzzzzzzzzzzzz";
    for (int i = 0; i < this->columns.size(); i++) {
        if (this->columns.at(i) == attribute_name) {
            string first_entry = this->rows.at(0).get_record_entry(i);
            // map the type of the value of this attribute to an integer
            int type_code = -1;
            if (is_number(first_entry) == true) {
                type_code = 0;
            }
            if (is_date(first_entry) == true) {
                type_code = 1;
            }
            if (is_time(first_entry) == true) {
                type_code = 2;
            }
            // compare every entry of the column and find the max entry
            for (int j = 0; j < this->rows.size(); j++) {
                switch(type_code) {
                    case(0): {// type is number
                        double d_entry = stod(rows.at(j).get_record_entry(i).c_str());
                        if (d_entry < dmin) {
                            dmin = d_entry;
                        }
                        break;
                    }
                    case(1): {// type is date
                        Date date_entry = convert_to_date(rows.at(j).get_record_entry(i));
                        if (count_days(date_entry) < count_days(date_min)) {
                            date_min = date_entry;
                        }
                        break;
                    }
                    case(2): {// type is time
                        Time t_entry = convert_to_time(rows.at(j).get_record_entry(i));
                        if (count_minutes(t_entry) < count_minutes(tmin)) {
                            tmin = t_entry;
                        }
                        break;
                    }
                    default: {// type is string
                        string s_entry = rows.at(j).get_record_entry(i);
                        if (s_entry < smin) {
                            smin = s_entry;
                        }
                        break;
                    }
                }
                
            }
            // return the max entry in a form of string
            switch(type_code) {
                case(0): {
                    return convert_to_string(dmin);
                }
                case(1): {
                    return date_to_string(date_min);
                }
                case(2): {
                    return time_to_string(tmin);
                }
                default: {
                    return smin;
                }
            }
        }
    }
    throw runtime_error("attribute does not exist");
}

Table Table::cross_join(Table& table_1, Table& table_2) {
    // new table with attributes in both table_1 and table_2
    // attributes in table_1 will be prior to attributes to table_2
    
    vector<string> attributes_1 = table_1.get_attributes();
    vector<string> attributes_2 = table_2.get_attributes();
    vector<string> join_attributes = attributes_1;
    
    join_attributes.insert(join_attributes.end(), attributes_2.begin(), attributes_2.end());
    
    Table cj_table(join_attributes);

    int size_1 = (int)table_1.get_size();
    int size_2 = (int)table_2.get_size();
    
    int attri_size_1 = int(attributes_1.size());
    
    Record record_1;
    Record record_2;
    Record join_record(int(join_attributes.size()));
    
    for (int j = 0; j < size_2; j++) {
        record_2 = table_2.return_record(j);
        
        // create a record with table_1 attributes number of empty entry and one of table_2 record append at the end
        for (int attri_index = attri_size_1; attri_index < join_attributes.size(); attri_index++) {
            join_record.set_record_entry(attri_index, record_2.get_record_entry(attri_index - attri_size_1));
        }
        
        // loop through table_1 records, fill in the empty entries of join record and insert into the join table
        for (int i = 0; i < size_1; i++) {
            record_1 = table_1.return_record(i);
            for (int attri_index = 0; attri_index < attri_size_1; attri_index++) {
                join_record.set_record_entry(attri_index, record_1.get_record_entry(attri_index));
            }
            cj_table.insert_record(join_record);
        }
    }
    
    (*this) = cj_table;
    return (*this);
}

Table Table::natural_join(Table& table_1, Table& table_2) {
    map<string, Record> that_map;
    Record record_2;
    string key = table_2.get_key();
    
    int size_1 = (int)table_1.get_size();
    
    vector<string> attributes_1 = table_1.get_attributes();
    vector<string> attributes_2 = table_2.get_attributes();
    vector<string> join_attribute;
    
    join_attribute.push_back(key);
    join_attribute.insert(join_attribute.end(), attributes_1.begin(), attributes_1.end());
    join_attribute.insert(join_attribute.end(), attributes_2.begin(), attributes_2.end());
    
    int attri_size_1 = int(attributes_1.size());
    int attri_size_2 = int(attributes_2.size());
    int attri_size_join = attri_size_1 + attri_size_2 - 1;
    
    int pos_1 = int(find(attributes_1.begin(), attributes_1.end(), key) - attributes_1.begin());
    int pos_2 = int(find(attributes_2.begin(), attributes_2.end(), key) - attributes_2.begin());
    
    join_attribute.erase(join_attribute.begin() + pos_1 + 1);
    join_attribute.erase(join_attribute.begin() + attri_size_1 + pos_2);
    Table nj_table(join_attribute);
    nj_table.set_key(key);
    
    for (int i = 0; i < table_2.get_size(); i++) {
        record_2 = table_2.return_record(i);
        that_map[record_2.get_record_entry(pos_2)] = record_2;
    }
    
    Record new_record(attri_size_1 + attri_size_2 - 1);
    Record record_1;
    for (int row = 0; row < size_1; row++) {
        record_1 = table_1.return_record(row);
        string key_value = record_1.get_record_entry(pos_1);
        record_2 = that_map[key_value];
        
        for (int col = 1; col < pos_1; col++) {
            new_record.set_record_entry(col, key_value);
        }
        new_record.set_record_entry(0, record_1.get_record_entry(pos_1));
        for (int col = pos_1 + 1; col < attri_size_1; col++) {
            new_record.set_record_entry(col, record_1.get_record_entry(col));
        }
        
        for (int col = attri_size_1; col < attri_size_1 + pos_2; col++) {
            new_record.set_record_entry(col, record_2.get_record_entry(col - attri_size_1));
        }
        for (int col = attri_size_1 + pos_2; col < attri_size_join; col++) {
            new_record.set_record_entry(col, record_2.get_record_entry(col - attri_size_1 + 1));
        }
        nj_table.insert_record(new_record);
    }
    
    (*this) = nj_table;
    return (*this);
}

