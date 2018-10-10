//
//  Application.cpp
//  TP1_CP4
//
//  Created by Susan Wang on 10/16/17.
//  Copyright © 2017 Suqian Wang. All rights reserved.
//
#include "Application.h"

using nlohmann::json;

void insert_json(string file_name){
    ifstream total(file_name);
    string line;

   // while(getline(total,line)){
        istringstream iss(line);
        json line_j = json::parse(line);
        cout << line_j.at("business_id").get<string>();
    //}
}


int main(){
    insert_json("business.json");
}
