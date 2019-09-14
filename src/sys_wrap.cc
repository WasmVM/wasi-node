#include "sys_wrap.hpp"

extern "C"{
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "wasi_core.h"
}

namespace wasi_node {

int32_t call_fstat(int fd, struct stat *statPtr){
  if(fstat(fd, statPtr) == -1){
    switch (errno){
      case EACCES:
        return __WASI_EACCES;
      break;
      case EBADF:
        return __WASI_EBADF;
      break;
      case EFAULT:
        return __WASI_EFAULT;
      break;
      case ELOOP:
        return __WASI_ELOOP;
      break;
      case ENAMETOOLONG:
        return __WASI_ENAMETOOLONG;
      break;
      case ENOENT:
        return __WASI_ENOENT;
      break;
      case ENOMEM:
        return __WASI_ENOMEM;
      break;
      case ENOTDIR:
        return __WASI_ENOTDIR;
      break;
      case EOVERFLOW:
        return __WASI_EOVERFLOW;
      break;
      default:
        return __WASI_EINVAL;
      break;
    }
  };
  return __WASI_ESUCCESS;
}
int32_t call_readlink(const char *pathname, char *buf, size_t bufsiz, ssize_t *pathSizPtr){
  if((*pathSizPtr = readlink(pathname, buf, bufsiz)) == -1){
    switch (errno){
      case EACCES:
        return __WASI_EACCES;
      break;
      case EFAULT:
        return __WASI_EFAULT;
      break;
      case EIO:
        return __WASI_EIO;
      break;
      case ELOOP:
        return __WASI_ELOOP;
      break;
      case ENAMETOOLONG:
        return __WASI_ENAMETOOLONG;
      break;
      case ENOENT:
        return __WASI_ENOENT;
      break;
      case ENOMEM:
        return __WASI_ENOMEM;
      break;
      case ENOTDIR:
        return __WASI_ENOTDIR;
      break;
      default:
        return __WASI_EINVAL;
      break;
    }
  };
  return __WASI_ESUCCESS;
}

#ifdef __APPLE__
int32_t call_fcntl_get_path(int fd, char* filePath){
  if(fcntl(fd, F_GETPATH, filePath) == -1){
    switch (errno){
    case EBADF:
      return __WASI_EBADF;
      break;
    default:
      return __WASI_EINVAL;
      break;
    }
  }
  return __WASI_ESUCCESS;
}
#endif

}