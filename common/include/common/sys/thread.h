/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id thread.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2014/06/18 17:39
 * @uses useful thread classes
 */
#ifndef PS_COMMON_THREAD_H_
#define PS_COMMON_THREAD_H_

#include "common/base/config.h"

namespace ps_common_sys {

class Thread {
 public:
   typedef enum { //线程的四种状态 (准备、运行中、退出中、已退出)
     kReady,
     kRunning,
     kExiting,
     kExit,
   }enum_thread_status;

 public:
   Thread();
   virtual ~Thread();
   void start();
   virtual void stop();
   void exit(void* retval = NULL);
   virtual void run();
#if __WINDOWS__
   DWORD get_id();
#elif __LINUX__
   uint64_t get_id();
#endif
   enum_thread_status get_status();
   void set_status(enum_thread_status status);

 private:
#if __LINUX__
   uint64_t id_;
#elif __WINDOWS__
   DWORD id_;
#endif
   enum_thread_status status_;
#if __WINDOWS__
   HANDLE thread_handle_;
#endif

};

#if __LINUX__
void* pap_thread_process(void* derived_thread);
#elif __WINDOWS__
DWORD WINAPI pap_thread_process(void* derived_thread);
#endif

class ThreadLock {
 public:
#if __LINUX__
   pthread_mutex_t mutex_;
#elif __WINDOWS__
   CRITICAL_SECTION lock_;
#endif
   ThreadLock();
   ~ThreadLock();
   void lock();
   void unlock();
};

uint64_t get_current_thread_id();

//global variable
extern uint16_t g_thread_quit_count;

}; //namespace ps_common_sys

//thread lock
extern ps_common_sys::ThreadLock g_thread_lock;

#endif //PS_COMMON_THREAD_H_
