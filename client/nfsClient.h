#ifndef NFSCLIENT
#define NFSCLIENT

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "../proto/schema.grpc.pb.h"
#include "../proto/schema.pb.h"
#include "../utils/utils.h"

using grpc::Channel;
using grpc::ClientContext;
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
using NfsProtocol::write_request_object;
using NfsProtocol::write_response_object;

class Datastore
{

	std::string data;
	bool isDirty;
	int originalOffset;

  public:
	Datastore(void)
	{
		data = "";
		isDirty = false;
		originalOffset = 0;
	}

	Datastore(std::string d, int offset, bool status)
	{
		data = d;
		isDirty = status;
		originalOffset = offset;
	}

	std::string getData(void)
	{
		return data;
	}
	bool getIsDirty(void)
	{
		return isDirty;
	}

	void setDirty()
	{
		isDirty = true;
	}

	int getOriginalOffset(void)
	{
		return originalOffset;
	}
	void setValues(std::string d, int offset, bool status = false)
	{
		data = d;
		originalOffset = offset;
		isDirty = status;
	}
};

class clientImplementation
{

  private:
	std::unique_ptr<NfsServer::Stub> stub_;

  public:
	clientImplementation(std::shared_ptr<Channel> channel);

	int client_mkdir(std::string path, mode_t mode);
	int client_rmdir(std::string path);
	int client_rename(std::string from, std::string to);
	int client_open(std::string path, struct fuse_file_info *fi);
	int client_create(std::string path, mode_t mode, struct fuse_file_info *fi);
	int client_truncate(std::string path, off_t size, struct fuse_file_info *fi);
	int client_unlink(std::string path);
	int client_read(std::string path, char *buffer, int size, int offset, struct fuse_file_info *fi);
	int client_mknod(std::string path, mode_t mode, dev_t rdev);
	std::list<DirEntry> read_directory(std::string path, int &responseCode);
	int get_attributes(std::string path, struct stat *st);
	int write(std::string path, const char *buf, int size, int offset, struct fuse_file_info *fi);

	// int getAttributes(std::string path, struct stat *st);
	// std::list<DirEntry> readDirectory(std::string path, int &responseCode);
	// int release(std::string path, struct fuse_file_info *fi);
	// int fsync(std::string path, int isdatasync, struct fuse_file_info* fi);
	// int write(std::string path, const char *buf, int size, int offset, struct fuse_file_info* fi);
	// int flush(std::string path, struct fuse_file_info *fi);
	// int utimens(std::string path,const struct timespec *ts, struct fuse_file_info *fi);
};

#endif