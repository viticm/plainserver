/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id manager.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2014/06/23 10:33
 * @uses db module odbc manager class
 */
#ifndef PS_COMMON_DB_ODBC_MANAGER_H_
#define PS_COMMON_DB_ODBC_MANAGER_H_

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
   Interface* get_interface(db_type_enum db_type);

 private:
   db_type_enum db_type_;
   Interface* character_interface_;
   Interface* user_interface_;
};

}; //namespace odbc

}; //namespace ps_common_db

#endif //PS_COMMON_DB_ODBC_MANAGER_H_
