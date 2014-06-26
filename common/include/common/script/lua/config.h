/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id config.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/25 19:04
 * @uses script lua module config file
 */
#ifndef PS_COMMON_SCRIPT_LUA_CONFIG_H_
#define PS_COMMON_SCRIPT_LUA_CONFIG_H_

#include "common/script/config.h"

#if __WINDOWS__
#define SCIPRT_LUA_ROOT_PATH_DEFAULT "..\\..\\public\\data\\script"
#define SCRIPT_LUA_WORK_PATH_DEFAULT "\\"
#elif __LINUX__
#define SCIPRT_LUA_ROOT_PATH_DEFAULT "../../public/data/script"
#define SCRIPT_LUA_WORK_PATH_DEFAULT "/"
#endif

#define SCRIPT_LUA_STACK_STEP_MAX 48
#define SCRIPT_LUA_STACK_FUNCTION_LENGTH_MAX 256
#define SCRIPT_LUA_SCENE_MAX 4096

#endif //PS_COMMON_SCRIPT_LUA_CONFIG_H_
