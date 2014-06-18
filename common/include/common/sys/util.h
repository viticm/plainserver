#ifndef PS_COMMON_SYS_UTIL_H_
#define PS_COMMON_SYS_UTIL_H_

#include "common/sys/config.h"

namespace ps_common_sys {

namespace util {

void dumpstack(const char* log_fileprefix, const char* type);

}; //namespace util

}; //namespace ps_common_sys

#endif //PS_COMMON_SYS_UTIL_H_
