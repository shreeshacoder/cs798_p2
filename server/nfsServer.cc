
#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "../proto/schema.grpc.pb.h"
	
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using NfsProtocol::mkdir_response;
using NfsProtocol::mkdir_request;
using NfsProtocol::NfsServer;



class serverImplementation final : public NfsServer::Service {

	public :

		Status mkdir(ServerContext * context, const mkdir_request * request, mkdir_response * response) override {

			return Status::OK;
		}


};




void runServer() {
	
	std::string server_address("0.0.0.0:3110");
	
	serverImplementation service;

	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(&service);
	std::unique_ptr<Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;
	server->Wait();
}


int main(int argc, char** argv) {
	runServer();
	return 0;	
}


