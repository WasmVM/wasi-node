#define NAPI_EXPERIMENTAL
#include <node_api.h>

extern "C"{
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <syslimits.h>
#include <string.h>
}

#include "wasi_core.h"
#include "api.hpp"
#include "util.hpp"
#include "sys_wrap.hpp"

namespace wasi_node {

napi_value fd_prestat_get(napi_env env, napi_callback_info cbinfo) {
  // Result variables;
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

  // Get args
  __wasi_fd_t fd;
  if(napi_get_value_uint32(env, argv[0], &fd)){
    napi_throw_error(env, "ERR_INVALID_ARG_VALUE", "Can't retrieve file descriptor");
    if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
    return result;
  }
  __wasi_prestat_t *buf = nullptr;
  if(napi_get_wasi_pointer(env, argv[1], contextRef, (void**)&buf, sizeof(__wasi_prestat_t)) != napi_ok){
    if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
    return result;
  }

  // Get preopen file type
  struct stat stats;
  if((resInt = call_fstat(fd, &stats)) != __WASI_ESUCCESS){
    if(napi_create_int32(env, resInt, &result) != napi_ok) return nullptr;
    return result;
  }
  if(S_ISDIR(stats.st_mode)){
    buf->pr_type = __WASI_PREOPENTYPE_DIR;
    // Get path
    char filePath[PATH_MAX];
    #ifdef __linux__
      char procPath[PATH_MAX];
      if(sprintf(procPath, "/proc/self/fd/%d", fd) < 0){
        if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
        return result;
      }
      ssize_t pathSize;
      if((resInt = call_readlink(procPath, filePath, PATH_MAX, &pathSize)) != __WASI_ESUCCESS){
        if(napi_create_int32(env, resInt, &result) != napi_ok) return nullptr;
        return result;
      }
      buf->u.dir.pr_name_len = pathSize;
    #else 
    #ifdef __APPLE__
      if((resInt = call_fcntl_get_path(fd, filePath)) != __WASI_ESUCCESS){
        if(napi_create_int32(env, resInt, &result) != napi_ok) return nullptr;
        return result;
      }
      buf->u.dir.pr_name_len = strlen(filePath);
    #else
    #error "Can't get directory path in this operating system"
    #endif
    #endif
  }else{
    buf->pr_type = !__WASI_PREOPENTYPE_DIR;
  }

  // Set result
  if(napi_create_int32(env, resInt, &result) != napi_ok) return nullptr; 
  return result;
}

} // namespace wasi_node