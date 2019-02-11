#include <sys/stat.h>
#include <unistd.h>

#include "nfsServer.h"

#define LOG true

serverImplementation::serverImplementation(std::string path)
{
	Service();
	this->base = path;
}

Status serverImplementation::server_mkdir(ServerContext *context, const mkdir_request *request, c_response *response)
{

	std::string newDir = this->base + request->dirfh();
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

Status serverImplementation::server_open(ServerContext *context, const open_request *request, d_response *response)
{
	std::string filepath = this->base + request->fh();
	char *filepathChars;
	filepathChars = (char *)malloc(filepath.length() + 1);
	strcpy(filepathChars, filepath.c_str());

	struct fuse_file_info fi;
	toFuseFileInfo(request->pfi(), &fi);

	int fh = open(filepathChars, fi.flags);

	if (fh == -1)
	{
		response->set_success(-1);
		response->set_ern(errno);
	}
	else
	{
		response->set_success(0);
		response->set_ern(errno);
	}
	fi.fh = fh;
	response->mutable_pfi()->CopyFrom(toProtoFileInfo(&fi));

	return Status::OK;
}

Status serverImplementation::server_create(ServerContext *context, const create_truncate_request *request, d_response *response)
{
	std::string filepath = this->base + request->fh();
	mode_t mode = request->attr().st_mode();
	char *filepathChars;
	filepathChars = (char *)malloc(filepath.length() + 1);
	strcpy(filepathChars, filepath.c_str());

	struct fuse_file_info fi;
	toFuseFileInfo(request->pfi(), &fi);

	int fh = open(filepathChars, fi.flags, mode);

	if (fh == -1)
	{
		response->set_success(-1);
		response->set_ern(errno);
	}
	else
	{
		response->set_success(0);
		response->set_ern(errno);
	}
	fi.fh = fh;
	response->mutable_pfi()->CopyFrom(toProtoFileInfo(&fi));

	return Status::OK;
}

Status serverImplementation::read_directory(ServerContext *context, const readdir_request *request,
											readdir_response *response)
{

	DIR *dp;
	struct dirent *de;
	if (LOG)
		std::cout << "------------------------------------------------\n";
	std::cout << "You are in read_directory method";
	if (LOG)
		std::cout << "ReadDirectory : path passed - " << request->path() << "\n";
	std::string adjustedPath = this->base + request->path();
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
			*rd->mutable_attr() = toGstat(&st);
		}
		response->set_status(0);
	}

	if (LOG)
		std::cout << "------------------------------------------------\n\n";

	closedir(dp);
	return Status::OK;
}

Status serverImplementation::get_attributes(ServerContext *context, const attribute_request_object *request,
											attribute_response_object *response)
{

	struct stat st;

	toCstat(request->attr(), &st);
	// if (LOG)
	// 	std::cout << "------------------------------------------------\n";
	// if (LOG)
	// 	std::cout << "GetAttributes : path passed - " << request->path() << "\n";
	std::string adjustedPath = this->base + request->path();

	// std::cout << adjustedPath << "\n";

	char *path = new char[adjustedPath.length() + 1];
	strcpy(path, adjustedPath.c_str());
	int res = lstat(path, &st);
	if (res == -1)
	{
		// if (LOG)
		// 	std::cout << "GetAttributes : Error getting stat -  " << errno << " Error message - " << std::strerror(errno) << "\n";
		response->set_status(-errno);
	}
	else
	{
		response->set_status(0);
		*response->mutable_attr() = toGstat(&st);
	}

	// if (LOG)
	// 	std::cout << "------------------------------------------------\n\n";

	return Status::OK;
}

Status serverImplementation::server_truncate(ServerContext *context, const create_truncate_request *request, d_response *response)
{

	std::string filepath = this->base + request->fh();
	char *filepathChars;
	filepathChars = (char *)malloc(filepath.length() + 1);
	strcpy(filepathChars, filepath.c_str());

	struct fuse_file_info fi;
	toFuseFileInfo(request->pfi(), &fi);

	int op;
	if (fi.fh != 0)
	{
		op = ftruncate(fi.fh, request->attr().st_size());
	}
	else
	{
		op = truncate(filepathChars, request->attr().st_size());
	}

	if (op == -1)
	{
		response->set_success(-1);
		response->set_ern(errno);
	}
	else
	{
		response->set_success(0);
		response->set_ern(0);
		response->mutable_pfi()->CopyFrom(toProtoFileInfo(&fi));
	}
	return Status::OK;
}

Status serverImplementation::server_unlink(ServerContext *context, const unlink_request *request, c_response *response)
{

	std::string filepath = this->base + request->fh();
	char *filepathChars;
	filepathChars = (char *)malloc(filepath.length() + 1);
	strcpy(filepathChars, filepath.c_str());

	if (unlink(filepathChars) == -1)
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

Status serverImplementation::server_read(ServerContext *context, const read_request *request, read_response *response)
{

	std::string filepath = this->base + request->path();
	char *filepathChars;
	filepathChars = (char *)malloc(filepath.length() + 1);
	strcpy(filepathChars, filepath.c_str());

	char *buffer = new char[request->size()];

	struct fuse_file_info fi;
	toFuseFileInfo(request->pfi(), &fi);

	int fileHandle = fi.fh;
	int op;

	std::cout << "in read " << filepathChars << request->offset() << request->size() << "\n";

	if (fi.fh == 0)
	{
		fileHandle = open(filepathChars, O_RDONLY);
		fi.fh = fileHandle;
	}
	if (fileHandle == -1)
	{
		response->set_success(-1);
		response->set_ern(errno);
	}
	else
	{
		op = pread(fileHandle, buffer, request->size(), request->offset());
		std::cout << "size: " << request->size() << ", " << buffer << "\n";
		if (op == -1)
		{
			response->set_success(-1);
			response->set_ern(errno);
		}
		else
		{
			response->set_data(buffer);
			response->set_size(op);
			response->mutable_pfi()->CopyFrom(toProtoFileInfo(&fi));
		}
	}
	delete buffer;
	return Status::OK;
}

Status serverImplementation::server_mknod(ServerContext *context, const read_directory_single_object *request, c_response *response)
{

	std::string filepath = this->base + request->name();
	char *filepathChars;
	filepathChars = (char *)malloc(filepath.length() + 1);
	strcpy(filepathChars, filepath.c_str());

	int op;

	mode_t mode = request->attr().st_mode();
	dev_t rdev = request->attr().st_dev();

	if (S_ISFIFO(mode))
	{
		op = mkfifo(filepathChars, mode);
	}
	else
	{
		op = mknod(filepathChars, mode, rdev);
	}

	if (op == -1)
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

Status serverImplementation::write(ServerContext *context, const write_request_object *request,
								   write_response_object *response)
{

	if (LOG)
		std::cout << "------------------------------------------------\n";
	if (LOG)
		std::cout << "Write : path passed - " << request->path() << "\n";
	int fileDir;
	//char *buffer = new char[request->data().length() + 1];
	std::string adjustedPath = this->base + request->path();
	char *path = new char[adjustedPath.length() + 1];
	strcpy(path, adjustedPath.c_str());

	std::string buffer = request->data();
	struct fuse_file_info fi;
	toCFileInfo(request->fileinfo(), &fi);
	if (LOG)
		std::cout << "Write: got all the inputs sorted. File header - " << fi.fh << "\n";

	//(void) fi;
	if (fi.fh == 0)
	{
		fileDir = open(path, O_WRONLY);
	}
	else
	{
		fileDir = fi.fh;
	}

	if (LOG)
		std::cout << "Write: got the file directory - " << fileDir << "\n";
	int res;

	if (fileDir == -1)
	{
		if (LOG)
			std::cout << "Write: Error occured with file directory - " << errno << "\n";
		response->set_status(-errno);
	}
	else
	{
		res = pwrite(fileDir, &buffer[0], request->size(), request->offset());

		if (res == -1)
		{
			if (LOG)
				std::cout << "Write: Error occured with writing to file- " << errno << " Error message - " << std::strerror(errno) << "\n";
			response->set_status(-errno);
		}

		// this checks if the commit flag has been set.
		// this would contain logic to write to disk
		if (request->flag())
		{
			// fsync(fi.fh);
			close(dup(fi.fh));
		}
	}
	if (LOG)
		std::cout << "------------------------------------------------\n\n";

	//close(fileDir);
	response->set_status(0);
	response->set_datasize(res);
	*response->mutable_fileinfo() = toGFileInfo(&fi);
	return Status::OK;
}

Status serverImplementation::fsync(ServerContext *context, const fsync_request *request,
								   fsync_response *response)
{

	if (LOG)
		std::cout << "------------------------------------------------\n";
	if (LOG)
		std::cout << "Fsync : path passed - " << request->path() << "\n";
	std::string adjustedPath = this->base + request->path();
	char *path = new char[adjustedPath.length() + 1];
	strcpy(path, adjustedPath.c_str());
	int isdatasync = request->isdatasync();
	struct fuse_file_info fi;
	toCFileInfo(request->fileinfo(), &fi);

	(void)path;
	(void)isdatasync;
	(void)fi;
	response->set_status(0);
	*response->mutable_fileinfo() = toGFileInfo(&fi);
	if (LOG)
		std::cout << "------------------------------------------------\n\n";
	return Status::OK;
}

Status serverImplementation::flush(ServerContext *context, const flush_request *request,
								   flush_response *response)
{

	if (LOG)
		std::cout << "------------------------------------------------\n";
	if (LOG)
		std::cout << "Flush : path passed - " << request->path() << "\n";
	std::string adjustedPath = this->base + request->path();
	char *path = new char[adjustedPath.length() + 1];
	strcpy(path, adjustedPath.c_str());
	struct fuse_file_info fi;
	toCFileInfo(request->fileinfo(), &fi);
	if (LOG)
		std::cout << "Flush : FH received - " << fi.fh << "\n";

	(void)path;
	int res = close(dup(fi.fh));
	response->set_status(0);
	if (res == -1)
	{
		response->set_status(-errno);
	}
	if (LOG)
		std::cout << "Flush : FH closed, fh - " << fi.fh << "\n";
	*response->mutable_fileinfo() = toGFileInfo(&fi);
	if (LOG)
		std::cout << "------------------------------------------------\n\n";
	return Status::OK;
}