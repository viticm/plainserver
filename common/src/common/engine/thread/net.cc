#include "common/engine/thread/net.h"

using namespace ps_common_engine::thread;

Net::Net(uint16_t port) : ps_common_net::Manager(port) {
  //do nothing    
}

Net::~Net() {
  //do nothing
}

bool Net::init(uint16_t connectionmax) {
  __ENTER_FUNCTION
    using namespace ps_common_net;
    isactive_ = Manager::init(connectionmax);
    return isactive_;
  __LEAVE_FUNCTION
    return false;
}

void Net::run() {
  __ENTER_FUNCTION
    using namespace ps_common_net;
    while (isactive()) Manager::loop();
  __LEAVE_FUNCTION
}

void Net::stop() {
  isactive_ = false;
}

void Net::quit() {
  //do nothing
}

bool Net::isactive() {
  return isactive_;
}
