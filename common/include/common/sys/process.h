/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id process.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/07/07 18:04
 * @uses the system process module
 */
#ifndef PS_COMMON_SYS_PROCESS_H_
#define PS_COMMON_SYS_PROCESS_H_

#include "common/sys/config.h"

namespace ps_common_sys {

namespace process {

typedef struct {
  int32_t id;
  float cpu_percent;
  uint64_t VSZ;
  uint64_t RSS;
} info_t;

int32_t getid();
int32_t getid(const char *filename);
bool writeid(const char *filename);
bool waitexit(const char *filename);
void getinfo(int32_t id, info_t &info);
float get_cpu_usage(int32_t id);
uint64_t get_virtualmemory_usage(int32_t id);
uint64_t get_physicalmemory_usage(int32_t id);
bool daemon();

}; //namespace process

}; //namespace ps_common_sys

#define SYS_PROCESS_CURRENT_INFO_PRINT() \
  DEBUGPRINTF("cpu: %.1f%% VSZ: %dk RSS: %dk",\
              ps_common_sys::process::get_cpu_usage(\
                ps_common_sys::process::getid()),\
              ps_common_sys::process::get_virtualmemory_usage(\
                ps_common_sys::process::getid()),\
              ps_common_sys::process::get_physicalmemory_usage(\
                ps_common_sys::process::getid()))

#endif //PS_COMMON_SYS_PROCESS_H_
