#include "common/base/util.h"
#include "common/base/log.h"
#include "common/db/odbc/system.h"

namespace ps_common_db {

namespace odbc {

System::System() {
  __ENTER_FUNCTION
    result_ = 0;
    result_count_ = 0;
    op_type_ = kDBOptionTypeInitEmpty;
    odbc_interface_ = NULL;
  __LEAVE_FUNCTION
}

System::~System() {
  __ENTER_FUNCTION
    SAFE_DELETE(odbc_interface_);
  __LEAVE_FUNCTION
}

bool System::init(const char *connectionname,
                  const char *username,
                  const char *password) {
  __ENTER_FUNCTION
    bool connected = true;
    odbc_interface_ = new Interface();
    Assert(odbc_interface_);
    connected = odbc_interface_->connect(connectionname, username, password);
    if (!connected) {
      SLOW_ERRORLOG("database",
                    "[db][odbc] (System::init) failed,"
                    " connectionname: %s, username: %s, password: %s"
                    " errormessage: %s",
                    connectionname,
                    username,
                    password,
                    get_error_message());
    }
    return connected;
  __LEAVE_FUNCTION
    return false;
}

uint32_t System::get_result_count() {
  __ENTER_FUNCTION
    return result_count_;
  __LEAVE_FUNCTION
    return 0;
}

db_query_t* System::get_internal_query() {
  __ENTER_FUNCTION
    return &odbc_interface_->get_query();
  __LEAVE_FUNCTION
    return NULL;
}

long_db_query_t* System::get_long_internal_query() {
  __ENTER_FUNCTION
    return &odbc_interface_->get_long_query();
  __LEAVE_FUNCTION
    return NULL;
}

int32_t System::get_internal_affect_count() {
  __ENTER_FUNCTION
    return odbc_interface_->get_affect_row_count();
  __LEAVE_FUNCTION
    return -1;
}

bool System::is_prepare() {
  __ENTER_FUNCTION
    return odbc_interface_->is_prepare();
  __LEAVE_FUNCTION
    return false;
}

bool System::check_db_connect() {
  __ENTER_FUNCTION
    Assert(odbc_interface_);
    if (!odbc_interface_->is_connected()) {
      int i;
      for (i = 0; i < 5; ++i) {
        ps_common_base::util::sleep(5000);
        if (odbc_interface_->connect()) {
          return true;
        }
      }
      return false;
    }
  __LEAVE_FUNCTION
    return false;
}

bool System::load() {
  __ENTER_FUNCTION
    if (!is_prepare()) return false;
    if (!odbc_interface_) return false;
    op_type_ = kDBOptionTypeLoad;
    result_ = odbc_interface_->execute();
    result_count_ = odbc_interface_->get_affect_row_count();
    return result_;
  __LEAVE_FUNCTION
    return false;
}

bool System::query() {
  __ENTER_FUNCTION
    if (!is_prepare()) return false;
    if (!odbc_interface_) return false;
    op_type_ = kDBOptionTypeQuery;
    result_ = odbc_interface_->execute();
    result_count_ = odbc_interface_->get_affect_row_count();
    return result_;
  __LEAVE_FUNCTION
    return false;
}

bool System::long_query() {
  __ENTER_FUNCTION
    if (!is_prepare()) return false;
    if (!odbc_interface_) return false;
    op_type_ = kDBOptionTypeQuery;
    result_ = odbc_interface_->long_execute();
    result_count_ = odbc_interface_->get_affect_row_count();
    return result_;
  __LEAVE_FUNCTION
    return false;
}

bool System::long_load() {
  __ENTER_FUNCTION
    if (!is_prepare()) return false;
    if (!odbc_interface_) return false;
    op_type_ = kDBOptionTypeLoad;
    result_ = odbc_interface_->long_execute();
    result_count_ = odbc_interface_->get_affect_row_count();
    return result_;
  __LEAVE_FUNCTION
    return false;
}

bool System::add_new() {
  __ENTER_FUNCTION
    if (!is_prepare()) return false;
    if (!odbc_interface_) return false;
    op_type_ = kDBOptionTypeAddNew;
    result_ = odbc_interface_->execute();
    result_count_ = odbc_interface_->get_affect_row_count();
    return result_;
  __LEAVE_FUNCTION
    return false;
}

bool System::_delete() {
  __ENTER_FUNCTION
    if (!is_prepare()) return false;
    if (!odbc_interface_) return false;
    op_type_ = kDBOptionTypeDelete;
    result_ = odbc_interface_->execute();
    result_count_ = odbc_interface_->get_affect_row_count();
    return result_;
  __LEAVE_FUNCTION
    return false;
}

bool System::save() {
  __ENTER_FUNCTION
    if (!is_prepare()) return false;
    if (!odbc_interface_) return false;
    op_type_ = kDBOptionTypeSave;
    result_ = odbc_interface_->execute();
    result_count_ = odbc_interface_->get_affect_row_count();
    return result_;
  __LEAVE_FUNCTION
    return false;
}

bool System::long_save() {
  __ENTER_FUNCTION
    if (!is_prepare()) return false;
    if (!odbc_interface_) return false;
    op_type_ = kDBOptionTypeSave;
    result_ = odbc_interface_->long_execute();
    result_count_ = odbc_interface_->get_affect_row_count();
    return result_;
  __LEAVE_FUNCTION
    return false;
}

int32_t System::get_error_code() {
  __ENTER_FUNCTION
    return odbc_interface_->get_error_code();
  __LEAVE_FUNCTION
    return -1;
}

const char *System::get_error_message() {
  __ENTER_FUNCTION
    return odbc_interface_->get_error_message();
  __LEAVE_FUNCTION
    return NULL;
}

} //namespace odbc

} //namespace ps_common_db
