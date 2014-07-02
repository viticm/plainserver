/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id config.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/25 19:06
 * @uses script module base config file
 */
#ifndef PS_COMMON_SCRIPT_CONFIG_H_
#define PS_COMMON_SCRIPT_CONFIG_H_

#include "common/base/config.h"

#if __WINDOWS__
#define SCRIPT_ROOT_PATH_DEFAULT "public\\data\\script"
#define SCRIPT_WORK_PATH_DEFAULT "\\"
#elif __LINUX__
#define SCRIPT_ROOT_PATH_DEFAULT "public/data/script"
#define SCRIPT_WORK_PATH_DEFAULT "/"
#endif

#endif //PS_COMMON_SCRIPT_CONFIG_H_
