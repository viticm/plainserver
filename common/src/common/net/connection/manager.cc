#include "common/net/connection/manager.h"

namespace ps_common_net {

namespace connection {

Manager::Manager() {
  //do nothing  
}

Manager::~Manager() {
  __ENTER_FUNCTION
    SAFE_DELETE_ARRAY(connection_idset_);
  __LEAVE_FUNCTION
}

void Manager::init(uint16_t maxcount) {
  __ENTER_FUNCTION
    count_ = 0;
    maxcount_ = maxcount;
    connection_idset_ = new int16_t[maxcount_];
    Assert(connection_idset_);
    memset(connection_idset_, 0, maxcount_);
  __ENTER_FUNCTION
}

bool Manager::heartbeat(uint32_t time) {
  __ENTER_FUNCTION
    USE_PARAM(time);
    bool result = true;
    return result;
  __LEAVE_FUNCTION
    return false;
}

bool Manager::add(Base* connection) {
  __ENTER_FUNCTION
    Assert(connection);
    if (ID_INVALID == connection_idset_[count_]) {
      connection_idset_[count_] = connection->getid();
      connection->set_managerid(count_);
      ++count_;
      Assert(count_ < maxcount_);
    }
    else {
      Assert(false);
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool Manager::add(int16_t id) {
  __ENTER_FUNCTION
    USE_PARAM(id);
    //not used
    Assert(false);
  __LEAVE_FUNCTION
    return false;
}

void Manager::remove(int16_t id) {
  __ENTER_FUNCTION
    //remember this function will completed in child
    Assert(count_ > 0);
    USE_PARAM(id);
  __LEAVE_FUNCTION
}

int16_t* Manager::get_idset() {
  return connection_idset_;
}

uint16_t Manager::getcount() {
  return count_;
}

bool Manager::hash() {
  bool result = connection_idset_[0] != ID_INVALID;
  return result;
}

} //namespace connection

} //namespace ps_common_net
