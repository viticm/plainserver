#include "common/net/socket/encode.h"

namespace ps_common_net {

namespace socket {

namespace encode {

bool make(struct encode_param_t* encode_param) {
  
  unsigned char const* in;
  uint32_t insize;
  unsigned char* out;
  uint32_t outsize;
  unsigned char const* key;
  uint32_t keysize;
  int32_t keyindex;
  int32_t index;
  in = (*encode_param).in;
  if(NULL == in) {
    return false;
  }
  insize = (*encode_param).insize;
  if(insize <= 0) {
    return false;
  }
  out = encode_param->out;
  if(NULL == out) {
    return false;
  }
  outsize = (*encode_param).outsize;
  if(outsize <= 0 || outsize < insize) {
    return false;
  }
  key = (*encode_param).key;
  if(NULL == key) {
    return false;
  }
  keysize = (*encode_param).keysize;
  if(keysize <= 0) {
    return false;
  }
  keyindex = (*encode_param).param[0];
  for(index = 0; (int32_t)insize > index; ++index) {
    out[index] = in[index] ^ key[keyindex];
    ++keyindex;
    if(keyindex >= (int32_t)keysize) {
      keyindex -=keyindex;
    }
  }
  encode_param->param[0] = keyindex;
  return true;
}

bool skip(struct encode_param_t* encode_param, int32_t length) {
  uint32_t keysize = 0;
  int32_t keyindex = 0;
  int32_t index = 0;
  keysize = (*encode_param).keysize;
  if(keysize == 0) {
    return false;
  }
  keyindex = (*encode_param).param[0];
  for(index = 0; index < length; ++index) {
    ++keyindex;
    if(keyindex >= (int32_t)keysize) {
      keyindex -= keysize;
    }
  }
  encode_param->param[0] = keyindex;
  return true;
}

}; //namespace encode

}; //namespace socket

}; //namespace ps_common_net
