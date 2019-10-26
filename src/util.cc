#include "util.hpp"

napi_status napi_get_wasi_memory(napi_env env, napi_ref contextRef, uint8_t **memoryPtr, size_t *memorySizePtr){
  napi_status status = napi_ok;
  napi_value context;
  status = napi_get_reference_value(env, contextRef, &context);
  if(status != napi_ok){
    napi_throw_error(env, "ERR_WASI", "Can't retrieve reference of WebAssembly instance");
    return status;
  }
  napi_value memoryKey;
  status = napi_create_string_utf8(env, "memory", 6, &memoryKey);
  if(status != napi_ok){
    napi_throw_error(env, "ERR_WASI", "Can't create 'memory' string as key");
    return status;
  }
  napi_value memoryObj;
  status = napi_get_property(env, context, memoryKey, &memoryObj);
  if(status != napi_ok){
    napi_throw_error(env, "ERR_WASI", "Can't retrieve memory object");
    return status;
  }
  napi_value bufferKey;
  status = napi_create_string_utf8(env, "buffer", 6, &bufferKey);
  if(status != napi_ok){
    napi_throw_error(env, "ERR_WASI", "Can't create 'buffer' string as key");
    return status;
  }
  napi_value buffer;
  status = napi_get_property(env, memoryObj, bufferKey, &buffer);
  if(status != napi_ok){
    napi_throw_error(env, "ERR_WASI", "Can't retrieve buffer object");
    return status;
  }
  status = napi_get_arraybuffer_info(env, buffer, (void**)memoryPtr, memorySizePtr);
  if(status != napi_ok){
    napi_throw_error(env, "ERR_WASI", "Can't retrieve pointer to buffer object");
    return status;
  }
  return status;
}

napi_status napi_get_wasi_pointer(napi_env env, napi_value value, napi_ref contextRef, void **resultPtr, size_t resultSize){
  napi_status status = napi_ok;
  uint8_t *memory = nullptr;
  size_t memorySize;
  status = napi_get_wasi_memory(env, contextRef, &memory, &memorySize);
  if(status != napi_ok){
    return status;
  }
  uint32_t offset;
  status = napi_get_value_uint32(env, value, &offset);
  if(status != napi_ok){
    return status;
  }
  if(offset + resultSize > memorySize){
    napi_throw_range_error(env, nullptr, "Out of memory bound");
    return napi_invalid_arg;
  }
  *resultPtr = (void*)((intptr_t)memory + offset);
  return status;
}

napi_value wasi_node_throw_error(napi_env env, const char *code, const char *msg){
  napi_throw_error(env, code, msg);
  return nullptr;
}