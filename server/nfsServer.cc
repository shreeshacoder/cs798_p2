#include <sys/stat.h> 	// mkdir
#include <unistd.h>		// rmdir

#include "nfsServer.h"


serverImplementation::serverImplementation(std::string path) {
	Service();
	this->base = path;
}


Status serverImplementation::server_mkdir(ServerContext * context, const mkdir_request * request, c_response * response) {

	std::string newDir = this->base + request->dirfh() + request->name();
	mode_t mode = request->attr().st_mode();
	char * newDirChars;
	newDirChars = (char *)malloc(newDir.length() + 1);
	strcpy(newDirChars, newDir.c_str());

	if(mkdir(newDirChars, mode) != 0) {
		response->set_success(-1);
		response->set_ern(errno);
	}
	else {
		response->set_success(0);
		response->set_ern(0);	
	}
	return 	Status::OK;	

}

Status serverImplementation::server_rmdir(ServerContext * context, const rmdir_request * request, c_response * response) {

	std::string dirToRemove = this->base + request->dirfh();
	char * dirToRemoveChars;
	dirToRemoveChars = (char *)malloc(dirToRemove.length() + 1);
	strcpy(dirToRemoveChars, dirToRemove.c_str());

	if(rmdir(dirToRemoveChars	) != 0) {
		response->set_success(-1);
		response->set_ern(errno);
	}
	else {
		response->set_success(0);
		response->set_ern(0);	
	}
	return 	Status::OK;	

}

Status serverImplementation::server_rename(ServerContext * context, const rename_request * request, c_response * response) {

	std::string toDir = this->base + request->todir();
	char * toDirChars;
	toDirChars = (char *)malloc(toDir.length() + 1);
	strcpy(toDirChars, toDir.c_str());

	std::string fromDir = this->base + request->fromdir();
	char * fromDirChars;
	fromDirChars = (char *)malloc(fromDir.length() + 1);
	strcpy(fromDirChars, fromDir.c_str());

	if(rename(fromDirChars, toDirChars) != 0) {
		response->set_success(-1);
		response->set_ern(errno);
	}
	else {
		response->set_success(0);
		response->set_ern(0);	
	}
	return 	Status::OK;	

}



