#include "common/db/manager.h"

namespace ps_common_db {

Manager::Manager(dbconnector_type_t connector_type) {
  __ENTER_FUNCTION
    connector_type_ = connector_type;
    odbc_system_ = NULL;
  __LEAVE_FUNCTION
}

Manager::~Manager() {
  __ENTER_FUNCTION
    SAFE_DELETE(odbc_system_);
  __LEAVE_FUNCTION
}
bool Manager::init(const char *connection_or_dbname,
                   const char *username,
                   const char *password) {
  __ENTER_FUNCTION
    bool result = true;
    switch (connector_type_) {
      case kDBConnectorTypeODBC: {
        odbc_system_ = new odbc::System();
        Assert(odbc_system_);
        result = 
          odbc_system_->init(connection_or_dbname, username, password);
        break;
      }
      default:
        result = false;
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return false;
}

dbconnector_type_t Manager::get_connector_type() const {
  return connector_type_;
}

bool Manager::query() {
  __ENTER_FUNCTION
    bool result = true;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = odbc_system_->query();
        break;
      default:
        result = false;
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return false;
}

bool Manager::long_query() {
  __ENTER_FUNCTION
    bool result = true;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = odbc_system_->long_query();
        break;
      default:
        result = false;
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return false;
}

db_query_t *Manager::get_internal_query() {
  __ENTER_FUNCTION
    db_query_t *query_pointer = NULL;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        query_pointer = odbc_system_->get_internal_query();
        break;
      default:
        query_pointer = NULL;
        break;
    }
    return query_pointer;
  __LEAVE_FUNCTION
    return NULL;
}

long_db_query_t *Manager::get_long_internal_query() {
  __ENTER_FUNCTION
    long_db_query_t *query_pointer = NULL;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        query_pointer = odbc_system_->get_long_internal_query();
        break;
      default:
        query_pointer = NULL;
        break;
    }
    return query_pointer;
  __LEAVE_FUNCTION
    return NULL;
}

int32_t Manager::get_affectcount() const {
  __ENTER_FUNCTION
    int32_t result = 0;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = odbc_system_->get_result_count();
        break;
      default:
        result = -1;
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return -1;
}

bool Manager::check_db_connect() {
  __ENTER_FUNCTION
    bool result = true;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = odbc_system_->check_db_connect();
        break;
      default:
        result = false;
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return false;
}

} //namespace ps_common_db
