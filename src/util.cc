#include "util.hpp"

napi_status napi_get_wasi_memory(napi_env env, napi_ref contextRef, uint8_t **memoryPtr, size_t *memorySizePtr){
  napi_status status = napi_ok;
  napi_value context;
  status = napi_get_reference_value(env, contextRef, &context);
  if(status != napi_ok){
    napi_throw_error(env, nullptr, "context");  // TODO:
    return status;
  }
  napi_value memoryKey;
  status = napi_create_string_utf8(env, "memory", 6, &memoryKey);
  if(status != napi_ok){
    napi_throw_error(env, nullptr, "memkey");  // TODO:
    return status;
  }
  napi_value memoryObj;
  status = napi_get_property(env, context, memoryKey, &memoryObj);
  if(status != napi_ok){
    napi_throw_error(env, nullptr, "memobj");  // TODO:
    return status;
  }
  napi_value bufferKey;
  status = napi_create_string_utf8(env, "buffer", 6, &bufferKey);
  if(status != napi_ok){
    napi_throw_error(env, nullptr, "bufferkey");  // TODO:
    return status;
  }
  napi_value buffer;
  status = napi_get_property(env, memoryObj, bufferKey, &buffer);
  if(status != napi_ok){
    napi_throw_error(env, nullptr, "buffer");  // TODO:
    return status;
  }
  status = napi_get_arraybuffer_info(env, buffer, (void**)memoryPtr, memorySizePtr);
  if(status != napi_ok){
    napi_throw_error(env, nullptr, "memory"); // TODO:
    return status;
  }
  return status;
}