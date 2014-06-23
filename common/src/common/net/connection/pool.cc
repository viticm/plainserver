#include "common/net/connection/pool.h"

ps_common_net::connection::Pool* g_connectionpool = NULL;

namespace ps_common_net {

namespace connection {

Pool::Pool() {
  connections_ = NULL;
  position_ = 0;
  count_ = 0;
}

Pool::~Pool() {
  SAFE_DELETE_ARRAY(connections_);
}

bool Pool::init() {
  __ENTER_FUNCTION
    connections_ = new Server[NET_CONNECTION_POOL_SIZE_MAX];
    Assert(connections_);
    uint16_t i;
    for(i = 0; i < NET_CONNECTION_POOL_SIZE_MAX; ++i) {
      connections_[i].setid(i);
      connections_[i].setempty(true);
    }
    position_ = 0;
    count_ = NET_CONNECTION_POOL_SIZE_MAX;
    return true;
  __LEAVE_FUNCTION
    return false;
}

Server* Pool::get(int16_t id) {
  __ENTER_FUNCTION
    Server* connection = NULL;
    if (id > NET_CONNECTION_POOL_SIZE_MAX) return NULL;
    connection = &(connections_[id]);
    return connection;
  __LEAVE_FUNCTION
    return NULL;
}

Server* Pool::create() {
  __ENTER_FUNCTION
    Server* connection = NULL;
    lock();
    uint16_t result = 0, i;
    for (i = 0; i < NET_CONNECTION_POOL_SIZE_MAX; ++i) {
      if (connections_[position_].isempty()) { //找出空闲位置
        result = static_cast<uint16_t>(position_);
        connections_[position_].setempty(false);
        ++position_;
        if (position_ >= NET_CONNECTION_POOL_SIZE_MAX) position_ = 0;
        --count_;
        connection = &(connections_[result]);
        break;
      }
      ++position_;
      if (position_ >= NET_CONNECTION_POOL_SIZE_MAX) position_ = 0;
    }
    unlock();
    return connection;
  __LEAVE_FUNCTION
    unlock();
    return NULL;
}

void Pool::remove(int16_t id) {
  __ENTER_FUNCTION
    lock();
    if (id > NET_CONNECTION_POOL_SIZE_MAX) {
      Assert(false);
      unlock();
      return;
    }
    connections_[id].setempty(true);
    ++count_;
    unlock();
  __LEAVE_FUNCTION
    unlock();
}

void Pool::lock() {
  lock_.lock();
}

void Pool::unlock() {
  lock_.unlock();
}

} //namespace connection

} //namespace ps_common_net
