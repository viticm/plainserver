/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id server.h
 * @link https://github.com/viticm/plainserver for the canonical source repository
 * @copyright Copyright (c) 2013-2013 viticm( viticm@126.com )
 * @license
 * @user viticm<viticm@126.com>
 * @date 2014/06/23 14:13
 * @uses net server connection class
 *       cn: 服务器连接模块，用于服务器之间的连接，因为各个服务器都会用到，
 *           所以挪到此处，消费（网关）服务器与世界（中心）服务器需重写
 */
#ifndef PS_COMMON_NET_CONNECTION_SERVER_H_
#define PS_COMMON_NET_CONNECTION_SERVER_H_

#include "common/net/connection/base.h"

namespace ps_common_net {

namespace connection {

class Server : public Base {

 public:
   Server(bool isserver = true);
   ~Server();

 public:
   bool init();
   virtual bool processinput();
   virtual bool processoutput();
   virtual bool processcommand(bool option = true);
   virtual void cleanup();
   virtual bool heartbeat(uint32_t time = 0);
   virtual void freeown() {};

 public:
   virtual bool isserver();
   virtual bool isplayer();
   virtual bool islogin();
   virtual bool isbilling();
   void setstatus(uint32_t status);
   virtual bool isvalid();
   virtual bool sendpacket(packet::Base* packet);

 private:
   uint32_t status_;

};

}; //namespace connection

}; //namespace ps_common_net

#endif //PS_COMMON_NET_CONNECTION_SERVER_H_
