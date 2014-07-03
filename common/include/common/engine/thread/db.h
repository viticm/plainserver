/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id db.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/07/03 14:26
 * @uses the engine db thread module
 */
#ifndef PS_COMMON_ENGINE_THREAD_DB_H_
#define PS_COMMON_ENGINE_THREAD_DB_H_

#include "common/engine/thread/config.h"
#include "common/sys/thread.h"
#include "common/db/manager.h"

namespace ps_common_engine {

namespace thread {

class DB : public ps_common_db::Manager, public ps_common_sys::Thread  {

 public:
   DB(dbconnector_type_t connector_type = kDBConnectorTypeODBC);
   ~DB();
 
 public:
   bool init(const char *connection_or_dbname,
             const char *username,
             const char *password);
   virtual void run();
   virtual void stop();
   void quit();
   bool isactive();

 private:
   bool isactive_;

};

}; //namespace thread

}; //namespace ps_common_engine

#endif //PS_COMMON_ENGINE_THREAD_DB_H_
