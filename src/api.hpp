#ifndef WASI_NODE_APIS
#define WASI_NODE_APIS

#include <node_api.h>

namespace wasi_node {

napi_value fd_prestat_get(napi_env env, napi_callback_info args);
napi_value fd_prestat_dir_name(napi_env env, napi_callback_info args);
napi_value environ_sizes_get(napi_env env, napi_callback_info args);
napi_value environ_get(napi_env env, napi_callback_info args);
napi_value args_sizes_get(napi_env env, napi_callback_info args);
napi_value args_get(napi_env env, napi_callback_info args);
napi_value proc_exit(napi_env env, napi_callback_info args);
napi_value fd_fdstat_get(napi_env env, napi_callback_info args);

}

#endif