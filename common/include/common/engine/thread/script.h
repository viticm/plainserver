/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id script.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/07/03 16:28
 * @uses the engine script thread class
 */
#ifndef PS_COMMON_ENGINE_THREAD_SCRIPT_H_
#define PS_COMMON_ENGINE_THREAD_SCRIPT_H_

#include "common/engine/thread/config.h"
#include "common/sys/thread.h"

namespace ps_common_engine {

namespace thread {

class Script : public ps_common_sys::Thread {

 public:
   Script();
   ~Script();

 public:
   bool init(const char *rootpath = NULL,
             const char *workpath = NULL,
             const char *globalfile = NULL);
   virtual void run();
   virtual void stop();
   void quit();
   bool isactive();

 private:
   bool isactive_;

}

}; //namespace thread

}; //namespace ps_common_engine

#endif //PS_COMMON_ENGINE_THREAD_SCRIPT_H_
