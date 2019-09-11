#include <node_api.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wasi_core.h"
#include "api.hpp"
#include "util.hpp"

namespace wasi_node {

napi_value fd_prestat_get(napi_env env, napi_callback_info cbinfo) {
  // Unfold callback info
  size_t argc = 2;
  napi_value argv[2];
  napi_ref contextRef;
  if(napi_get_cb_info(env, cbinfo, &argc, argv, nullptr, (void**)&contextRef) != napi_ok) return nullptr;

  // Get context
  uint8_t *memory = nullptr;
  size_t memorySize;
  if(napi_get_wasi_memory(env, contextRef, &memory, &memorySize) != napi_ok) return nullptr;
  printf("memory: %p, size: %lu\n", memory, memorySize);

  // Get args
  __wasi_fd_t fd;
  if(napi_get_value_uint32(env, argv[0], &fd)) return nullptr;
  printf("fd: %d\n", fd);

  // Result
  napi_value result;
  //if(napi_create_int32(env, __WASI_ESUCCESS, &result) != napi_ok) return nullptr; 
  if(napi_create_int32(env, 8, &result) != napi_ok) return nullptr; //FIXME:
  return result;
}

} // namespace wasi_node