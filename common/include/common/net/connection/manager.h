/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id manager.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/23 14:00
 * @uses connection manager class
 */
#ifndef PS_COMMON_NET_CONNECTION_MANAGER_H_
#define PS_COMMON_NET_CONNECTION_MANAGER_H_

#include "common/net/socket/base.h"
#include "common/net/packet/base.h"
#include "common/net/connection/base.h"

namespace ps_common_net {

namespace connection {

class Manager {

 public:
   Manager();
   ~Manager();

 public:
   void cleanup();
   virtual bool heartbeat(uint32_t time = 0);
   bool add(Base* connection);
   bool add(int16_t id);
   virtual void remove(int16_t id);
   int16_t* get_idset();
   uint16_t getcount();
   bool hash();

 protected:
   int16_t connection_idset_[NET_CONNECTION_MAX];
   uint16_t count_;

};

}; //namespace connection

}; //namespace ps_common_net

#endif //PS_COMMON_NET_CONNECTION_MANAGER_H_
