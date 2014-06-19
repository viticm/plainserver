/**
 * PAP Engine ( https://github.com/viticm/pap )
 * $Id ptype.h
 * @link https://github.com/viticm/pap for the canonical source repository
 * @copyright Copyright (c) 2013-2013 viticm( viticm@126.com )
 * @license
 * @user viticm<viticm@126.com>
 * @date 2013-11-22 19:29:37
 * @uses the db manager class, just for server use dbs.
 */
#ifndef PAP_SERVER_COMMON_DB_MANAGER_H_
#define PAP_SERVER_COMMON_DB_MANAGER_H_

#include "common/base/singleton.h"
#include "common/db/odbc/config.h"
#include "common/db/odbc/interface.h"

namespace ps_common_db {

namespace odbc {

class Manager : public ps_common_base::Singleton<Manager> {
 public:
   Manager();
   ~Manager();

 public:
   Manager& getsingleton();
   Manager* getsingleton_pointer();
   bool init(db_type_enum db_type = kAllDatabase);
   ODBCInterface* get_interface(db_type_enum db_type);

 private:
   db_type_enum db_type_;
   ODBCInterface* character_interface_;
   ODBCInterface* user_interface_;
};

}; //namespace odbc

}; //namespace ps_common_db

#endif //PAP_SERVER_COMMON_DB_DB_MANAGER_H_
