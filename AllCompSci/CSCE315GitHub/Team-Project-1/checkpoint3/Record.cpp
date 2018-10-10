//
// Record.cpp
// Created by Suqian Wang on 9/22/2017.
// Team 44
// Modified by Suqian Wang on 10/11/2017
//

#include "Record.h"
#include <stdexcept>

Record::Record(int record_size) {
    this->record_size_ = record_size;
    for (int i = 0; i < record_size; i++) {
        this->vec_record_.push_back("");
    }
}

int Record::get_record_size() const {
    return this->record_size_;
}

string Record::get_record_entry(int index) const {
    if ((index > (this->record_size_ - 1)) || (index < 0)) {
        throw out_of_range("entry does not exist");
    }
    return this->vec_record_.at(index);
}

void Record::set_record_entry(int index, string record_entry) {
    // if the entry is out of range, create enough entries with empty string and append the given entry at the end
    if (index > (this->record_size_ - 1)) {
        for (int i = this->record_size_; i < index; i++) {
            this->vec_record_.push_back("");
        }
        this->vec_record_.push_back(record_entry);
        this->record_size_ = index + 1;
    }
    else {
        this->vec_record_.at(index) = record_entry;
    }
}
