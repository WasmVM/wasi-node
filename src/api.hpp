#ifndef WASI_NODE_APIS
#define WASI_NODE_APIS

// #define NAPI_EXPERIMENTAL // Some of the N-API surface is considered experimental and requires explicit opt-in to access those APIs 
#include <node_api.h>

namespace wasi_node {

napi_value fd_prestat_get(napi_env env, napi_callback_info args);

}

#endif