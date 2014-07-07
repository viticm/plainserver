/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id eyes.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/07/07 10:15
 * @uses the perfoemance eyes base class
 */
#ifndef PS_COMMON_PERFORMANCE_EYES_H_
#define PS_COMMON_PERFORMANCE_EYES_H_

namespace ps_common_performance {

class Eyes {

 public:
   Eyes();
   ~Eyes();

 public:
   void send_custominfo();
   void activate();

 public:
   virtual uint64_t get_sendbytes() = 0;
   virtual uint64_t get_reveivebytes() = 0;

};

}; //namespace ps_common_performance

#endif //PS_COMMON_PERFORMANCE_EYES_H_
