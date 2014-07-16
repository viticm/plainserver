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
#include "common/engine/thread/db.h"
#include "common/engine/thread/net.h"
#include "common/engine/thread/performance.h"
#include "common/engine/thread/script.h"

namespace ps_common_engine {

class Kernel {

 public:
   Kernel();
   ~Kernel();

 public: //kernel sys functions
   bool init();
   void run();
   void stop();
   void registerconfig(int32_t key, int32_t value);
   void registerconfig(int32_t key, bool value);
   void registerconfig(int32_t key, const char *value);
   bool setconfig(int32_t key, int32_t value);
   bool setconfig(int32_t key, bool value);
   bool setconfig(int32_t key, const char *value);
   int32_t getconfig_int32value(int32_t key);
   bool getconfig_boolvalue(int32_t key);
   const char *getconfig_stringvalue(int32_t key);

 public: //kernel for set_* functions
   void set_base_logprint(bool flag);
   void set_base_logactive(bool flag);

 protected:
   ps_common_base::hashmap::Template<int32_t, const char *> config_string_;
   ps_common_base::hashmap::Template<int32_t, int32_t> config_int32_;
   ps_common_base::hashmap::Template<int32_t, bool> config_bool_;
   ps_common_db::Manager *db_manager_;
   ps_common_net::Manager *net_manager_;
   thread::DB *db_thread_;
   thread::Net *net_thread_;
   thread::Performance *performance_thread_;
   thread::Script *script_thread_;

 protected:
   //子类重载此方法以实现初始化不同类型的网络连接数据，
   //引擎默认连接的socket未初始化，只在接受新连接时才初始化，
   //如需服务器启动就初始化可以重写此方法来实现（启动时就初始化会占用大量内存）。
   virtual bool init_net_connectionpool_data();

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
