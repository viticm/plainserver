/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id kernel.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/30 15:43
 * @uses the plian server engine kernel class
 */
#ifndef PS_COMMON_ENGINE_KERNEL_H_
#define PS_COMMON_ENGINE_KERNEL_H_

#include "common/engine/config.h"

namespace ps_common_engine {

class Kernel {

 public:
   Kernel();
   ~Kernel();

 public: //kernel sys functions
   bool init();
   bool init_base();
   bool init_db();
   bool init_net();
   bool init_script();
   bool init_performance();
   void run();
   void run_base();
   void run_db();
   void run_net();
   void run_script();
   void run_performance();
   void stop();
   void stop_base();
   void stop_db();
   void stop_net();
   void stop_performance();

 public:


 public: //kernel for set_* functions
   void set_base_logprint(bool flag);
   void set_base_logactive(bool flag);

 protected:
   bool db_isactive_;
   bool net_isactive_;
   bool script_isactive_;
   bool performance_isactive_;

};

}; //namespace ps_common_engine

#endif //PS_COMMON_ENGINE_KERNEL_H_
