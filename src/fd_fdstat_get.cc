#include <node_api.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>

#include "wasi_core.h"
#include "api.hpp"
#include "util.hpp"
#include "sys_wrap.hpp"

namespace wasi_node {

napi_value fd_fdstat_get(napi_env env, napi_callback_info cbinfo) {
  napi_value result;
  int32_t resInt = __WASI_ESUCCESS;

  // Unfold callback info
  size_t argc = 2;
  napi_value argv[2];
  napi_ref contextRef;
  if(napi_get_cb_info(env, cbinfo, &argc, argv, nullptr, (void**)&contextRef) != napi_ok){
    napi_throw_error(env, "ERR_INVALID_ARG_VALUE", "Can't retrieve callback and argument info");
    if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
    return result;
  };
  napi_value context;
  if(napi_get_reference_value(env, contextRef, &context) != napi_ok) return nullptr;

  // Get args
  __wasi_fd_t fd;
  if(napi_get_value_uint32(env, argv[0], &fd)){
    napi_throw_error(env, "ERR_INVALID_ARG_VALUE", "Can't retrieve file descriptor");
    if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
    return result;
  }
  __wasi_fdstat_t *buf = nullptr;
  if(napi_get_wasi_pointer(env, argv[1], contextRef, (void**)&buf, sizeof(__wasi_fdstat_t)) != napi_ok){
    napi_throw_error(env, "ERR_INVALID_ARG_VALUE", "Can't retrieve fdstat buffer");
    if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
    return result;
  }

  // Get stats
  struct stat stats;
  if((resInt = call_fstat(fd, &stats)) != __WASI_ESUCCESS){
    if(napi_create_int32(env, resInt, &result) != napi_ok) return nullptr;
    return result;
  }
  if(S_ISBLK(stats.st_mode)){
    buf->fs_filetype = __WASI_FILETYPE_BLOCK_DEVICE;
  }else if(S_ISCHR(stats.st_mode)){
    buf->fs_filetype = __WASI_FILETYPE_CHARACTER_DEVICE;
  }else if(S_ISDIR(stats.st_mode)){
    buf->fs_filetype = __WASI_FILETYPE_DIRECTORY;
  }else if(S_ISREG(stats.st_mode)){
    buf->fs_filetype = __WASI_FILETYPE_REGULAR_FILE;
  }else if(S_ISLNK(stats.st_mode)){
    buf->fs_filetype = __WASI_FILETYPE_SYMBOLIC_LINK;
  }else if(S_ISSOCK(stats.st_mode)){
    int sockType;
    socklen_t optlen = sizeof(int);
    if((resInt = call_getsockopt(fd, SOL_SOCKET, SO_TYPE, &sockType, &optlen)) != __WASI_ESUCCESS){
      if(napi_create_int32(env, resInt, &result) != napi_ok) return nullptr;
      return result;
    }
    switch (sockType){
      case SOCK_STREAM:
        buf->fs_filetype = __WASI_FILETYPE_SOCKET_STREAM;
        break;
      case SOCK_DGRAM:
        buf->fs_filetype = __WASI_FILETYPE_SOCKET_DGRAM;
        break;
      default:
        buf->fs_filetype = __WASI_FILETYPE_UNKNOWN;
        break;
    }
  }else{
    buf->fs_filetype = __WASI_FILETYPE_UNKNOWN;
  }

  // Result
  if(napi_create_int32(env, resInt, &result) != napi_ok) return nullptr;
  return result;
}

} // namespace wasi_node