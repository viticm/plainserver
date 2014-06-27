/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id base.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/27 14:55
 * @uses script cache base class
 */
#ifndef PS_COMMON_SCRIPT_CACHE_BASE_H_
#define PS_COMMON_SCRIPT_CACHE_BASE_H_

#include "common/script/cache/config.h"

namespace ps_common_script {

namespace cache {

class Base {

 public:
   Base();
   ~Base();
   
 public:
   virtual bool init(int32_t id, const char *filename, FILE* fp);
   virtual void clear();

 public:
   int32_t getid() const;
   const char *get_filename() const;

 protected:
   int32_t id_;
   char filename_[FILENAME_MAX];

};

}; //namespace cache

}; //namespace ps_common_script

#endif //PS_COMMON_SCRIPT_CACHE_BASE_H_
