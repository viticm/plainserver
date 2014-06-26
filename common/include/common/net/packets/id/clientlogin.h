/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id client_tologin.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/23 19:09
 * @uses client to login id defines
 */
#ifndef PS_COMMON_NET_PACKETS_ID_CLIENT_TOLOGIN_H_
#define PS_COMMON_NET_PACKETS_ID_CLIENT_TOLOGIN_H_

#include "common/net/packets/id/config.h" //must include this

namespace clientlogin {

enum packetid_enum {
  kFirst = CLIENTLOGIN_PACKETID_MIN,
  kConnect,
  kLast,
  kMax = CLIENTLOGIN_PACKETID_MAX,
};

}; //namespace clientlogin

namespace client_tologin {

enum packetid_enum {
  kFirst = CLIENT_TOLOGIN_PACKETID_MIN,
  kLast,
  kMax = CLIENT_TOLOGIN_PACKETID_MAX
};

}; //namespace client_tologin

namespace login_toclient {

enum packetid_enum {
  kFirst = LOGIN_TOCLIENT_PACKETID_MIN,
  kLast,
  kMax = LOGIN_TOCLIENT_PACKETID_MAX,
};

}; //namespace login_toclient

#endif //PS_COMMON_NET_PACKETS_ID_CLIENT_TOLOGIN_H_
