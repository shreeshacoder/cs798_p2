#ifndef NFSSERVER
#define NFSSERVER

#include <sys/stat.h>

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

using NfsProtocol::mkdir_request;
using NfsProtocol::rmdir_request;
using NfsProtocol::rename_request;
using NfsProtocol::c_response;
using NfsProtocol::NfsServer;


class serverImplementation final : public NfsServer::Service {

	private :

		std::string base;

	public :

		serverImplementation(std::string path);
		
		Status server_mkdir(ServerContext * context, const mkdir_request * request, c_response * response) override;
		Status server_rmdir(ServerContext * context, const rmdir_request * request, c_response * response) override;
		Status server_rename(ServerContext * context, const rename_request * request, c_response * response) override;
		Status server_open(ServerContext * context, const open_request * request, d_response * response) override;
		Status server_create(ServerContext * context, const create_request * request, d_response * response) override;



};

#endif 