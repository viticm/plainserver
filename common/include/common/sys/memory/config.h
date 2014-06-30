/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id config.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/25 20:41
 * @uses system memory base config file
 */
#ifndef PS_COMMON_SYS_MEMORY_CONFIG_H_
#define PS_COMMON_SYS_MEMORY_CONFIG_H_

#include "common/sys/config.h"

typedef enum {
  kCmdModelClearAll = 1,
    kCmdModelLoadDump = 2,
} cmd_model_t; //命令行模式
extern int32_t g_cmd_model;

#endif //PS_COMMON_SYS_MEMORY_CONFIG_H_
