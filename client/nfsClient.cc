#include "nfsClient.h"

#define LOG true

clientImplementation::clientImplementation(std::shared_ptr<Channel> channel)
	: stub_(NfsServer::NewStub(channel)) {}

int clientImplementation::client_mkdir(std::string path, mode_t mode)
{

	mkdir_request request;
	c_response response;
	attributes atr;
	ClientContext context;

	request.set_dirfh(path);
	atr.set_st_mode(mode);
	request.mutable_attr()->CopyFrom(atr);

	std::cout << "calling mkdir: " << std::endl;
	Status status = stub_->server_mkdir(&context, request, &response);

	if (status.ok())
	{
		if (response.success() != 0)
			return (-response.ern());
		return 0;
	}
	else
	{
		std::cout << status.error_code() << ": " << status.error_message() << std::endl;
		return -1;
	}
}

int clientImplementation::client_rmdir(std::string path)
{

	rmdir_request request;
	c_response response;
	ClientContext context;

	request.set_dirfh(path);

	std::cout << "calling rmdir: " << std::endl;
	Status status = stub_->server_rmdir(&context, request, &response);

	if (status.ok())
	{
		if (response.success() != 0)
			return (-response.ern());
		return 0;
	}
	else
	{
		std::cout << status.error_code() << ": " << status.error_message() << std::endl;
		return -1;
	}
}

int clientImplementation::client_rename(std::string from, std::string to)
{

	rename_request request;
	c_response response;
	ClientContext context;

	request.set_fromdir(from);
	request.set_todir(to);

	std::cout << "calling rename: " << std::endl;
	Status status = stub_->server_rename(&context, request, &response);

	if (status.ok())
	{
		if (response.success() != 0)
			return (-response.ern());
		return 0;
	}
	else
	{
		std::cout << status.error_code() << ": " << status.error_message() << std::endl;
		return -1;
	}
}

int clientImplementation::client_open(std::string path, struct fuse_file_info *fi)
{

	open_request request;
	d_response response;
	ClientContext context;

	request.set_fh(path);
	request.mutable_pfi()->CopyFrom(toProtoFileInfo(fi));

	std::cout << "calling open: " << std::endl;
	Status status = stub_->server_open(&context, request, &response);

	if (status.ok())
	{
		toFuseFileInfo(response.pfi(), fi);
		if (response.success() != 0)
			return (-response.ern());
		return 0;
	}
	else
	{
		std::cout << status.error_code() << ": " << status.error_message() << std::endl;
		return -1;
	}
}

int clientImplementation::client_create(std::string path, mode_t mode, struct fuse_file_info *fi)
{

	create_truncate_request request;
	d_response response;
	attributes atr;
	ClientContext context;

	request.set_fh(path);

	atr.set_st_mode(mode);
	request.mutable_attr()->CopyFrom(atr);
	request.mutable_pfi()->CopyFrom(toProtoFileInfo(fi));

	std::cout << "calling create: " << std::endl;
	Status status = stub_->server_create(&context, request, &response);

	if (status.ok())
	{
		toFuseFileInfo(response.pfi(), fi);
		if (response.success() != 0)
			return (-response.ern());
		return 0;
	}
	else
	{
		std::cout << status.error_code() << ": " << status.error_message() << std::endl;
		return -1;
	}
}

int clientImplementation::client_truncate(std::string path, off_t size, struct fuse_file_info *fi)
{

	create_truncate_request request;
	attributes atr;
	d_response response;
	ClientContext context;

	request.set_fh(path);
	atr.set_st_size(size);
	request.mutable_attr()->CopyFrom(atr);
	request.mutable_pfi()->CopyFrom(toProtoFileInfo(fi));

	std::cout << "calling truncate: " << std::endl;
	Status status = stub_->server_truncate(&context, request, &response);

	if (status.ok())
	{
		toFuseFileInfo(response.pfi(), fi);
		if (response.success() != 0)
			return (-response.ern());
		return 0;
	}
	else
	{
		std::cout << status.error_code() << ": " << status.error_message() << std::endl;
		return -1;
	}
}

int clientImplementation::client_unlink(std::string path)
{

	unlink_request request;
	c_response response;
	ClientContext context;

	request.set_fh(path);

	std::cout << "calling unlink: " << std::endl;
	Status status = stub_->server_unlink(&context, request, &response);

	if (status.ok())
	{
		if (response.success() != 0)
			return (-response.ern());
		return 0;
	}
}

std::list<DirEntry> clientImplementation::read_directory(std::string path, int &responseCode)
{
	readdir_request request;
	readdir_response response;
	ClientContext context;

	request.set_path(path);

	std::cout << "calling readdir: " << std::endl;
	Status status = stub_->read_directory(&context, request, &response);

	std::list<DirEntry> entries;
	if (status.ok())
	{
		responseCode = response.status();
		for (int i = 0; i < response.objects_size(); i++)
		{
			DirEntry dirEntry;
			toCstat(response.objects(i).attr(), &dirEntry.st);
			dirEntry.name = response.objects(i).name();
			entries.push_back(dirEntry);
		}
		return entries;
	}
	else
	{
		return entries;
	}
}

// int clientImplementation::read(std::string path, char* buffer,int offset, int size, struct fuse_file_info *fi) {

// }

int clientImplementation::get_attributes(std::string path, struct stat *st)
{

	if (LOG)
		std::cout << "------------------------------------------------\n";
	if (LOG)
		std::cout << "getAttributes : path passed - " << path << "\n";
	// Container request
	attribute_request_object getAttributesRequestObject;
	getAttributesRequestObject.set_path(path);
	*getAttributesRequestObject.mutable_attr() = toGstat(st);
	ClientContext context;

	// Container response
	attribute_response_object getAttributesResponseObject;
	if (LOG)
		std::cout << "getAttributes : Calling server \n";
	// Actual call
	Status status = stub_->get_attributes(&context, getAttributesRequestObject, &getAttributesResponseObject);
	if (LOG)
		std::cout << "getAttributes : Response from server \n";
	if (status.ok())
	{
		if (LOG)
			std::cout << "getAttributes : converting response \n";
		toCstat(getAttributesResponseObject.attr(), st);
		if (LOG)
			std::cout << "getAttributes : returning resposne \n";
		return getAttributesResponseObject.status();
	}
	else
	{
		if (LOG)
			std::cout << "getAttributes : Failed \n";
		if (LOG)
			std::cout << status.error_code() << ": " << status.error_message()
					  << std::endl;

		if (LOG)
			std::cout << "------------------------------------------------\n\n";
		return -1;
	}
}
int clientImplementation::read(std::string path, char *buffer, int offset, int size, struct fuse_file_info *fi)
{

	read_request request;
	read_response response;
	ClientContext context;

	request.set_path(path);
	request.set_offset(offset);
	request.set_size(size);
	request.mutable_pfi()->CopyFrom(toProtoFileInfo(fi));

	std::cout << "calling readdir: " << std::endl;
	Status status = stub_->server_read(&context, request, &response);

	toFuseFileInfo(response.pfi(), fi);
	if (status.ok())
	{
		strncpy(buffer, response.data().c_str(), size);
		return response.size();
	}
	else
	{
		return -1;
	}
}
