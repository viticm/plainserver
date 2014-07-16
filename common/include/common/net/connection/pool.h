/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id pool.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2014/06/23 14:10
 * @uses net connection pool class
 */
#ifndef PS_NET_CONNECTION_POOL_H_
#define PS_NET_CONNECTION_POOL_H_

#include "common/net/connection/config.h"
#include "common/sys/thread.h"
#include "common/net/connection/base.h"

namespace ps_common_net {

namespace connection {

class Pool {

 public:
   Pool();
   ~Pool();

 public:
   bool init(uint32_t maxcount = NET_CONNECTION_POOL_SIZE_DEFAULT);
   Base *get(int16_t id);
   Base *create(); //new
   bool init_data(uint16_t index, Base* connection);
   void remove(int16_t id); //delete
   void lock();
   void unlock();
   uint32_t get_maxcount() const { return maxcount_; }   

 private:
   Base **connections_; //注意，这是一个指向Server对象的数组指针
   uint32_t position_;
   ps_common_sys::ThreadLock lock_;
   uint32_t count_;
   uint32_t maxcount_;

};

}; //namespace connection

}; //namespace ps_common_net

//extern ps_common_net::connection::Pool* g_connectionpool;

#endif //PS_COMMON_NET_CONNECTION_POOL_H_
