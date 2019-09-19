#define NAPI_EXPERIMENTAL
#include <node_api.h>

extern "C" {
#include <stddef.h>
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

napi_value fd_prestat_dir_name(napi_env env, napi_callback_info cbinfo) {
// Result variables;
  napi_value result;
  int32_t resInt = __WASI_ESUCCESS;

  // Unfold callback info
  size_t argc = 3;
  napi_value argv[3];
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
  uint32_t pathBuf_len;
  if(napi_get_value_uint32(env, argv[2], &pathBuf_len)){
    napi_throw_error(env, "ERR_INVALID_ARG_VALUE", "Can't retrieve path buffer length");
    if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
    return result;
  }
  char *pathBuf = nullptr;
  if(napi_get_wasi_pointer(env, argv[1], contextRef, (void**)&pathBuf, sizeof(char)*pathBuf_len) != napi_ok){
    napi_throw_error(env, "ERR_INVALID_ARG_VALUE", "Can't retrieve path buffer");
    if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
    return result;
  }

  // Get path name by fd
  char filePath[pathBuf_len + 1];
  #ifdef __linux__
    char procPath[PATH_MAX];
    if(sprintf(procPath, "/proc/self/fd/%d", fd) < 0){
      if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
      return result;
    }
    if((resInt = call_readlink(procPath, filePath, pathBuf_len, nullptr)) != __WASI_ESUCCESS){
      if(napi_create_int32(env, resInt, &result) != napi_ok) return nullptr;
      return result;
    }
  #else 
  #ifdef __APPLE__
    if((resInt = call_fcntl_get_path(fd, pathBuf)) != __WASI_ESUCCESS){
      if(napi_create_int32(env, resInt, &result) != napi_ok) return nullptr;
      return result;
    }
  #else
  #error "Can't get directory path in this operating system"
  #endif
  #endif
  filePath[pathBuf_len] = '\0';

  // Check capable
  napi_value capablePath;
  if(napi_get_named_property(env, context, "capable_path", &capablePath) != napi_ok) return nullptr;
  uint32_t capableLen;
  if(napi_get_array_length(env, capablePath, &capableLen) != napi_ok) return nullptr;
  for(uint32_t i = 0; i < capableLen; ++i){
    napi_value pathObj;
    if(napi_get_element(env, capablePath, i, &pathObj) != napi_ok) return nullptr;
    char pathStr[PATH_MAX];
    if(napi_get_value_string_utf8(env, pathObj, pathStr, PATH_MAX, nullptr) != napi_ok) return nullptr;
    if(!strcmp(filePath, pathStr)){
      memset(filePath, '\0',pathBuf_len);
      if(napi_create_int32(env, resInt, &result) != napi_ok) return nullptr;
      return result;
    }
  }

  // Set path if not capable
  memcpy(pathBuf, filePath, pathBuf_len + 1);

  // Result
  if(napi_create_int32(env, resInt, &result) != napi_ok) return nullptr;
  return result;
}

} // namespace wasi_node