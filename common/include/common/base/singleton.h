/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id singleton.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2014/06/18 16:05
 * @uses Template class for creating single-instance global classes.
 */
#ifndef PS_COMMON_BASE_SINGLETON_H_
#define PS_COMMON_BASE_SINGLETON_H_

#include "common/base/config.h"

namespace ps_common_base {

template <typename T>
class Singleton {
 
 public:
   Singleton() {
     Assert(!singleton_);
     singleton_ = static_cast<T*>(this);
   }

   ~Singleton() {
     Assert(singleton_);
     singleton_ = NULL;
   }

   static T& getsingleton() {
     Assert(singleton_);
     return *singleton_;
   }

   static T* getsingleton_pointer() {
     return singleton_;
   }

 protected:
   static T* singleton_;

 private:
   Singleton(const Singleton<T> &);
   Singleton& operator=(const Singleton<T> &);

};

}; //namespace ps_common_base

#endif //PS_COMMON_BASE_SINGLETON_H_
