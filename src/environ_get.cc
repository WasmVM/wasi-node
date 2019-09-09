#include <node_api.h>
#include <stddef.h>
#include <stdio.h>

#include "wasi_core.h"
#include "api.hpp"

namespace wasi_node {

napi_value environ_get(napi_env env, napi_callback_info cbinfo) {
  // TODO: 
  printf("environ_get\n");
  // Result
  napi_value result;
  if(napi_create_int32(env, __WASI_ESUCCESS, &result) != napi_ok) return nullptr;
  return result;
}

} // namespace wasi_node