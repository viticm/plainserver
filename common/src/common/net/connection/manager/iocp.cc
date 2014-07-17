#include "common/net/connection/manager/iocp.h"

#if __WINDOWS__ && defined(_PS_NET_IOCP)
namespace ps_common_net {

namespace connection {

namespace manager {

Iocp::Iocp() {
  //do nothing
}

Iocp::~Iocp() {
  //do nothing
}

} //namespace manager

} //namespace connection

} //namespace ps_common_net
#endif
