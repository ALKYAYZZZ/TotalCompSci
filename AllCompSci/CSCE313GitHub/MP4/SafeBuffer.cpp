//
//  SafeBuffer.cpp
//
//
//  Created by Joshua Higginbotham on 11/4/15.
//
//

#include "SafeBuffer.h"
#include <string>
#include <queue>


SafeBuffer::SafeBuffer(int newsize) {
	lock = new semaphore(1);
	full = new semaphore(0);
	empty = new semaphore(newsize);
}

SafeBuffer::~SafeBuffer() {
	delete lock;
	delete full;
	delete empty;
}

int SafeBuffer::size() {
    return response.size();
}

/*
SafeBuffer & SafeBuffer::operator=(SafeBuffer &safe){
	lock = new semaphore(safe.lock);
	full = new semaphore(safe.full);
	empty = new semaphore(safe.empty);
	return *this;
}
*/

void SafeBuffer::push_back(std::string str) {
	empty->P();
	lock->P();
	response.push(str);
	lock->V();
	full->V();
}

std::string SafeBuffer::retrieve_front() {
	full->P();
	lock->P();
	string r = response.front();
	response.pop();
	lock->V();
	empty->V();
	return r;
}
