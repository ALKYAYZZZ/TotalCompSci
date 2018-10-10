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
#include <grpc++/grpc++.h>
#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;
using helloworld::Posting;


// 1. create concrete class of service
// 2. override the function defined by proto
// 3. create instance of the class and register
// 4. run server

// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public Greeter::Service {
	// callback function
	Status SayHello(ServerContext* context, const HelloRequest* request, 
		HelloReply* reply) override {

		std::string name = request->name();
		std::cout << "got a message from client: " << name << std::endl;

		std::string return_message = "Hello " + name;
		reply->set_message(return_message);
		std::cout << "returning a message to client: " << return_message << std::endl;

		return Status::OK;
	}

	// callback function
	Status BiDirectionalStream(ServerContext* context,
			ServerReaderWriter<Posting, Posting>* stream) override {

		Posting p;
		while(stream->Read(&p)) {
			std::string msg = p.content();
			std::cout << "got a message from client: " << msg << std::endl;

			Posting new_posting;
			new_posting.set_content(msg + " from server");
			std::cout << "returning a message to client: " << new_posting.content() << std::endl;

			stream->Write(new_posting);
		}

		return Status::OK;
	}
};

int main(int argc, char** argv) {

	// create server by registering your service and address info
	ServerBuilder builder;
	builder.AddListeningPort("localhost:3010", grpc::InsecureServerCredentials());

	GreeterServiceImpl service;
	builder.RegisterService(&service);

	// Finally we can assemble the server.
	std::unique_ptr<Server> server(builder.BuildAndStart());

	std::cout << "Server listening on " << "localhost:3010" << std::endl;

	// Wait for the server to shutdown.
	server->Wait();
}
