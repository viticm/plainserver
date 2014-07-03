/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id net.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/07/03 16:05
 * @uses your description
 */
#ifndef PS_COMMON_ENGINE_THREAD_NET_H_
#define PS_COMMON_ENGINE_THREAD_NET_H_

#include "common/engine/thread/config.h"
#include "common/sys/thread.h"
#include "common/net/manager.h"

namespace ps_common_engine {

namespace thread {

class Net : public ps_common_net::Manager, public ps_common_sys::Thread {

 public:
   Net(uint16_t port = 0);
   ~Net();

 public:
   bool init(uint16_t connectionmax = NET_CONNECTION_MAX);
   virtual void run();
   virtual void stop();
   void quit();
   bool isactive();

 private:
   bool isactive_;

};

}; //namespace thread

}; //namespace ps_common_engine

#endif //PS_COMMON_ENGINE_THREAD_NET_H_
