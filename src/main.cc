#include <node_api.h>

#include "api.hpp"

#include <stdio.h>

#define WASI_NODE_INIT_API(module, method, func, name) \
  if (napi_create_function(env, nullptr, 0, func, nullptr, &method) != napi_ok) return nullptr; \
  if (napi_set_named_property(env, module, name, method) != napi_ok) return nullptr;

namespace wasi_node {

napi_value initInstance(napi_env env, napi_callback_info cbinfo) {
  // Get argument (instance)
  size_t argc = 1;
  napi_value argv;
  napi_value thisArg;
  void *data;
  if(napi_get_cb_info(env, cbinfo, &argc, &argv, &thisArg, &data) != napi_ok) return nullptr;
  // Get this name
  napi_value props;
  napi_get_property_names(env, thisArg, &props);
  uint32_t propCount;
  napi_get_array_length(env, props, &propCount);
  printf("thisCount: %u\n", propCount);
  return nullptr;
}

napi_value initialize(napi_env env, napi_value exports) {
  napi_value module;
  if (napi_create_object(env, &module) != napi_ok) return nullptr;
  
  napi_value method;
  WASI_NODE_INIT_API(module, method, fd_prestat_get, "fd_prestat_get");
  WASI_NODE_INIT_API(module, method, fd_prestat_dir_name, "fd_prestat_dir_name");
  WASI_NODE_INIT_API(module, method, environ_sizes_get, "environ_sizes_get");
  WASI_NODE_INIT_API(module, method, environ_get, "environ_get");
  WASI_NODE_INIT_API(module, method, args_sizes_get, "args_sizes_get");
  WASI_NODE_INIT_API(module, method, args_get, "args_get");
  WASI_NODE_INIT_API(module, method, proc_exit, "proc_exit");
  WASI_NODE_INIT_API(module, method, fd_fdstat_get, "fd_fdstat_get");

  // module.exports.wasi_unstable
  if (napi_set_named_property(env, exports, "wasi_unstable", module) != napi_ok) return nullptr;
  // module.exports.init
  if (napi_create_function(env, nullptr, 0, initInstance, nullptr, &method) != napi_ok) return nullptr;
  if (napi_set_named_property(env, exports, "init", method) != napi_ok) return nullptr;
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, initialize)

} // namespace wasi_node