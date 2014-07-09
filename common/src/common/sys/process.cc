#include "common/sys/util.h"
#include "common/sys/process.h"
#if __WINDOWS__
#include <process.h>
#include <psapi.h>
#elif __LINUX__
#include <unistd.h>
#include <sys/stat.h>
#endif

namespace ps_common_sys {

namespace process {

int32_t getid() {
  __ENTER_FUNCTION
    int32_t id = ID_INVALID;
#if __WINDOWS__
    id = _getpid();
#elif __LINUX__
    id = getpid();
#endif
    return id;
  __LEAVE_FUNCTION
    return ID_INVALID;
}


void getinfo(int32_t id, info_t &info) {
  __ENTER_FUNCTION
    info.id = id;
    info.cpu_percent = get_cpu_usage(id);
    info.VSZ = get_virtualmemory_usage(id);
    info.RSS = get_physicalmemory_usage(id);
  __LEAVE_FUNCTION
}

#if __WINDOWS__
static uint64_t file_time_2_utc(const FILETIME* ftime) {
  LARGE_INTEGER li;
  Assert(ftime);
  li.LowPart = ftime->dwLowDateTime;
  li.HighPart = ftime->dwHighDateTime;
  return li.QuadPart;
}

static int32_t get_processor_number() {
  SYSTEM_INFO info;
  GetSystemInfo(&info);
  int32_t result = static_cast<int32_t>(info.dwNumberOfProcessors);
  return result;
}
#endif 

float get_cpu_usage(int32_t id) {
  __ENTER_FUNCTION
  float cpu = -1.0f;
#if __WINDOWS__ /* { */
  static int processor_count = -1;
  static int64_t last_time = 0;
  static int64_t last_system_time = 0;
  FILETIME now;
  FILETIME creation_time;
  FILETIME exit_time;
  FILETIME kernel_time;
  FILETIME user_time;
  int64_t system_time;
  int64_t time;
  int64_t system_time_delta;
  int64_t time_delta;
  if (-1 == processor_count) {
    processor_count = get_processor_number();
  }
  GetSystemTimeAsFileTime(&now);
  HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
  if (!::GetProcessTimes(hProcess, 
                         &creation_time, 
                         &exit_time,
                         &kernel_time, 
                         &user_time)) {
    return -1.0f;
  }
  system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time)) / 
    processor_count;
  time = file_time_2_utc(&now);
  if ((0 == last_system_time) || (0 == last_time)) {
    last_system_time = system_time;
    last_time = time;
    return -1.0f;
  }
  system_time_delta = system_time - last_system_time;
  time_delta = time - last_time;
  if (time_delta == 0) return -1.0f;
  cpu = static_cast<float>(
    (system_time_delta * 100 + time_delta / 2) / time_delta);
  last_system_time = system_time;
  last_time = time;
#elif __LINUX__ /* } { */
  char temp[32] = {0};
  char command[128] = {0};
  snprintf(command, 
           sizeof(command) - 1, 
           "ps aux | awk '{if ($2 == %d) print $3}'",
           id);
  if (0 == util::exec(command, temp, sizeof(temp))) {
    cpu = atof(temp);
  }
#endif /* } */
    return cpu;
  __LEAVE_FUNCTION
    return -1.0f;
}

uint64_t get_virtualmemory_usage(int32_t id) {
  __ENTER_FUNCTION
    uint64_t result = 0;
#if __WINDOWS__ /* { */
    PROCESS_MEMORY_COUNTERS pmc;
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
    if (::GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
      result = pmc.PagefileUsage;
    }
#elif __LINUX__ /* }{ */
  char temp[128] = {0};
  char command[128] = {0};
  snprintf(command, 
           sizeof(command) - 1, 
           "ps aux | awk '{if ($2 == %d) print $5}'",
           id);
  if (0 == util::exec(command, temp, sizeof(temp))) {
    char *endpointer = NULL;
    result = strtouint64(temp, &endpointer, 10);
    result *= 1024;
  }
#endif /* } */
    return result;
  __LEAVE_FUNCTION
    return 0;
}

uint64_t get_physicalmemory_usage(int32_t id) {
  __ENTER_FUNCTION
    uint64_t result = 0;
#if __WINDOWS__ /* { */
    PROCESS_MEMORY_COUNTERS pmc;
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
    if (::GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
      result = pmc.WorkingSetSize;
    }
#elif __LINUX__ /* }{ */
  char temp[128] = {0};
  char command[128] = {0};
  snprintf(command, 
           sizeof(command) - 1, 
           "ps aux | awk '{if ($2 == %d) print $6}'",
           id);
  if (0 == util::exec(command, temp, sizeof(temp))) {
    char *endpointer = NULL;
    result = strtouint64(temp, &endpointer, 10);
    result *= 1024;
  }
#endif /* } */
    return result;
  __LEAVE_FUNCTION
    return 0;
}


bool daemon() {
  __ENTER_FUNCTION
#if __LINUX__
    pid_t pid;
    if ((pid = fork()) != 0) exit(0);
    setsid();
    signal(SIGHUP, SIG_IGN);
    if ((pid = fork()) != 0) exit(0);
    umask(0);
    for (uint8_t i = 0; i < 3; i++) close(i);
    return true;
#endif
  __LEAVE_FUNCTION
    return false;
}

} //namespace process

} //namespace ps_common_sys
