#include <node_api.h>

#include "api.hpp"

#include <stdio.h>

#define WASI_NODE_INIT_API(module, data, method, func, name) \
  if (napi_create_function(env, nullptr, 0, func, data, &method) != napi_ok) return nullptr; \
  if (napi_set_named_property(env, module, name, method) != napi_ok) return nullptr;

namespace wasi_node {

napi_value initInstance(napi_env env, napi_callback_info cbinfo) {
  // Get instance
  size_t argc = 1;
  napi_value instance;
  napi_ref contextRef;
  if(napi_get_cb_info(env, cbinfo, &argc, &instance, nullptr, (void**)&contextRef) != napi_ok) return nullptr;
  // Get this name
  napi_value context;
  if(napi_get_reference_value(env, contextRef, &context) != napi_ok) return nullptr;
  napi_value exports;
  napi_value exportKey;
  if(napi_create_string_utf8(env, "exports", 7, &exportKey) != napi_ok) return nullptr;
  if(napi_get_property(env, instance, exportKey, &exports) != napi_ok) return nullptr;
  napi_value memoryKey;
  if(napi_create_string_utf8(env, "memory", 6, &memoryKey) != napi_ok) return nullptr;
  napi_value memory;
  if(napi_get_property(env, exports, memoryKey, &memory) != napi_ok) return nullptr;
  if(napi_set_property(env, context, memoryKey, memory) != napi_ok) return nullptr;
  return nullptr;
}

napi_value initialize(napi_env env, napi_value exports) {
  napi_value module;
  if (napi_create_object(env, &module) != napi_ok) return nullptr;
  
  // API methods
  napi_value method;
  napi_ref contextRef;
  napi_value context;
  if (napi_create_object(env, &context) != napi_ok) return nullptr;
  if (napi_create_reference(env, context, 1, &contextRef) != napi_ok) return nullptr;
  WASI_NODE_INIT_API(module, contextRef, method, fd_prestat_get, "fd_prestat_get");
  WASI_NODE_INIT_API(module, contextRef, method, fd_prestat_dir_name, "fd_prestat_dir_name");
  WASI_NODE_INIT_API(module, contextRef, method, environ_sizes_get, "environ_sizes_get");
  WASI_NODE_INIT_API(module, contextRef, method, environ_get, "environ_get");
  WASI_NODE_INIT_API(module, contextRef, method, args_sizes_get, "args_sizes_get");
  WASI_NODE_INIT_API(module, contextRef, method, args_get, "args_get");
  WASI_NODE_INIT_API(module, contextRef, method, proc_exit, "proc_exit");
  WASI_NODE_INIT_API(module, contextRef, method, fd_fdstat_get, "fd_fdstat_get");

  // module.exports.wasi_unstable
  if (napi_set_named_property(env, exports, "wasi_unstable", module) != napi_ok) return nullptr;
  // module.exports.init
  if (napi_create_function(env, nullptr, 0, initInstance, contextRef, &method) != napi_ok) return nullptr;
  if (napi_set_named_property(env, exports, "init", method) != napi_ok) return nullptr;
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, initialize)

} // namespace wasi_node