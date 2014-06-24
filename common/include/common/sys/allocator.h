/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id allocator.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/24 19:41
 * @uses the system memory allocator
 */
#ifndef PS_COMMON_SYS_ALLOCATOR_H_
#define PS_COMMON_SYS_ALLOCATOR_H_

#include "common/sys/config.h"

namespace ps_common_sys {

class Allocator {

 public:
   Allocator();
   ~Allocator();

 public:
   void init(char* buffer, size_t size);
   void clear();
   void* malloc(size_t size);
   void* calloc(size_t count, size_t size);
   void* realloc(void* data, size_t newsize);
   void free(void* data);

 private:
   char* buffer_;
   size_t size_;
   size_t offset_;

};

}; //namespace ps_common_sys

#endif //PS_COMMON_SYS_ALLOCATOR_H_
