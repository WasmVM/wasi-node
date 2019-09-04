#include <node_api.h>

#include "api.hpp"

namespace wasi_node {

napi_value fd_prestat_get(napi_env env, napi_callback_info args) {
  
  napi_value greeting;

  if (napi_create_string_utf8(env, "fd_prestat_get", NAPI_AUTO_LENGTH, &greeting) != napi_ok){
    return nullptr;
  }

  return greeting;
}

} // namespace wasi_node