/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id config.h
 * @link https://github.com/viticm/plainserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm@126.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/18 12:05
 * @uses 游戏公用基础模块的基本定义文件
 */
#ifndef PS_COMMON_BASE_CONFIG_H_
#define PS_COMMON_BASE_CONFIG_H_

/* base type { */
#include "common/base/type.h"
/* } base type */

enum {
  kDebugLogFile = 0,
  kErrorLogFile = 1,
  kNetLogFile = 2,
  kFunctionLogFile = 3,
  kLogFileCount,
};

#define PASSWORD_ENCRYPT_KEY "123456" //密码加密串，真实加密串是取其MD5值
#define DATE_LENGTH_MAX 20

#include "common/application/extend/define.h"

//headers include order: common/base/config.h -> sys include -> module include

#endif //PS_COMMON_BASE_CONFIG_H_
