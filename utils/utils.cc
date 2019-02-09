#include "utils.h"


std::string toStr(const char * path) {
	std::string a(path);
	return a;
}

proto_file_info toProtoFileInfo(struct fuse_file_info *fi) {
	proto_file_info pfi;
	pfi.set_fh(fi->fh);
	pfi.set_flags(fi->flags);
	return pfi;
}

void toFuseFileInfo(proto_file_info pfi, struct fuse_file_info *fi) {
	fi->fh = pfi.fh();
	fi->flags = pfi.flags();
}

