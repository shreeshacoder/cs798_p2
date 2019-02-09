#include <sys/stat.h>
#include <unistd.h>

#include "nfsServer.h"

serverImplementation::serverImplementation(std::string path)
{
	Service();
	this->base = path;
}

Status serverImplementation::server_mkdir(ServerContext *context, const mkdir_request *request, c_response *response)
{

	std::string newDir = this->base + request->dirfh() + request->name();
	mode_t mode = request->attr().st_mode();
	char *newDirChars;
	newDirChars = (char *)malloc(newDir.length() + 1);
	strcpy(newDirChars, newDir.c_str());

	if (mkdir(newDirChars, mode) != 0)
	{
		response->set_success(-1);
		response->set_ern(errno);
	}
	else
	{
		response->set_success(0);
		response->set_ern(0);
	}
	return Status::OK;
}

Status serverImplementation::server_rmdir(ServerContext *context, const rmdir_request *request, c_response *response)
{

	std::string dirToRemove = this->base + request->dirfh();
	char *dirToRemoveChars;
	dirToRemoveChars = (char *)malloc(dirToRemove.length() + 1);
	strcpy(dirToRemoveChars, dirToRemove.c_str());

	if (rmdir(dirToRemoveChars) != 0)
	{
		response->set_success(-1);
		response->set_ern(errno);
	}
	else
	{
		response->set_success(0);
		response->set_ern(0);
	}
	return Status::OK;
}

Status serverImplementation::server_rename(ServerContext *context, const rename_request *request, c_response *response)
{

	std::string toDir = this->base + request->todir();
	char *toDirChars;
	toDirChars = (char *)malloc(toDir.length() + 1);
	strcpy(toDirChars, toDir.c_str());

	std::string fromDir = this->base + request->fromdir();
	char *fromDirChars;
	fromDirChars = (char *)malloc(fromDir.length() + 1);
	strcpy(fromDirChars, fromDir.c_str());

	if (rename(fromDirChars, toDirChars) != 0)
	{
		response->set_success(-1);
		response->set_ern(errno);
	}
	else
	{
		response->set_success(0);
		response->set_ern(0);
	}
	return Status::OK;
}

Status server_open(ServerContext *context, const open_request *request, d_response *response)
{
}

Status server_create(ServerContext *context, const create_request *request, d_response *response)
{
}

Status serverImplementation::read_directory(ServerContext *context, const readdir_request *request,
											readdir_response *response)
{

	DIR *dp;
	struct dirent *de;
	if (LOG)
		std::cout << "------------------------------------------------\n";
	if (LOG)
		std::cout << "ReadDirectory : path passed - " << request->path() << "\n";
	std::string adjustedPath = mountpoint + request->path();
	char *path = new char[adjustedPath.length() + 1];
	strcpy(path, adjustedPath.c_str());

	dp = opendir(path);
	if (dp == NULL)
	{
		if (LOG)
			std::cout << "ReadDirectory : Error getting directory path -  " << errno << "\n";
		response->set_status(-errno);
	}
	else
	{
		while ((de = readdir(dp)) != NULL)
		{
			struct stat st;
			read_directory_single_object *rd = response->add_objects();

			// memset(&st, 0, sizeof(st));
			st.st_ino = de->d_ino;
			st.st_mode = de->d_type << 12;

			rd->set_name(de->d_name);
			*rd->mutable_st() = toGstat(&st);
		}
		response->set_status(0);
	}

	if (LOG)
		std::cout << "------------------------------------------------\n\n";

	closedir(dp);
	return Status::OK;
}
