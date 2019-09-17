#define NAPI_EXPERIMENTAL
#include <node_api.h>

#include <tuple>

extern "C"{
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
#include <syslimits.h>
#include <string.h>
}

#include "api.hpp"

#define WASI_NODE_INIT_API(module, data, method, func, name) \
  if (napi_create_function(env, nullptr, 0, func, data, &method) != napi_ok) return nullptr; \
  if (napi_set_named_property(env, module, name, method) != napi_ok) return nullptr;

namespace wasi_node {

napi_value init_instance(napi_env env, napi_callback_info cbinfo) {
  // Get arguments
  /*
    argv[0]: instance
    argv[1]: options
  */
  size_t argc = 2;
  napi_value argv[2];
  napi_ref contextRef;
  if(napi_get_cb_info(env, cbinfo, &argc, argv, nullptr, (void**)&contextRef) != napi_ok) return nullptr;
  // Get context
  napi_value context;
  if(napi_get_reference_value(env, contextRef, &context) != napi_ok) return nullptr;
  // Set memory to context
  napi_value exports;
  napi_value exportKey;
  if(napi_create_string_utf8(env, "exports", 7, &exportKey) != napi_ok) return nullptr;
  if(napi_get_property(env, argv[0], exportKey, &exports) != napi_ok) return nullptr;
  napi_value memoryKey;
  if(napi_create_string_utf8(env, "memory", 6, &memoryKey) != napi_ok) return nullptr;
  napi_value memory;
  if(napi_get_property(env, exports, memoryKey, &memory) != napi_ok) return nullptr;
  if(napi_set_property(env, context, memoryKey, memory) != napi_ok) return nullptr;
  // Set capable path to context & open
  napi_value capablePath;
  napi_value capablePathKey;
  uint32_t pathLen;
  if(napi_create_string_utf8(env, "capable_path", 12, &capablePathKey) != napi_ok) return nullptr;
  if(napi_get_property(env, argv[1], capablePathKey, &capablePath) != napi_ok) return nullptr;
  if(napi_set_property(env, context, capablePathKey, capablePath) != napi_ok) return nullptr;
  if(napi_get_array_length(env, capablePath, &pathLen) != napi_ok) return nullptr;
  for(uint32_t i = 0; i < pathLen; ++i){
    napi_value elem;
    if(napi_get_element(env, capablePath, i, &elem) != napi_ok) return nullptr;
    char path[PATH_MAX];
    memset(path, '\0', PATH_MAX);
    if(napi_get_value_string_utf8(env,elem, path, PATH_MAX, nullptr) != napi_ok) return nullptr;
    open(path, O_DIRECTORY | O_PATH | O_RDWR);
  }
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
  if (napi_create_function(env, nullptr, 0, init_instance, contextRef, &method) != napi_ok) return nullptr;
  if (napi_set_named_property(env, exports, "init", method) != napi_ok) return nullptr;
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, initialize)

} // namespace wasi_node