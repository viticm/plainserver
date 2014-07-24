/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id log.h
 * @link https://github.com/viticm/plainserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/07/22 21:10
 * @uses the application extend for base module log form framework, important!
 *       cn:
 *       框架基础日志扩展
 *       重要的 -- 编译时必须包含
 */
#ifndef PS_COMMON_APPLICATION_EXTEND_LOG_H_
#define PS_COMMON_APPLICATION_EXTEND_LOG_H_

#include "common/application/extend/config.h"
#include "common/base/log.h"

enum {
  kLoginLogFile = kLogFileCount,
  kShareMemoryLogFile,
  kGatewayLogFile,
  kCenterLogFile,
  kServerLogFile,
  kFinalLogFileCount,
}; //扩展的日志记录ID，从基础的kLogFileCount开始扩展

namespace ps_common_base {
extern const char *g_extend_log_filename[];
}; //扩展命名空间

#endif //PS_COMMON_APPLICATION_EXTEND_LOG_H_
