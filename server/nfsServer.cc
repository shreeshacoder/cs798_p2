#include <sys/stat.h>
#include <unistd.h>
#include <experimental/filesystem>

#include "nfsServer.h"

using std::experimental::filesystem::path;

#define LOG true

serverImplementation::serverImplementation(std::string path)
{
	Service();
	this->base = path;
	this->stofh[""] = 0;
	this->fhtos[0] = "";
	fhcount = 1;
}

void serverImplementation::print_lookup() {
	std::cout << "---\n";
	for (auto& e : this->stofh) {
		std::cout << e.first << " : " << e.second << "\n";
	}
	std::cout << "---\n";
}

int serverImplementation::addToLookup(std::string path) {
	if( this->stofh.find(path) == this->stofh.end() ) {
		this->stofh[path] = this->fhcount;
		this->fhtos[this->fhcount] = path;
		this->fhcount++;
		std::cout << "added " << path << " at " << (this->fhcount-1) << " in lookup \n";
	}
	return this->stofh[path];
}

Status serverImplementation::server_lookup(ServerContext *context, const lookup_request *request, lookup_response *response) {

	std::string parent_path = this->fhtos[request->dirfh()];
	std::string new_parent_path = parent_path + "/" + request->name();

	// std::cout << "server lookup for " << new_parent_path << "\n";

	if( this->stofh.find(new_parent_path) == this->stofh.end() ) {
		this->addToLookup(new_parent_path);
	}
	// std::cout << "server response to lookup" << this->stofh.find(new_parent_path)->second << "\n";
	response->set_fh(this->stofh.find(new_parent_path)->second);
	return Status::OK;
}

std::string serverImplementation::back_lookup(int fh) {
	
	if( this->fhtos.find(fh) == this->fhtos.end() ) 
		return NULL;
	return this->fhtos[fh];

}


Status serverImplementation::server_mkdir(ServerContext *context, const mkdir_request *request, c_response *response)
{

	std::string newDir = this->base + this->back_lookup(request->dirfh()) + "/" + request->name();
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
		this->addToLookup(this->back_lookup(request->dirfh()) + "/" + request->name());
		response->set_success(0);
		response->set_ern(0);
	}
	return Status::OK;
}

Status serverImplementation::server_rmdir(ServerContext *context, const rmdir_request *request, c_response *response)
{
	std::string dirToRemove = this->base + this->back_lookup(request->dirfh());
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
		this->stofh.erase( this->stofh.find(this->back_lookup(request->dirfh()))  );
		this->fhtos.erase( this->fhtos.find(request->dirfh() )  );
	}
	this->print_lookup();
	return Status::OK;
}

Status serverImplementation::server_rename(ServerContext *context, const rename_request *request, c_response *response)
{

	std::string toDir = this->base + this->back_lookup(request->todirfh()) + "/" + request->name();
	char *toDirChars;
	toDirChars = (char *)malloc(toDir.length() + 1);
	strcpy(toDirChars, toDir.c_str());

	std::string fromDir = this->base + this->back_lookup(request->fromfh());
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
		this->addToLookup(this->back_lookup(request->todirfh()) + "/" + request->name());
		this->stofh.erase( this->stofh.find(this->back_lookup(request->fromfh()))  );
		this->fhtos.erase( this->fhtos.find(request->fromfh() ) );
		response->set_success(0);
		response->set_ern(0);
	}
	return Status::OK;
}

Status serverImplementation::server_open(ServerContext *context, const open_request *request, d_response *response)
{
	std::string filepath = this->base + this->back_lookup(request->fh());
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

Status serverImplementation::server_create(ServerContext *context, const create_request *request, d_response *response)
{
	std::string filepath = this->base +  this->back_lookup(request->dirfh() ) + "/" + request->name()   ;
	char *filepathChars;
	filepathChars = (char *)malloc(filepath.length() + 1);
	strcpy(filepathChars, filepath.c_str());
	mode_t mode = request->attr().st_mode();

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
		this->addToLookup(this->back_lookup(request->dirfh()) + "/" + request->name());
		response->set_success(0);
		response->set_ern(errno);
	}
	fi.fh = fh;
	response->mutable_pfi()->CopyFrom(toProtoFileInfo(&fi));

	return Status::OK;
}

Status serverImplementation::read_directory(ServerContext *context, const readdir_request *request, readdir_response *response)
{

	DIR *dp;
	struct dirent *de;
	std::string qpath;

	std::string adjustedPath = this->base + this->back_lookup(request->dirfh());
	char *path = new char[adjustedPath.length() + 1];
	strcpy(path, adjustedPath.c_str());

	std::cout << "server readdir\n";
	// std::cout << request->path()	 << "\n";

	dp = opendir(path);
	if (dp == NULL)
	{
		response->set_status(-errno);
	}
	else
	{
		while ((de = readdir(dp)) != NULL) {

			struct stat st;
			read_directory_single_object *rd = response->add_objects();
			st.st_ino = de->d_ino;
			st.st_mode = de->d_type << 12;
			rd->set_name(de->d_name);
			*rd->mutable_attr() = toGstat(&st);

			if(this->back_lookup(request->dirfh())[this->back_lookup(request->dirfh()).length()-1] == '/')
				qpath = this->back_lookup(request->dirfh()) + rd->name();
			else
				qpath = this->back_lookup(request->dirfh()) + "/" + rd->name();

			this->addToLookup(qpath);
		
		}
		response->set_status(0);
	}

	closedir(dp);
	return Status::OK;
}

Status serverImplementation::get_attributes(ServerContext *context, const attribute_request *request, attribute_response *response)
{
	std::cout << "server getattr\n";

	struct stat st;

	toCstat(request->attr(), &st);
	std::string adjustedPath = this->base + this->back_lookup(request->fh());

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

Status serverImplementation::server_truncate(ServerContext *context, const truncate_request *request, d_response *response)
{

	std::string filepath = this->base + this->back_lookup(request->fh());
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

	std::string filepath = this->base + this->back_lookup(request->fh());
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

	std::string filepath = this->base + this->back_lookup(request->fh());
	char *filepathChars;
	filepathChars = (char *)malloc(filepath.length() + 1);
	strcpy(filepathChars, filepath.c_str());

	char *buffer = new char[request->size()];

	struct fuse_file_info fi;
	toFuseFileInfo(request->pfi(), &fi);

	int fileHandle = fi.fh;
	int op;

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
		std::cout << "size: " << request->size() << ", "<<  buffer << "\n";
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

Status serverImplementation::server_mknod(ServerContext *context, const read_directory_single_object *request, c_response *response) {

	std::string filepath = this->base + request->name();
	char *filepathChars;
	filepathChars = (char *)malloc(filepath.length() + 1);
	strcpy(filepathChars, filepath.c_str());

	int op;

	mode_t mode = request->attr().st_mode();
	dev_t rdev = request->attr().st_dev();
   
	if (S_ISFIFO(mode)) {
		op = mkfifo(filepathChars, mode);
	} else {
		op = mknod(filepathChars, mode, rdev);
	}

	if (op == -1) {
		response->set_success(-1);
		response->set_ern(errno);
	}
	else {
		response->set_success(0);
		response->set_ern(0);	
	}
	return Status::OK;
}

Status serverImplementation::server_release(ServerContext *context, const read_request *request, d_response *response) {

	std::string filepath = this->base + this->back_lookup(request->fh());
	char *filepathChars;
	filepathChars = (char *)malloc(filepath.length() + 1);
	strcpy(filepathChars, filepath.c_str());

	struct fuse_file_info fi;
	toFuseFileInfo(request->pfi(), &fi);

	(void) filepathChars;
	close(fi.fh);

	response->set_success(0);
	response->set_ern(0);
	response->mutable_pfi()->CopyFrom(toProtoFileInfo(&fi));
	return Status::OK;
}
