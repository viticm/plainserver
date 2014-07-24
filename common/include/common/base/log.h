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
#include "common/base/time_manager.h"

typedef enum {
  kDebugLogFile = 0,
  kErrorLogFile = 1,
  kNetLogFile = 2,
  kFunctionLogFile = 3,
  kLogFileCount,
} logid_t;

namespace ps_common_base {

extern const char* kBaseLogSaveDir; //如果不要外部使用，就别使用宏

extern bool g_command_logprint; //global if print log to io
extern bool g_command_logactive; //global if write log to file
extern bool g_log_in_one_file;
extern ps_common_sys::ThreadLock g_log_lock;

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
   void flush_log(logid_t logid);
   int32_t get_logsize(logid_t logid);
   void get_log_filename(logid_t logid, char* filename);
   static void get_log_filename(const char* filename_prefix, char* filename);
   void flush_alllog();
   static void get_serial(char* serial, int16_t worldid, int16_t serverid);
   static void remove_log(const char* filename);
   static void get_log_timestr(char* time_str, int32_t length);

 public:
   //模板函数 type 0 普通日志 1 警告日志 2 错误日志 3 调试日志 9 只写日志
   template <uint8_t type>
   void fast_savelog(logid_t logid, const char* format, ...) {
     __ENTER_FUNCTION
       if (logid < 0 || logid >= kLogFileCount) return;
       char buffer[4096] = {0};
       char temp[4096] = {0};
       va_list argptr;
       try {
         va_start(argptr, format);
         vsnprintf(temp, sizeof(temp) - 1, format, argptr);
         va_end(argptr);
         if (g_time_manager) {
           char time_str[256] = {0};
           memset(time_str, '\0', sizeof(time_str));
           get_log_timestr(time_str, sizeof(time_str) - 1);
           snprintf(buffer, sizeof(buffer) - 1,"%s %s", time_str, temp);
         }
       }
       catch(...) {
         Assert(false);
         return;
       }

       if (g_command_logprint) {
         switch (type) {
          case 1:
            WARNINGPRINTF(buffer);
            break;
          case 2:
            ERRORPRINTF(buffer);
            break;
          case 3:
            DEBUGPRINTF(buffer);
            break;
          case 9:
            break;
          default:
            printf("%s"LF"", buffer);
            break;
         }
       }
       strncat(buffer, LF, sizeof(LF)); //add wrap
       if (!g_command_logactive) return; //save log condition
       int32_t length = static_cast<int32_t>(strlen(buffer));
       if (length <= 0) return;
       if (g_log_in_one_file) {
         //do nothing(one log file is not active in pap)
       }
       log_lock_[logid].lock();
       try {
         memcpy(log_cache_[logid] + log_position_[logid], buffer, length);
       }
       catch(...) {
         //do nogthing
       }
       log_position_[logid] += length;
       log_lock_[logid].unlock();
       if (log_position_[logid] > 
         static_cast<int32_t>((kDefaultLogCacheSize * 2) / 3)) {
           flush_log(logid);
       }
     __LEAVE_FUNCTION
   }

   //模板函数 type 0 普通日志 1 警告日志 2 错误日志 3 调试日志 9 只写日志
   template <uint8_t type>
   static void slow_savelog(const char* filename_prefix, 
                            const char* format, ...) {
       __ENTER_FUNCTION
       g_log_lock.lock();
       char buffer[4096] = {0};
       char temp[4096] = {0};
       va_list argptr;
       try {
         va_start(argptr, format);
         vsnprintf(temp, sizeof(temp) - 1, format, argptr);
         va_end(argptr);
         if (g_time_manager) {
           char time_str[256];
           memset(time_str, '\0', sizeof(time_str));
           get_log_timestr(time_str, sizeof(time_str) - 1);
           snprintf(buffer, sizeof(buffer) - 1,"%s %s", time_str, temp);
         }

         if (g_command_logprint) {
           switch (type) {
          case 1:
            WARNINGPRINTF(buffer);
            break;
          case 2:
            ERRORPRINTF(buffer);
            break;
          case 3:
            DEBUGPRINTF(buffer);
            break;
          case 9:
            break;
          default:
            printf("%s"LF"", buffer);
           }
         }
         strncat(buffer, LF, sizeof(LF)); //add wrap
         if (!g_command_logactive) return;
         char log_file_name[FILENAME_MAX];
         memset(log_file_name, '\0', sizeof(log_file_name));
         get_log_filename(filename_prefix, log_file_name);
         FILE* fp;
         fp = fopen(log_file_name, "ab");
         if (fp) {
           fwrite(buffer, 1, strlen(buffer), fp);
           fclose(fp);
         }
       }
       catch(...) {
         ERRORPRINTF("ps_common_base::Log::save_log have some log error here%s", 
           LF);
       }
       g_log_lock.unlock();
       __LEAVE_FUNCTION
   }

 private:
   char* log_cache_[kLogFileCount];
   int32_t log_position_[kLogFileCount];
   ps_common_sys::ThreadLock log_lock_[kLogFileCount];
   int32_t cache_size_;
   uint32_t day_time_;

};

}; //namespace ps_common_base


//log sytem macros
#define LOGSYSTEM_POINTER ps_common_base::Log::getsingleton_pointer()
#define FAST_LOG LOGSYSTEM_POINTER->fast_savelog<0>
#define FAST_WARNINGLOG LOGSYSTEM_POINTER->fast_savelog<1>
#define FAST_ERRORLOG LOGSYSTEM_POINTER->fast_savelog<2>
#define FAST_DEBUGLOG LOGSYSTEM_POINTER->fast_savelog<3>
#define FAST_WRITELOG LOGSYSTEM_POINTER->fast_savelog<9>
#define SLOW_LOG ps_common_base::Log::slow_savelog<0>
#define SLOW_WARNINGLOG ps_common_base::Log::slow_savelog<1>
#define SLOW_ERRORLOG ps_common_base::Log::slow_savelog<2>
#define SLOW_DEBUGLOG ps_common_base::Log::slow_savelog<3> 
#define SLOW_WRITELOG ps_common_base::Log::slow_savelog<9>

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

extern ps_common_base::Log* g_log;

#endif //PS_COMMON_BASE_LOG_H_
