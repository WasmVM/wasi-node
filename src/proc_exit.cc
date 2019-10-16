#include <node_api.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "wasi_core.h"
#include "api.hpp"

namespace wasi_node {

napi_value proc_exit(napi_env env, napi_callback_info cbinfo) {
  napi_value result;
  // Unfold callback info
  size_t argc = 1;
  napi_value argv[1];
  if(napi_get_cb_info(env, cbinfo, &argc, argv, nullptr, nullptr) != napi_ok) return nullptr;
  // Get argument
  __wasi_exitcode_t code;
  if(napi_get_value_uint32(env, argv[0], &code)){
    napi_throw_error(env, "ERR_INVALID_ARG_VALUE", "Can't retrieve exit code");
    if(napi_create_int32(env, __WASI_EINVAL, &result) != napi_ok) return nullptr;
    return result;
  }
  // Execute exit
  exit(code);
  // Result
  return nullptr;
}

} // namespace wasi_node