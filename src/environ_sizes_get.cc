#include <node_api.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "wasi_core.h"
#include "api.hpp"
#include "util.hpp"

extern char **environ;

namespace wasi_node {

napi_value environ_sizes_get(napi_env env, napi_callback_info cbinfo) {
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
  uint32_t *environ_count = nullptr;
  if(napi_get_wasi_pointer(env, argv[0], contextRef, (void**)&environ_count, sizeof(uint32_t)) != napi_ok){
    napi_throw_error(env, "ERR_INVALID_ARG_VALUE", "Can't retrieve environ_count pointer");
    if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
    return result;
  }
  uint32_t *environ_buf_size = nullptr;
  if(napi_get_wasi_pointer(env, argv[1], contextRef, (void**)&environ_buf_size, sizeof(uint32_t)) != napi_ok){
    napi_throw_error(env, "ERR_INVALID_ARG_VALUE", "Can't retrieve environ_buf_size pointer");
    if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
    return result;
  }

  // Calculate
  for(*environ_count = 0, *environ_buf_size = 0; environ[*environ_count] != nullptr; *environ_count += 1){
    *environ_buf_size += strlen(environ[*environ_count]) + 1;
  }

  // Result
  if(napi_create_int32(env, resInt, &result) != napi_ok) return nullptr;
  return result;
}

} // namespace wasi_node