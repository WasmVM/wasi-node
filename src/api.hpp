#ifndef WASI_NODE_APIS
#define WASI_NODE_APIS

#include <node_api.h>
#include <stdint.h>

#define napi_get_pointer_value(env, value, result, lossless) \
  (sizeof(void*) == sizeof(uint32_t)) ? napi_get_value_uint32(env, value, result) : napi_get_value_bigint_int64(env, value, result, lossless)

namespace wasi_node {

napi_value fd_prestat_get(napi_env env, napi_callback_info args);

}

#endif