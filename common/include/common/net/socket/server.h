/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id server.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2014/06/20 11:18
 * @uses server net model socket class
 */
#ifndef PS_COMMON_NET_SOCKET_SERVER_H_
#define PS_COMMON_NET_SOCKET_SERVER_H_

#include "common/net/socket/base.h"

namespace ps_common_net {

namespace socket {

class Server {

 public:
   Server(uint16_t port, uint32_t backlog = 5);
   ~Server();

 public:
   void close();
   bool accept(ps_common_net::socket::Base* socket);
   uint32_t getlinger() const;
   bool setlinger(uint32_t lingertime);
   bool is_nonblocking() const;
   bool set_nonblocking(bool on = true);
   uint32_t getreceive_buffersize() const;
   bool setreceive_buffersize(uint32_t size);
   uint32_t getsend_buffersize() const;
   bool setsend_buffersize(uint32_t size);
   int32_t getid() const;

 protected:
   ps_common_net::socket::Base* socket_;

};

}; //namespace socket

}; //namespace ps_common_net

#endif //PS_COMMON_NET_SOCKET_SERVER_H_
