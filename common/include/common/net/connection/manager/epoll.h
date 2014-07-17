/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id epoll.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/07/17 13:40
 * @uses connection manager with select mode
 */
#ifndef PS_COMMON_NET_CONNECTION_MANAGER_EPOLL_H_
#define PS_COMMON_NET_CONNECTION_MANAGER_EPOLL_H_

#if __LINUX__ && defined(_PS_NET_EPOLL)
#include "common/net/connection/manager/config.h"
#include "common/net/connection/manager/base.h"
#include "common/net/socket/extend.inl"

namespace ps_common_net {

namespace connection {

namespace manager {

class Epoll : public Base {

 public:
   Epoll();
   ~Epoll();

 public:
   bool init(uint16_t connectionmax = NET_CONNECTION_MAX,
             uint16_t listenport = 0,
             const char *listenip = NULL);
   bool select(); //网络侦测
   bool processinput(); //数据接收接口
   bool processoutput(); //数据发送接口
   bool processexception(); //异常连接处理
   bool processcommand(); //消息执行
   virtual bool heartbeat();
   bool set_poll_maxcount(uint16_t maxcount);

 public:
   bool addsocket(int32_t socketid, int16_t connectionid);
   //将拥有fd句柄的玩家(服务器)数据从当前系统中清除
   virtual bool removesocket(int32_t socketid);

 protected:
   polldata_t polldata_;

};

}; //namespace manager

}; //namespace connection

}; //namespace ps_common_net


#endif

#endif //PS_COMMON_NET_CONNECTION_MANAGER_EPOLL_H_
