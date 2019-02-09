#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <string>
#include "../proto/schema.grpc.pb.h"

using NfsProtocol::proto_file_info;

std::string toStr(const char * path);

proto_file_info toProtoFileInfo(struct fuse_file_info *fi);

void toFuseFileInfo(proto_file_info pfi, struct fuse_file_info *fi);




