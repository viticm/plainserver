/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id dynamic_allocator.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/25 20:47
 * @uses the system memory manger base class
 */
#ifndef PS_COMMON_SYS_MEMORY_DYNAMIC_ALLOCATOR_H_
#define PS_COMMON_SYS_MEMORY_DYNAMIC_ALLOCATOR_H_

#include "common/sys/memory/config.h"

namespace ps_common_sys {

namespace memory {

class DynamicAllocator {

 public:
   DynamicAllocator();
   ~DynamicAllocator();

 public:
   void* malloc(int64_t length);
   void free();
   void* getpointer();
   int64_t getlength();
 
 private:
   void* pointer_;
   int64_t length_;

}; 

}; //namespace memory

}; //namespace ps_common_sys

#endif //PS_COMMON_SYS_MEMORY_DYNAMIC_ALLOCATOR_H_
