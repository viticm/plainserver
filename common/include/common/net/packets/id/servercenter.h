/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id servercenter.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/23 19:40
 * @uses net packets define of server between center
 */
#ifndef PS_COMMON_NET_PACKETS_ID_SERVERCENTER_H_
#define PS_COMMON_NET_PACKETS_ID_SERVERCENTER_H_

#include "common/net/packets/id/config.h"

NET_PACKETS_ID_NAMESPACE_START

namespace servercenter {

enum packetid_enum {
  kFirst = SERVERCENTER_PACKETID_MIN,
  kLast,
  kMax = SERVERCENTER_PACKETID_MAX
};

}; //namespace servercenter

namespace server_tocenter {

enum packetid_enum {
  kFirst = CENTER_TOSERVER_PACKETID_MIN,
  kLast,
  kMax = CENTER_TOSERVER_PACKETID_MAX
};

}; //server_tocenter

namespace center_toserver {

enum packetid_enum {
  kFirst = SERVER_TOCENTER_PACKETID_MIN,
  kLast,
  kMax = SERVER_TOCENTER_PACKETID_MAX
};

}; //center_toserver

NET_PACKETS_ID_NAMESPACE_END

#endif //PS_COMMON_NET_PACKETS_ID_SERVERCENTER_H_
