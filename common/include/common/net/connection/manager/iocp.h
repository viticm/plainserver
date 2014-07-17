/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id iocp.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/07/17 17:38
 * @uses the connection iocp manager
 */
#ifndef PS_COMMON_NET_CONNECTION_MANAGER_IOCP_H_
#define PS_COMMON_NET_CONNECTION_MANAGER_IOCP_H_

#include "common/net/connection/manager/base.h"

#if __WINDOWS__ && defined(_NET_IOCP)
#error iocp connection manager not completed!

namespace ps_common_net {

namespace connection {

namespace manager {

class Iocp : public Base {

 public:
   Iocp();
   ~Iocp();

};

}; //namespace manager

}; //namespace connection

}; //namespace ps_common_net
#endif

#endif //PS_COMMON_NET_CONNECTION_MANAGER_IOCP_H_
