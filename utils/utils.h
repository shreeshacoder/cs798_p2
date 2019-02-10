#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <string>
#include "../proto/schema.grpc.pb.h"
#include "../proto/schema.pb.h"

using NfsProtocol::attributes;
using NfsProtocol::proto_file_info;

struct DirEntry
{
    struct stat st;
    std::string name;
};

std::string toStr(const char *path);

proto_file_info toProtoFileInfo(struct fuse_file_info *fi);

void toFuseFileInfo(proto_file_info pfi, struct fuse_file_info *fi);

attributes toGstat(struct stat *st);

void toCstat(attributes gstat, struct stat *st);
