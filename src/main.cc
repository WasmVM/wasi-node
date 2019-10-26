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
#include "util.hpp"

#define WASI_NODE_INIT_API(module, data, method, func, name) \
  if (napi_create_function(env, nullptr, 0, func, data, &method) != napi_ok) \
    return wasi_node_throw_error(env, nullptr, "[constructor] Cannot create API function of " name); \
  if (napi_set_named_property(env, module, name, method) != napi_ok) \
    return wasi_node_throw_error(env, nullptr, "[constructor] Error register API  " name); \

namespace wasi_node {

napi_value initialize(napi_env env, napi_callback_info cbinfo){
  // Get argument & context
  size_t argc = 1;
  napi_value argv[1];
  napi_ref contextRef;
  if(napi_get_cb_info(env, cbinfo, &argc, argv, nullptr, (void**)&contextRef) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[initialize] Cannot get argument");
  napi_value context;
  if(napi_get_reference_value(env, contextRef, &context) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[initialize] Cannot get context");
  // Set memory to context
  napi_value instance;
  if(napi_get_named_property(env, argv[0], "instance", &instance) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[initialize] Cannot get instance from initialize promise");
  napi_value exports;
  if(napi_get_named_property(env, instance, "exports", &exports) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[initialize] Cannot get exports from instance");
  napi_value memory;
  if(napi_get_named_property(env, exports, "memory", &memory) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[initialize] Cannot get memory from instance.exports");
  if(napi_set_named_property(env, context, "memory", memory) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[initialize] Error set memory to context");
  return argv[0];
}

napi_value constructor(napi_env env, napi_callback_info cbinfo) {
  // Get arguments
  /*
    argv[0]: wasm binary data
    argv[1]: options
      {
        argv: [], // command line arguments pass to main function
        capable_path: [], // Pathes regarded as capable 
      }
  */
  size_t argc = 2;
  napi_value argv[2];
  if(napi_get_cb_info(env, cbinfo, &argc, argv, nullptr, nullptr) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[constructor] Cannot get arguments");
  
  // Create imports object
  napi_value imports;
  if (napi_create_object(env, &imports) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[constructor] Cannot create imports object");
  // Create wasi_unstable object and set to imports
  napi_value wasi_unstable;
  if (napi_create_object(env, &wasi_unstable) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[constructor] Cannot create wasi_unstable object");
  if(napi_set_named_property(env, imports, "wasi_unstable", wasi_unstable) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[constructor] Error set wasi_unstable to imports object");
  // Create context & context reference
  napi_value context;
  if (napi_create_object(env, &context) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[constructor] Cannot create context object");
  napi_ref contextRef;
  if (napi_create_reference(env, context, 1, &contextRef) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[constructor] Cannot create context reference");

  /* Options */
  // Set capable path to context & open
  bool hasCapablePath;
  if(napi_has_named_property(env, argv[1], "capable_path", &hasCapablePath) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[constructor] Error determine whether capable_path option exist or not");
  if(hasCapablePath){
    napi_value capablePath;
    uint32_t pathLen;
    if(napi_get_named_property(env, argv[1], "capable_path", &capablePath) != napi_ok)
      return wasi_node_throw_error(env, nullptr, "[constructor] Cannot get capable_path");
    if(napi_set_named_property(env, context, "capable_path", capablePath) != napi_ok)
      return wasi_node_throw_error(env, nullptr, "[constructor] Cannot set capable_path to context");
    if(napi_get_array_length(env, capablePath, &pathLen) != napi_ok)
      return wasi_node_throw_error(env, nullptr, "[constructor] Cannot get capable_path length");
    for(uint32_t i = 0; i < pathLen; ++i){
      napi_value elem;
      if(napi_get_element(env, capablePath, i, &elem) != napi_ok)
        return wasi_node_throw_error(env, nullptr, "[constructor] Cannot get capable_path element");
      char path[PATH_MAX];
      memset(path, '\0', PATH_MAX);
      if(napi_get_value_string_utf8(env,elem, path, PATH_MAX, nullptr) != napi_ok)
        return wasi_node_throw_error(env, nullptr, "[constructor] Cannot get path from capable_path");
      if(open(path, O_DIRECTORY | O_PATH | O_RDWR) == -1){
        return wasi_node_throw_error(env, nullptr, "[constructor] Cannot open path");
      }
    }
  }

  // Set process argv to context
  bool hasProcessArgv;
  if(napi_has_named_property(env, argv[1], "argv", &hasCapablePath) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[constructor] Error determine whether argv option exist or not");
  if(hasProcessArgv){
    napi_value process_argv;
    if(napi_get_named_property(env, argv[1], "argv", &process_argv) != napi_ok)
      return wasi_node_throw_error(env, nullptr, "[constructor] Cannot get process argv");
    if(napi_set_named_property(env, context, "argv", process_argv) != napi_ok)
      return wasi_node_throw_error(env, nullptr, "[constructor] Error set process argv to context");
  }

  /* Create & register APIs */
  napi_value method;
  WASI_NODE_INIT_API(wasi_unstable, contextRef, method, fd_prestat_get, "fd_prestat_get");
  WASI_NODE_INIT_API(wasi_unstable, contextRef, method, fd_prestat_dir_name, "fd_prestat_dir_name");
  WASI_NODE_INIT_API(wasi_unstable, contextRef, method, environ_sizes_get, "environ_sizes_get");
  WASI_NODE_INIT_API(wasi_unstable, contextRef, method, environ_get, "environ_get");
  WASI_NODE_INIT_API(wasi_unstable, contextRef, method, args_sizes_get, "args_sizes_get");
  WASI_NODE_INIT_API(wasi_unstable, contextRef, method, args_get, "args_get");
  WASI_NODE_INIT_API(wasi_unstable, contextRef, method, proc_exit, "proc_exit");
  WASI_NODE_INIT_API(wasi_unstable, contextRef, method, fd_fdstat_get, "fd_fdstat_get");

  // Get global object
  napi_value global;
  if(napi_get_global(env, &global) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[constructor] Cannot get global object");
  // Get WebAssembly
  napi_value webAssembly;
  if(napi_get_named_property(env, global, "WebAssembly", &webAssembly) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[constructor] Cannot get WebAssembly object");
  // Call instantiate
  napi_value instantiate;
  if(napi_get_named_property(env, webAssembly, "instantiate", &instantiate) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[constructor] Cannot get WebAssembly.instantiate function");
  napi_value instantiate_result;
  napi_value instantiate_result_args[2] = {argv[0], imports};
  if(napi_call_function(env, webAssembly, instantiate, 2, instantiate_result_args, &instantiate_result) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[constructor] Error calling WebAssembly.instantiate function");
  // Initialize wasi-node after instantiate
  napi_value initialize_func;
  if(napi_create_function(env, nullptr, 0, initialize, contextRef, &initialize_func) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[constructor] Cannot create initialize function");
  // Call .then of instantiate result & pass initialize as callback
  napi_value instantiate_then;
  if(napi_get_named_property(env, instantiate_result, "then", &instantiate_then) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[constructor] Cannot get WebAssembly.instantiate().then function");
  napi_value result;
  if(napi_call_function(env, instantiate_result, instantiate_then, 1, &initialize_func, &result) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[constructor] Error calling WebAssembly.instantiate().then function");
  return result;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, [](napi_env env, napi_value exports){
  if (napi_create_function(env, nullptr, 0, constructor, nullptr, &exports) != napi_ok)
    return wasi_node_throw_error(env, nullptr, "[module] Cannot create function of module");
  return exports;
})

} // namespace wasi_node