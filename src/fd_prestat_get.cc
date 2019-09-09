#include <node_api.h>
#include <stddef.h>
#include <stdio.h>

#include "wasi_core.h"
#include "api.hpp"

namespace wasi_node {

napi_value fd_prestat_get(napi_env env, napi_callback_info cbinfo) {
  // Unfold callback info
  size_t argc = 2;
  napi_value argv[2];
  napi_value thisArg;
  void *data;
  if(napi_get_cb_info(env, cbinfo, &argc, argv, &thisArg, &data) != napi_ok) return nullptr;

  // Get args
  __wasi_fd_t fd;
  if(napi_get_value_uint32(env, argv[0], &fd)) return nullptr;
  printf("fd: %d\n", fd);

  // Result
  napi_value result;
  if(napi_create_int32(env, __WASI_ESUCCESS, &result) != napi_ok) return nullptr;
  return result;
}

} // namespace wasi_node