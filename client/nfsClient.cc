#include "nfsClient.h"

using NfsProtocol::mkdir_request;
using NfsProtocol::c_response;
using NfsProtocol::NfsServer;

clientImplementation::clientImplementation(std::shared_ptr<Channel> channel)
      : stub_(NfsServer::NewStub(channel)) {}


int clientImplementation::client_mkdir(std::string path, mode_t mode) {

	return 0;
}

int clientImplementation::client_rmdir(std::string path) {

	return 0;
}

int clientImplementation::client_rename(std::string from, std::string to) {

	return 0;
}

int clientImplementation::client_open(std::string path, struct fuse_file_info *fi) {

	return 0;
}

int clientImplementation::client_create(std::string path, mode_t mode, struct fuse_file_info *fi) {

	return 0;
}


