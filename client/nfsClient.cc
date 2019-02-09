#include "nfsClient.h"

using NfsProtocol::mkdir_request;
using NfsProtocol::c_response;
using NfsProtocol::NfsServer;

clientImplementation::clientImplementation(std::shared_ptr<Channel> channel)
      : stub_(NfsServer::NewStub(channel)) {}


int clientImplementation::client_mkdir(std::string path, mode_t mode) {

	// mkdir_request request;
	// c_response response;
	// ClientContext context;
	
	// request.set_path(path);
	// request.set_mode(mode);

	// std::cout << "calling mkdir: " << std::endl;
	// Status status = stub_->server_mkdir(&context, request, &response);

	// if(status.ok()){
	// 	if(response.success() != 0)
	// 		return (- response.ern());
	// 	return 0;
	// }
	// else {
	// 	std::cout << status.error_code() << ": " << status.error_message() << std::endl;
 //    	return -1;
	// }

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


