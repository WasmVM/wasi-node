#include <node_api.h>

#include "api.hpp"

#define WASI_NODE_INIT_API(method, name) \
  if (napi_create_function(env, nullptr, 0, method, nullptr, &module) != napi_ok) return nullptr; \
  if (napi_set_named_property(env, exports, name, module) != napi_ok) return nullptr;

namespace wasi_node {

napi_value initialize(napi_env env, napi_value exports) {
  napi_value module;

  WASI_NODE_INIT_API(fd_prestat_get, "fd_prestat_get");

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, initialize)

} // namespace wasi_node