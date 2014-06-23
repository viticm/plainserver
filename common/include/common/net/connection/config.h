/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id config.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/23 13:47
 * @uses the net connection base config file
 */
#ifndef PS_COMMON_NET_CONNECTION_CONFIG_H_
#define PS_COMMON_NET_CONNECTION_CONFIG_H_

#include "common/net/config.h"

//默认每帧执行的消息数量上限
#define NET_CONNECTION_EXECUTE_COUNT_PRE_TICK_DEFAULT 12
#define NET_CONNECTION_MAX 1024
#define NET_CONNECTION_CACHESIZE_MAX 1024
#define NET_CONNECTION_KICKTIME 6000000 //超过该时间则断开连接
#define NET_CONNECTION_INCOME_KICKTIME 60000
#define NET_CONNECTION_POOL_SIZE_MAX 1280 //连接池上限

typedef enum {
  kPacketFlagNone = 0,
  kPacketFlagRemove,
} packetflag_enum;

typedef enum {
  kConnectionStatusGatewayEmpty = 400,
  kConnectionStatusGatewayConnect,
  kConnectionStatusCenterConnect,
  kConnectionStatusCenterNormal,
} connection_status_enum;

#endif //PS_COMMON_NET_CONNECTION_CONFIG_H_
