#include "common/base/util.h"
#include "common/net/socket/encode.h"
#include "common/net/socket/inputstream.h"

namespace pap_common_net {

namespace socket {

InputStream::InputStream(Base* socket, 
                         uint32_t bufferlength,
                         uint32_t bufferlength_max) {
  __ENTER_FUNCTION
    socket_ = socket;
    memset(&packet_, 0, sizeof(packet_));
    memset(&encode_param_, 0, sizeof(encode_param_));
    packet_.bufferlength = bufferlength;
    packet_.bufferlength_max = bufferlength_max;
    packet_.headlength = 0;
    packet_.taillength = 0;
    packet_.buffer = new char[sizeof(char) * bufferlength];
  __LEAVE_FUNCTION
}

InputStream::~InputStream() {
  __ENTER_FUNCTION
    SAFE_DELETE_ARRAY(packet_.buffer);
  __LEAVE_FUNCTION
}

uint32_t InputStream::read(char* buffer, uint32_t length) {
  __ENTER_FUNCTION
    uint32_t result = length; //normal state
    char* stream_buffer = streamdata_.buffer;
    uint32_t bufferlength = streamdata_.bufferlength;
    uint32_t headlength = streamdata_.headlength;
    uint32_t taillength = streamdata_.taillength;
    if (0 == length || length > reallength()) return 0;
    char* tempbuffer = new char[sizeof(char) * length];
    if (0 == tempbuffer) return 0;
    if (headlength < taillength) {
      memcpy(tempbuffer, &stream_buffer[headlength], length);
    }
    else {
      uint32_t rightlength = bufferlength - headlength;
      if (length < rightlength) {
        memcpy(tempbuffer, &stream_buffer[headlength], length);
      }
      else {
        memcpy(tempbuffer, &stream_buffer[headlength], rightlength);
        memcpy(&(tempbuffer[rightlength]), stream_buffer, length - rightlength);
      }
    }
    streamdata_.headlength = (headlength + length) % bufferlength;
    if (encode_param_.keysize > 0) {
      bool encode_result = true;
      encode_param_.in = tempbuffer;
      encode_param_.insize = length;
      encode_param_.out = reinterpret_cast<unsigned char*>(buffer);
      encode_param_.outsize = length;
      encode_result = encode::make(encode_param_);
      if (!encode_result) result = 0;
    }
    else {
      memcpy(buffer, tempbuffer, length);
    }
    SAFE_DELETE_ARRAY(tempbuffer);
    return result;
  __LEAVE_FUNCTION
    return 0;
}

bool InputStream::readpacket(packet::Base* packet) {
  __ENTER_FUNCTION
    bool result = false;
    result = skip(PACKET_HEADERSIZE);
    if (!result) return result;
    result = packet->read(*this);
    return result;
  __LEAVE_FUNCTION
    return false;
}

bool InputStream::peek(char* buffer, uint32_t length) {
  __ENTER_FUNCTION
    if (0 == length || length > reallength()) {
      return false;
    }
    uint32_t bufferlength = streamdata_.bufferlength;
    uint32_t headlength = streamdata_.headlength;
    uint32_t taillength = streamdata_.taillength;
    char* stream_buffer = streamdata_.buffer;
    if (headlength < taillength) {
      memcpy(buffer, &(stream_buffer[headlength]), length);
    }
    else {
      uint32_t rightlength = bufferlength - headlength;
      if (length < rightlength) {
        memcpy(&buffer[0], &(stream_buffer[headlength]), length);
      }
      else {
        memcpy(&buffer[0], &(stream_buffer[headlength]), rightlength);
        memcpy(&buffer[rightlength], &(stream_buffer[0]), length - rightlength);
      }
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool InputStream::skip(uint32_t length) {
  __ENTER_FUNCTION
    if (0 == length || length > reallength()) return false;
    bool result = true;
    uint32_t headlength = streamdata_.headlength;
    uint32_t bufferlength = streamdata_.bufferlength;
    streamdata_.headlength = (headlength + length) % bufferlength;
    if (encode_param_.keysize > 0) {
      encode_param_.in = NULL;
      encode_param_.insize = 0;
      encode_param_.out = NULL;
      encode_param_.outsize = 0;
      result = encode::skip(encode_param_, length);
    }
    return result;
  __LEAVE_FUNCTION
    return false;
}

uint32_t InputStream::fill() {
  __ENTER_FUNCTION
    uint32_t result = 0;
    if (!socket_->isvalid()) return result;
    int32_t socketid = socket_->getid();
    int32_t fillcount = 0;
    int32_t receivecount = 0;
    int32_t freecount = 0;
    uint32_t bufferlength = streamdata_.bufferlength;
    uint32_t bufferlength_max = streamdata_.bufferlength_max;
    uint32_t headlength = streamdata_.headlength;
    uint32_t taillength = streamdata_.taillength;
    char* stream_buffer = streamdata_.buffer;
    // head tail length=10
    // 0123456789
    // abcd......
    if (headlength < taillength) {
      freecount = 0 == headlength ? 
                  bufferlength - taillength - 1 : 
                  bufferlength - headlength;
    }
    else {
      freecount = headlength - taillength - 1;
    }
    if (freecount != 0) {
      receivecount = 
        api::recvex(scoketid, &stream_buffer[taillength], freecount, 0);
      if (SOCKET_ERROR_WOULD_BLOCK == receivecount) return 0;
      if (SOCKET_ERROR == receivecount) return SOCKET_ERROR - 1;
      if (0 == receivecount) return SOCKET_ERROR - 2;
      streamdata_.taillength += receivecount;
      fillcount += receivecount;
    }
    if (receivecount == freecount) {
      
    }
    return result;
  __LEAVE_FUNCTION
    return 0;
}

void InputStream::init() {
  __ENTER_FUNCTION
    vnet_socket_inputstream_packetinit(packet_);
  __LEAVE_FUNCTION
}

bool InputStream::resize(int32_t size) {
  __ENTER_FUNCTION
    bool result = false;
    result = 1 == vnet_socket_inputstream_resize(packet_, size);
    return result;
  __LEAVE_FUNCTION
    return false;
}

uint32_t InputStream::reallength() {
  __ENTER_FUNCTION
    uint32_t length = 0;
    uint32_t bufferlength = streamdata_.bufferlength;
    uint32_t headlength = streamdata_.headlength;
    uint32_t taillength = streamdata_.taillength;
    if (headlength < taillength) {
      length = taillength - headlength;
    }
    else {
      length = bufferlength - headlength + taillength;
    }
    return length;
  __LEAVE_FUNCTION
    return 0;
}

bool InputStream::isempty() {
  return (*packet_).headlength == (*packet_).taillength;
}

void InputStream::cleanup() {
  __ENTER_FUNCTION
    vnet_socket_inputstream_packetinit(packet_);
    endecode_param_ = NULL;
  __LEAVE_FUNCTION
}

void InputStream::setkey(unsigned char const* key) {
  __ENTER_FUNCTION
    SAFE_FREE(endecode_param_); //free last
    endecode_param_ = //malloc for it, construct not get memory 
      (struct endecode_param_t*)malloc(sizeof(struct endecode_param_t));
    endecode_param_->key = key;
    endecode_param_->keysize = strlen(reinterpret_cast<const char*>(key));
  __LEAVE_FUNCTION
}

int32_t InputStream::get_keylength() {
  int32_t result = (*endecode_param_).keysize;
  return result;
}

Base* InputStream::getsocket() {
  return socket_;
}

} //namespace socket

} //namespace pap_common_net
