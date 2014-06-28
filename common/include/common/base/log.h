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
  kFunctionLogFile = 8,
  kLogFileCount,
} logid_t;


namespace ps_common_base {

extern const char* kBaseLogSaveDir; //如果不要外部使用，就别使用宏

extern bool g_command_logprint; //global if print log to io
extern bool g_command_logactive; //global if write log to file

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
   //模板函数 type 0 普通日志 1 警告日志 2 错误日志
   template <uint8_t type>
   void fast_savelog(logid_t logid, const char* format, ...); 
   void flush_log(logid_t logid);
   int32_t get_logsize(logid_t logid);
   void get_log_filename(logid_t logid, char* filename);
   static void get_log_filename(const char* filename_prefix, char* filename);
   void flush_alllog();
   static void get_serial(char* serial, int16_t worldid, int16_t serverid);
   static void remove_log(const char* filename);
   static void get_log_timestr(char* time_str, int32_t length);
   //模板函数 type 0 普通日志 1 警告日志 2 错误日志
   template <uint8_t type>
   static void slow_savelog(const char* file_nameprefix, 
                            const char* format, 
                            ...);

 private:
   char* log_cache_[kLogFileCount];
   int32_t log_position_[kLogFileCount];
   ps_common_sys::ThreadLock log_lock_[kLogFileCount];
   int32_t cache_size_;
   uint32_t day_time_;

};

#if __LINUX__
#define SaveErrorLog() (SLOW_ERRORLOG( \
    "error", \
    "%s %d %s", \
    __FILE__, \
    __LINE__, \
    __PRETTY_FUNCTION__))
#elif __WINDOWS__
#define SaveErrorLog() (SLOW_ERRORLOG( \
    "error", \
    "%s %d %s", \
    __FILE__, \
    __LINE__, \
    __FUNCTION__))
#endif

}; //namespace ps_common_base

//log sytem macros
#define LOGSYSTEM_POINTER ps_common_base::Log::getsingleton_pointer()
#define FAST_LOG LOGSYSTEM_POINTER->fast_savelog<0>
#define FAST_WARNINGLOG LOGSYSTEM_POINTER->fast_savelog<1>
#define FAST_ERRORLOG LOGSYSTEM_POINTER->fast_savelog<2>
#define SLOW_LOG ps_common_base::Log::slow_savelog<0>
#define SLOW_WARNINGLOG ps_common_base::Log::slow_savelog<1>
#define SLOW_ERRORLOG ps_common_base::Log::slow_savelog<2>

extern ps_common_base::Log* g_log;

#endif //PS_COMMON_BASE_LOG_H_
