#include "nfsServer.h"

void runServer()
{

	std::string server_address("0.0.0.0:3110");

	serverImplementation service("/home/shreesha/cs680");

	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(&service);
	std::unique_ptr<Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;
	server->Wait();
}

int main(int argc, char **argv)
{
	runServer();
	return 0;
}
