#ifndef NFSSERVER
#define NFSSERVER

#include <sys/stat.h>
#include <dirent.h>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "../proto/schema.grpc.pb.h"
#include "../utils/utils.h"

#include "../utils/utils.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using NfsProtocol::attribute_request_object;
using NfsProtocol::attribute_response_object;
using NfsProtocol::attributes;
using NfsProtocol::c_response;
using NfsProtocol::create_truncate_request;
using NfsProtocol::d_response;
using NfsProtocol::mkdir_request;
using NfsProtocol::NfsServer;
using NfsProtocol::open_request;
using NfsProtocol::read_directory_single_object;
using NfsProtocol::read_request;
using NfsProtocol::read_response;
using NfsProtocol::readdir_request;
using NfsProtocol::readdir_response;
using NfsProtocol::rename_request;
using NfsProtocol::rmdir_request;
using NfsProtocol::unlink_request;

class serverImplementation final : public NfsServer::Service
{

  private:
	std::string base;

  public:
	serverImplementation(std::string path);

	Status server_mkdir(ServerContext *context, const mkdir_request *request, c_response *response) override;
	Status server_rmdir(ServerContext *context, const rmdir_request *request, c_response *response) override;
	Status server_rename(ServerContext *context, const rename_request *request, c_response *response) override;
	Status server_open(ServerContext *context, const open_request *request, d_response *response) override;
	Status server_create(ServerContext *context, const create_truncate_request *request, d_response *response) override;
	Status server_truncate(ServerContext *context, const create_truncate_request *request, d_response *response) override;
	Status server_unlink(ServerContext *context, const unlink_request *request, c_response *response) override;
	Status server_read(ServerContext *context, const read_request *request, read_response *response) override;
	Status read_directory(ServerContext *context, const readdir_request *request,
						  readdir_response *response) override;

	Status get_attributes(ServerContext *context, const attribute_request_object *request,
						  attribute_response_object *response) override;
};

#endif