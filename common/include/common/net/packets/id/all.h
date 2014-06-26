/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id all.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/23 11:17
 * @uses 游戏服务器网络包ID的定义，彻底改变天龙与武侠的设计模式，不过这里需要
 *       包含游戏基本定义中的宏，单独包含进来即可，不用担心ID重复
 *       （看似有局限，其实可以任意扩展）
 */
#ifndef PS_COMMON_NET_PACKETS_ID_ALL_H_
#define PS_COMMON_NET_PACKETS_ID_ALL_H_

namespace ps_common_net {

namespace packets {

namespace id {

#include "common/net/packets/id/clientlogin.h"
#include "common/net/packets/id/clientserver.h"
#include "common/net/packets/id/gatewaylogin.h"
#include "common/net/packets/id/logincenter.h"
#include "common/net/packets/id/servercenter.h"
#include "common/net/packets/id/serverserver.h"

}; //namespace id

}; //namespace packet

}; //namespace ps_common_net

#endif //PS_COMMON_NET_PACKETS_ID_ALL_H_
