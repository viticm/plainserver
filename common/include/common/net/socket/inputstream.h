/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id inputstream.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2014/06/20 11:45
 * @uses socket inputstream class
 */
#ifndef PS_COMMON_NET_SOCKET_INPUTSTREAM_H_
#define PS_COMMON_NET_SOCKET_INPUTSTREAM_H_

#include "common/net/socket/config.h"
#include "common/net/socket/base.h"
#include "common/net/packet/base.h"

namespace pap_common_net {

namespace socket {

class InputStream {

 public: //construct and destruct
   InputStream(
       Base* socket, 
       uint32_t bufferlength = SOCKETINPUT_BUFFERSIZE_DEFAULT, 
       uint32_t bufferlength_max = SOCKETINPUT_DISCONNECT_MAXSIZE);
   virtual ~InputStream();
   
 public:
   uint32_t read(char* buffer, uint32_t length);
   bool readpacket(packet::Base* packet);
   bool peek(char* buffer, uint32_t length);
   bool skip(uint32_t length);
   uint32_t fill();
   void init();
   bool resize(int32_t size);
   uint32_t reallength();
   bool isempty();
   void cleanup();
   void setkey(unsigned char const* key);
   int32_t get_keylength();
   Base* getsocket();

 private:
   Base* socket_;
   struct streamdata_t streamdata_;
   struct encode_param_t encode_param_;

};

}; //namespace socket

}; //namespace ps_common_net

#endif //PS_COMMON_NET_SOCKET_INPUTSTREAM_H_
