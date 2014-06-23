/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id stream.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2014/06/21 12:32
 * @uses net socket stream class
 */
#ifndef PS_COMMON_NET_SOCKET_STREAM_H_
#define PS_COMMON_NET_SOCKET_STREAM_H_

#include "common/net/socket/base.h"

namespace ps_common_net {

namespace socket {

class Stream {

 public:
   Stream(
       Base* socket, 
       uint32_t bufferlength = SOCKETINPUT_BUFFERSIZE_DEFAULT, 
       uint32_t bufferlength_max = SOCKETINPUT_DISCONNECT_MAXSIZE); 
   virtual ~Stream();

 public:
   void init();
   bool resize(int32_t size);
   uint32_t reallength();
   void cleanup();
   void setkey(unsigned char const* key);
   int32_t get_keylength();
   Base* getsocket();

 protected:
   Base* socket_;
   streamdata_t streamdata_;
   encodeparam_t encodeparam_;

};

}; //namespace socket

}; //namespace ps_common_net

#endif //PS_COMMON_NET_SOCKET_STREAM_H_
