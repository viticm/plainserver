/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id manager.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/23 14:58
 * @uses net manager class
 *       根据不同的服务器，可以继承该类来实现不同的需求
 */
#ifndef PS_COMMON_NET_MANAGER_H_
#define PS_COMMON_NET_MANAGER_H_

#include "common/base/singleton.h"
#include "common/sys/thread.h"
#include "common/net/connection/pool.h"
#include "common/net/connection/manager.h"
#include "common/net/connection/server.h"
#include "common/net/socket/server.h"

namespace ps_common_net {

class Manager : public connection::Manager {

 public:
   Manager(uint16_t port = 0);
   ~Manager();

/**
 public:
   static Manager* getsingleton_pointer();
   static Manager& getsingleton();
**/

 public:
   bool init(uint16_t connectionmax = NET_CONNECTION_MAX); //初始化
   bool select(); //网络侦测
   bool processinput(); //数据接收接口
   bool processoutput(); //数据发送接口
   bool processexception(); //异常连接处理
   bool processcommand(); //消息执行
   bool accept_newconnection(); //新连接接收处理
   virtual bool heartbeat();
   void loop();
   bool isactive();
   void setactive(bool active);

 public:
   //将connection数据加入系统中
   bool addconnection(connection::Base* connection);
   //将拥有fd句柄的玩家(服务器)数据从当前系统中清除
   bool deleteconnection(connection::Base* connection);
   //出现异常后将connection信息清除，并将系统中的信息也清除 断开玩家(服务器)的连接
   bool removeconnection(connection::Base* connection);
   void removeconnection(int16_t id);
   void remove_allconnection();
   //获得服务器连接指针
   connection::Server* get_serverconnection(uint16_t id);
   //服务器广播
   void broadcast(packet::Base* packet);
   bool connectserver(); //just test

 public:
   int32_t get_onestep_accept() const;
   void set_onestep_accept(int32_t count);
   uint64_t get_send_bytes() const;
   uint64_t get_receive_bytes() const;
   uint16_t get_listenport() const;
   uint16_t get_connectionmax() const;

 public:
   uint64_t threadid_;
   int16_t serverhash_[NET_OVER_SERVER_MAX]; //服务器连接ID池

 protected:
   //用于侦听的服务器Socket
   socket::Server* serversocket_;
   //用于侦听的服务器SOCKET句柄值（此数据即serversocket_内拥有的SOCKET句柄值）
   int32_t socketid_;
   //网络相关数据
   enum {
     kSelectFull = 0, //当前系统中拥有的完整句柄数据
     kSelectUse, //用于select调用的句柄数据
     kSelectMax,
   };
   fd_set readfds_[kSelectMax];
   fd_set writefds_[kSelectMax];
   fd_set exceptfds_[kSelectMax];
   timeval timeout_[kSelectMax];
   uint16_t listenport_;
   uint16_t connectionmax_;
   int32_t maxfd_;
   int32_t minfd_;
   int32_t fdsize_;
   bool active_;
   uint64_t send_bytes_; //发送字节数
   uint64_t receive_bytes_; //接收字节数
   int32_t onestep_accept_; //一帧内接受的新连接数量, -1无限制
   connection::Pool connectionpool_;   
   connection::Server billing_serverconnection_; //for debug

};

}; //namespace ps_common_net

//extern ps_common_net::Manager* g_netmanager;

#endif //PS_COMMON_NET_MANAGER_H_
