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
#define __WINDOWS__ (defined(_MSC_VER) || defined(__ICL))
#define __LINUX__ !(__WINDOWS__)
#include "common/base/type.h"
/* } base type */

#define PASSWORD_ENCRYPT_KEY "123456" //密码加密串，真实加密串是取其MD5值

#endif //PS_COMMON_BASE_CONFIG_H_
