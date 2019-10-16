#ifndef WASI_NODE_UTIL
#define WASI_NODE_UTIL

#define NAPI_EXPERIMENTAL

#include <node_api.h>
#include <stdint.h>

#define WASI_NODE_IGNORE_SIZE_CHECK 0

napi_status napi_get_wasi_memory(napi_env env, napi_ref contextRef, uint8_t **memoryPtr, size_t *memorySizePtr);
napi_status napi_get_wasi_pointer(napi_env env, napi_value value, napi_ref contextRef, void **resultPtr, size_t resultSize);

#endif