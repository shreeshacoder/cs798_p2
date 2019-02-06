
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "../proto/schema.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using NfsProtocol::mkdir_response;
using NfsProtocol::mkdir_request;
using NfsProtocol::NfsServer;


class NfsClient {
	public :
		NfsClient(std::shared_ptr<Channel> channel)
			: stub_(NfsServer::NewStub(channel)) {}

	void mkdir() {

	}

	

	private :
		std::unique_ptr<NfsServer::Stub> stub_;


};


int main() {

}


