/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id outputstream.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2014/06/21 12:03
 * @uses socket output stream class
 */
#ifndef PS_COMMON_NET_SOCKET_OUTPUTSTREAM_H_
#define PS_COMMON_NET_SOCKET_OUTPUTSTREAM_H_

#include "common/net/socket/base.h"
#include "common/net/packet/base.h"

namespace ps_common_net {

namespace socket {

class OutputStream {

 public:
   OutputStream(
     socket::Base* socket, 
       uint32_t bufferlength = SOCKETOUTPUT_BUFFERSIZE_DEFAULT,
       uint32_t bufferlength_max = SOCKETOUTPUT_DISCONNECT_MAXSIZE)
     : Stream(socket, bufferlength, bufferlength_max) {};
   ~OutputStream() {};

 public:
   uint32_t write(const char* buffer, uint32_t length);
   bool writepacket(const packet::Base* packet);
   int32_t flush();

};

}; //namespace socket

}; //namespace ps_common_net


#endif //PS_COMMON_NET_SOCKET_OUTPUTSTREAM_H_
