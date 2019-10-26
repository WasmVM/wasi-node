#include <node_api.h>
#include <stddef.h>
#include <stdio.h>

#include "wasi_core.h"
#include "api.hpp"
#include "util.hpp"

namespace wasi_node {

napi_value args_sizes_get(napi_env env, napi_callback_info cbinfo) {
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
  uint32_t *argcPtr = nullptr;
  if(napi_get_wasi_pointer(env, argv[0], contextRef, (void**)&argcPtr, sizeof(uint32_t)) != napi_ok){
    napi_throw_error(env, "ERR_INVALID_ARG_VALUE", "Can't retrieve argc pointer");
    if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
    return result;
  }
  uint32_t *argv_buf_sizePtr = nullptr;
  if(napi_get_wasi_pointer(env, argv[1], contextRef, (void**)&argv_buf_sizePtr, sizeof(uint32_t)) != napi_ok){
    napi_throw_error(env, "ERR_INVALID_ARG_VALUE", "Can't retrieve argv_buf_size pointer");
    if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
    return result;
  }

  // Get command line argv object
  napi_value cmdArgv;
  if(napi_get_named_property(env, context, "argv", &cmdArgv) != napi_ok) return nullptr;
  // Get command line argc
  if(napi_get_array_length(env, cmdArgv, argcPtr) != napi_ok) return nullptr;
  // Get command line buf length
  *argv_buf_sizePtr = 0;
  for(uint32_t i = 0; i < *argcPtr; ++i){
    napi_value arg;
    if(napi_get_element(env, cmdArgv, i, &arg) != napi_ok) return nullptr;
    size_t argLen;
    if(napi_get_value_string_utf8(env, arg, nullptr, 0, &argLen) != napi_ok) return nullptr;
    *argv_buf_sizePtr += argLen + 1;
  }
  
  // Result
  if(napi_create_int32(env, resInt, &result) != napi_ok) return nullptr;
  return result;
}

} // namespace wasi_node