/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id serverserver.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/23 11:21
 * @uses 服务器与服务器之间的通讯包
 */
#ifndef PS_COMMON_NET_PACKETS_ID_SERVERSERVER_H_
#define PS_COMMON_NET_PACKETS_ID_SERVERSERVER_H_

#include "common/net/packets/id/config.h" //must include this

NET_PACKETS_ID_NAMESPACE_START

namespace serverserver { //服务器与服务器公用

typedef enum {
  kFirst = 0, //起始ID
  kConnect,
  kLast, /* the last packetid */
  kMax = 7,
 } packetid_enum;

}; //namespace serverserver

NET_PACKETS_ID_NAMESPACE_END

#endif //PS_COMMON_NET_PACKETS_ID_SERVERSERVER_H_
