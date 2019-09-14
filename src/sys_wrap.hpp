#ifndef WASI_NODE_SYS_WRAP_DEF
#define WASI_NODE_SYS_WRAP_DEF

extern "C"{
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
}

namespace wasi_node {

int32_t call_fstat(int fd, struct stat *statPtr);
int32_t call_readlink(const char *pathname, char *buf, size_t bufsiz, ssize_t *pathSizPtr);

#ifdef __APPLE__
int32_t call_fcntl_get_path(int fd, char* filePath);
#endif

}

#endif