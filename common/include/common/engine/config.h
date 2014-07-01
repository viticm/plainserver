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

#define ENGINE_CONFIG_DB_ISACTIVE "db_isactive"
#define ENGINE_CONFIG_NET_ISACTIVE "net_isactive"
#define ENGINE_CONFIG_SCRIPT_ISACTIVE "script_isactive"
#define ENGINE_CONFIG_PERFORMANCE_ISACTIVE "performance_isactive"
#define ENGINE_CONFIG_DB_CONNECTOR_TYPE "db_connector_type"
#define ENGINE_CONFIG_DB_CONNECTION_OR_DBNAME "db_connection_or_dbname"
#define ENGINE_CONFIG_DB_USERNAME "db_username"
#define ENGINE_CONFIG_DB_PASSWORD "db_password"
#define ENGINE_CONFIG_NET_LISTEN_PORT "net_listenport"
#define ENGINE_CONFIG_NET_CONNECTION_MAX "net_connectionmax"
#define ENGINE_CONFIG_SCRIPT_ROOTPATH "script_rootpath"
#define ENGINE_CONFIG_SCRIPT_WORKPATH "script_workpath"
#define ENGINE_CONFIG_SCRIPT_GLOBALFILE "script_globalfile"

#endif //PS_COMMON_ENGINE_CONFIG_H_
