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
#include "common/base/hashmap/template.h"
#include "common/db/manager.h"
#include "common/net/manager.h"

namespace ps_common_engine {

class Kernel {

 public:
   Kernel();
   ~Kernel();

 public: //kernel sys functions
   bool init();
   void run();
   void stop();
   void registerconfig(const char *name, int32_t value);
   void registerconfig(const char *name, bool value);
   void registerconfig(const char *name, const char *value);
   bool setconfig(const char *name, int32_t value);
   bool setconfig(const char *name, bool value);
   bool setconfig(const char *name, const char *value);
   int32_t getconfig_int32value(const char *name);
   bool getconfig_boolvalue(const char *name);
   const char *getconfig_stringvalue(const char *name);

 public: //kernel for set_* functions
   void set_base_logprint(bool flag);
   void set_base_logactive(bool flag);

 protected:
   ps_common_base::hashmap::Template<const char *, const char *> config_string_;
   ps_common_base::hashmap::Template<const char *, int32_t> config_int32_;
   ps_common_base::hashmap::Template<const char *, bool> config_bool_;
   ps_common_db::Manager *db_manager_;
   ps_common_net::Manager *net_manager_;

 protected:
   bool init_base();
   bool init_db();
   bool init_net();
   bool init_script();
   bool init_performance();
   void run_base();
   void run_db();
   void run_net();
   void run_script();
   void run_performance();
   void stop_base();
   void stop_db();
   void stop_net();
   void stop_script();
   void stop_performance();

};

}; //namespace ps_common_engine

#endif //PS_COMMON_ENGINE_KERNEL_H_
