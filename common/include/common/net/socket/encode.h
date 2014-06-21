/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id encode.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2014/06/20 19:11
 * @uses simple socket stream buffer encode and decode functions
 */
#ifndef PS_NET_SOCKET_ENCODE_H_
#define PS_NET_SOCKET_ENCODE_H_

#include "common/net/socket/config.h"

namespace ps_common_net {

namespace scoket {

namespace encode {

bool make(struct encodeparam_t& encodeparam);
bool skip(struct encodeparam_t& encodeparam, int32_t length);

}; //namespace encode

}; //namespace socket

}; //namespace ps_common_net

#endif //PS_NET_SOCKET_ENCODE_H_
