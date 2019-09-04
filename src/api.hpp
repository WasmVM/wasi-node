#ifndef WASI_NODE_APIS
#define WASI_NODE_APIS

#include <node_api.h>

namespace wasi_node {

napi_value fd_prestat_get(napi_env env, napi_callback_info args);

}

#endif