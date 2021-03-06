/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id integerex.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/27 12:09
 * @uses base hashmap module integer extend template class 
 */
#ifndef PS_COMMON_BASE_HASHMAP_INTEGEREX_H_
#define PS_COMMON_BASE_HASHMAP_INTEGEREX_H_

#include "common/base/hashmap/config.h"

namespace ps_common_base {

namespace hashmap {

class Integer {

 public:
   typedef hash_map<uint32_t, void *> hashmap_t;
   typedef hashmap_t::iterator iterator_t;

 public:
   Integer();
   ~Integer();

 public:
   void init(uint32_t count);
   bool add(uint32_t id, void *pointer);
   void *get(uint32_t);
   bool remove(uint32_t);
   uint32_t getcount() const;
   uint32_t get_maxcount() const;
   void clear();
   iterator_t begin();
   iterator_t end();

 private:
   hashmap_t hashmap_;
   iterator_t iterator_;
   uint32_t maxcount_;

};

}; //namespace hashmap

}; //namespace ps_common_base

#endif //PS_COMMON_BASE_HASHMAP_INTEGEREX_H_
