/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpc++/grpc++.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;
using helloworld::Posting;

// 1. create stub instance
// 2. make a request and reply
// 3. call rpc function
void onetime_message_passing() {
	std::unique_ptr<Greeter::Stub> stub(Greeter::NewStub(
		grpc::CreateChannel(
			"0.0.0.0:3000", grpc::InsecureChannelCredentials()
		)
	));

	HelloRequest request;
	request.set_name("howdy");

	HelloReply reply;

	// Context for the client. It could be used to convey extra information to
	// the server and/or tweak certain RPC behaviors.
	ClientContext context;

	Status status = stub->SayHello(&context, request, &reply);

	// Act upon its status.
	if(status.ok()){
		std::cout << reply.message() << std::endl;
	}
	else{
		std::cout << status.error_code() << ": " << status.error_message() << std::endl;
		std::cout << "RPC failed";
	}
}

// 1. create stub
// 2. create stream
// 3. create two thread for reading and writing
void bidirectional_streaming_messages() {
	// Create Stub instance
	std::unique_ptr<Greeter::Stub> stub(Greeter::NewStub(
		grpc::CreateChannel(
			"localhost:3000", grpc::InsecureChannelCredentials()
		)
	));

	ClientContext context;

	std::shared_ptr<ClientReaderWriter<Posting, Posting>> stream(
			stub->BiDirectionalStream(&context));

	//Thread used to read chat messages and send them to the server
	std::thread writer([stream]() {
		std::string msg;
		while(true){
			std::cin >> msg;
			Posting p;
			p.set_content(msg);
			stream->Write(p);
		}
		stream->WritesDone();
	});

	std::thread reader([stream]() {
		Posting p;
		while(stream->Read(&p)){
			std::cout << p.content() << std::endl;
		}
	});

	//Wait for the threads to finish
	writer.join();
	reader.join();
}

int main(int argc, char** argv) {
	//onetime_message_passing();
	bidirectional_streaming_messages();
}