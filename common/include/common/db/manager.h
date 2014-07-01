/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id manager.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/30 19:51
 * @uses the db manager class
 */
#include "common/db/config.h"
#include "common/db/odbc/system.h"

namespace ps_common_db {

class Manager {

 public:
   Manager(dbconnector_type_t connector_type = kDBConnectorTypeODBC);
   ~Manager();

 public:
   bool init(const char *connection_or_dbname,
             const char *username,
             const char *password);
   dbconnector_type_t get_connector_type() const;
   db_query_t *get_internal_query();
   long_db_query_t *get_long_internal_query();
   bool query();
   bool long_query();
   int32_t get_affectcount() const;
   bool check_db_connect();

 protected:
   dbconnector_type_t connector_type_;
   odbc::System *odbc_system_;

};

}; //namespace ps_common_db
