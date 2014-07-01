/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id config.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/30 15:41
 * @uses the engine module config file
 */
#ifndef PS_COMMON_ENGINE_CONFIG_H_
#define PS_COMMON_ENGINE_CONFIG_H_

#include "common/base/config.h"

#define ENGINE_CONFIG_DB_ISACTIVE 1
#define ENGINE_CONFIG_NET_ISACTIVE 2
#define ENGINE_CONFIG_SCRIPT_ISACTIVE 3
#define ENGINE_CONFIG_PERFORMANCE_ISACTIVE 4
#define ENGINE_CONFIG_DB_CONNECTOR_TYPE 5
#define ENGINE_CONFIG_DB_CONNECTION_OR_DBNAME 6
#define ENGINE_CONFIG_DB_USERNAME 7
#define ENGINE_CONFIG_DB_PASSWORD 8
#define ENGINE_CONFIG_NET_LISTEN_PORT 9
#define ENGINE_CONFIG_NET_CONNECTION_MAX 10
#define ENGINE_CONFIG_SCRIPT_ROOTPATH 11
#define ENGINE_CONFIG_SCRIPT_WORKPATH 12
#define ENGINE_CONFIG_SCRIPT_GLOBALFILE 13

#endif //PS_COMMON_ENGINE_CONFIG_H_
