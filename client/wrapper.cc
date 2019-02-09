#include "wrapper.h"
#include "nfsClient.h"
#include "../utils/utils.h"
#include <stdio.h>

static clientImplementation nfsClient(grpc::CreateChannel("0.0.0.0:3110", grpc::InsecureChannelCredentials()));


int wrapper_getattr(const char *path, struct stat *statbuf, struct fuse_file_info *fi) {
	printf("wrapper get attr\n");
	return 0;
}

int wrapper_readlink(const char *path, char *link, size_t size) {
	printf("wrapper read link\n");
	return 0;
}

int wrapper_mknod(const char *path, mode_t mode, dev_t dev) {
	printf("wrapper mknod\n");
	return 0;
}

int wrapper_mkdir(const char *path, mode_t mode) {
	printf("wrapper mkdir\n");
	return nfsClient.client_mkdir(toStr(path), mode);
}

int wrapper_unlink(const char *path) {
	printf("wrapper unlink\n");
	return 0;
}

int wrapper_rmdir(const char *path) {
	printf("wrapper rmdir\n");
	return nfsClient.client_rmdir(toStr(path));
}

int wrapper_symlink(const char *path, const char *link) {
	printf("wrapper symlink\n");
	return 0;
}

int wrapper_rename(const char *path, const char *newpath, unsigned int flag) {
	printf("wrapper rename\n");
	return nfsClient.client_rename(toStr(path), toStr(newpath));
}

int wrapper_link(const char *path, const char *newpath) {
	printf("wrapper link\n");
	return 0;
}

int wrapper_chmod(const char *path, mode_t mode) {
	printf("wrapper chmod\n");
	return 0;
}

int wrapper_create(const char * path, mode_t mode, struct fuse_file_info * fileInfo) {
	printf("wrapper create\n");
	return nfsClient.client_create(toStr(path), mode, fileInfo);
}

int wrapper_chown(const char *path, uid_t uid, gid_t gid) {
	printf("wrapper chown\n");
	return 0;
}

int wrapper_truncate(const char *path, off_t newSize, struct fuse_file_info *fi) {
	printf("wrapper truncate\n");
	return 0;
}

int wrapper_utime(const char *path, struct utimbuf *ubuf) {
	printf("wrapper utime\n");
	return 0;
}

int wrapper_open(const char *path, struct fuse_file_info *fileInfo) {
	printf("wrapper open\n");
	return nfsClient.client_open(toStr(path), fileInfo);
}

int wrapper_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
	printf("wrapper read\n");
	return 0;
}

int wrapper_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
	printf("wrapper write\n");
	return 0;
}

int wrapper_statfs(const char *path, struct statvfs *statInfo) {
	printf("wrapper statfs\n");
	return 0;
}

int wrapper_flush(const char *path, struct fuse_file_info *fileInfo) {
	printf("wrapper flush\n");
	return 0;
}

int wrapper_release(const char *path, struct fuse_file_info *fileInfo) {
	printf("wrapper release\n");
	return 0;
}

int wrapper_fsync(const char *path, int datasync, struct fuse_file_info *fi) {
	printf("wrapper fsync\n");
	return 0;
}

int wrapper_setxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
	printf("wrapper rsetxattr\n");
	return 0;
}

int wrapper_getxattr(const char *path, const char *name, char *value, size_t size) {
	printf("wrapper getxattr\n");
	return 0;
}

int wrapper_listxattr(const char *path, char *list, size_t size) {
	printf("wrapper listxattr\n");
	return 0;
}

int wrapper_removexattr(const char *path, const char *name) {
	printf("wrapper removexattr\n");
	return 0;
}

int wrapper_opendir(const char *path, struct fuse_file_info *fileInfo) {
	printf("wrapper opendir\n");
	return 0;
}

int wrapper_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo, fuse_readdir_flags rflags) {
	printf("wrapper readdir\n");
	return 0;
}

int wrapper_releasedir(const char *path, struct fuse_file_info *fileInfo) {
	printf("wrapper releasedir\n");
	return 0;
}

int wrapper_fsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo) {
	printf("wrapper fsyncdir\n");
	return 0;
}

int wrapper_init(struct fuse_conn_info *conn) {
	printf("wrapper init\n");
	return 0;
}
