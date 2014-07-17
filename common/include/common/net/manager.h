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
#if __LINUX__ && defined(_NET_EPOLL) /* { */
#include "common/net/connection/manager/epoll.h"
#elif __WINDOWS__ && defined(_NET_IOCP) /* }{ */
#include "common/net/connection/manager/iocp.h"
#else /* }{ */
#include "common/net/connection/manager/select.h"
#endif /* } */
#include "common/net/connection/base.h"
#include "common/net/socket/server.h"

namespace ps_common_net {

#if __LINUX__ && defined(_NET_EPOLL) /* { */
class Manager : public connection::manager::Epoll {
#elif __WINDOWS__ && defined(_NET_IOCP) /* }{ */
class Manager : public connection::manager::Iocp {
#else /* }{ */
class Manager : public connection::manager::Select {
#endif /* } */

 public:
   Manager();
   ~Manager();

 public:
   virtual bool heartbeat();
   void loop();
   bool isactive();
   void setactive(bool active);

 public:
   //服务器广播
   void broadcast(packet::Base *packet);

 protected:
   bool active_;

};

}; //namespace ps_common_net

#endif //PS_COMMON_NET_MANAGER_H_
