//
//  bounded_buffer.cpp
//  
//
//  Created by Joshua Higginbotham on 11/4/15.
//
//

#include "bounded_buffer.h"
#include <string>
#include <queue>

bounded_buffer::bounded_buffer(int _capacity) {
	lock = new semaphore(1);
	full = new semaphore(0);
	empty = new semaphore(_capacity);
}

void bounded_buffer::push_back(std::string req) {
	empty->P();
	lock->P();
	response.push(req);
	lock->V();
	full->V();
}

std::string bounded_buffer::retrieve_front() {
	full->P();
	lock->P();
	string r = response.front();
	response.pop();
	lock->V();
	empty->V();
	return r;
}

int bounded_buffer::size() {
	return response.size();
}