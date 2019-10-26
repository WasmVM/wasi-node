#include <node_api.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "wasi_core.h"
#include "api.hpp"
#include "util.hpp"

extern char **environ;

namespace wasi_node {

napi_value environ_get(napi_env env, napi_callback_info cbinfo) {
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
  napi_value context;
  if(napi_get_reference_value(env, contextRef, &context) != napi_ok) return nullptr;

  // Get args
  uint32_t *environ_wasi = nullptr;
  if(napi_get_wasi_pointer(env, argv[0], contextRef, (void**)&environ_wasi, WASI_NODE_IGNORE_SIZE_CHECK) != napi_ok){
    napi_throw_error(env, "ERR_INVALID_ARG_VALUE", "Can't retrieve environ pointer");
    if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
    return result;
  }
  char *environ_buf = nullptr;
  if(napi_get_wasi_pointer(env, argv[1], contextRef, (void**)&environ_buf, WASI_NODE_IGNORE_SIZE_CHECK) != napi_ok){
    napi_throw_error(env, "ERR_INVALID_ARG_VALUE", "Can't retrieve environ_buf pointer");
    if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
    return result;
  }
  uint32_t environ_buf_offset;
  if(napi_get_value_uint32(env, argv[1], &environ_buf_offset) != napi_ok){
    napi_throw_error(env, "ERR_INVALID_ARG_VALUE", "Can't retrieve offset of environ_buf");
    if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
    return result;
  }
  printf("Offset: %u\n", environ_buf_offset);

  // Fill environ
  for(size_t i = 0; environ[i] != nullptr; ++i){
    environ_wasi[i] = environ_buf_offset;
    size_t len = strlen(environ[i]);
    strcpy(environ_buf, environ[i]);
    environ_buf[len] = '\0';
    environ_buf_offset += len + 1;
    environ_buf += len + 1;
  }

  // Result
  if(napi_create_int32(env, resInt, &result) != napi_ok) return nullptr;
  return result;
}

} // namespace wasi_node