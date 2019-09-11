#ifndef WASI_NODE_UTIL
#define WASI_NODE_UTIL

#include <node_api.h>
#include <stdint.h>

#define napi_get_pointer_value(env, value, result, lossless) \
  (sizeof(void*) == sizeof(uint32_t)) ? napi_get_value_uint32(env, value, result) : napi_get_value_bigint_int64(env, value, result, lossless)

napi_status napi_get_wasi_memory(napi_env env, napi_ref contextRef, uint8_t **memoryPtr, size_t *memorySizePtr);

#endif