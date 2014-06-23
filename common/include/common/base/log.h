/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id log.h
 * @link https://github.com/viticm/plainserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/18 15:48
 * @uses server log class
 */
#ifndef PS_COMMON_BASE_LOG_H_
#define PS_COMMON_BASE_LOG_H_

#include "common/base/config.h"
#include "common/sys/thread.h"
#include "common/base/singleton.h"

typedef enum {
  kLoginLogFile = 0,
  kDebugLogFile = 1,
  kErrorLogFile = 2,
  kShareMemoryLogFile = 3,
  kGatewayLogFile = 4,
  kCenterLogFile = 5,
  kServerLogFile = 6,
  kNetLogFile = 7,
  kLogFileCount,
} enum_log_id;


namespace ps_common_base {

extern const char* kBaseLogSaveDir; //如果不要外部使用，就别使用宏

extern bool g_command_log_print; //global if print log to io
extern bool g_command_log_active; //global if write log to file

const uint32_t kLogBufferTemp = 4096;
const uint32_t kLogNameTemp = 128;
const uint32_t kDefaultLogCacheSize = 1024 * 1024 * 4;

class Log : public Singleton<Log> {

 public:
   Log();
   ~Log();
   static Log* getsingleton_pointer();
   static Log& getsingleton();

 public:
   static void disk_log(const char* file_name_prefix, const char* format, ...);
   bool init(int32_t cache_size = kDefaultLogCacheSize);
   void fast_save_log(enum_log_id log_id, const char* format, ...); //save in memory
   void flush_log(enum_log_id log_id);
   int32_t get_log_size(enum_log_id log_id);
   void get_log_file_name(enum_log_id log_id, char* file_name);
   static void get_log_file_name(const char* file_name_prefix, char* file_name);
   void flush_all_log();
   static void get_serial(char* serial, int16_t world_id, int16_t server_id);
   static void save_log(const char* file_name_prefix, const char* format, ...);
   static void remove_log(const char* file_name);
   static void get_log_time_str(char* time_str, int32_t length);

 private:
   char* log_cache_[kLogFileCount];
   int32_t log_position_[kLogFileCount];
   ps_common_sys::ThreadLock log_lock_[kLogFileCount];
   int32_t cache_size_;
   uint32_t day_time_;

};

#if __LINUX__
#define SaveErrorLog() (ps_common_base::Log::save_log( \
    "error", \
    "%s %d %s", \
    __FILE__, \
    __LINE__, \
    __PRETTY_FUNCTION__))
#elif __WINDOWS__
#define SaveErrorLog() (ps_common_base::Log::save_log( \
    "error", \
    "%s %d %s", \
    __FILE__, \
    __LINE__, \
    __FUNCTION__))
#endif

}; //namespace ps_common_base

extern ps_common_base::Log* g_log;

#endif //PS_COMMON_BASE_LOG_H_
