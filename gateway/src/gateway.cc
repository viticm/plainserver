#include "common/engine/kernel.h"
#include "common/base/time_manager.h"
#include "common/base/log.h"
#include "common/sys/process.h"
#include "gateway.h"

ps_common_engine::Kernel engine_kernel;

void signal_handler(int32_t signal);

int32_t main(int32_t argc, char * argv[]) {
#if __WINDOWS__
  WORD versionrequested;
  WSADATA data;
  int32_t error;
  versionrequested = MAKEWORD(2, 2);
  error = WSAStartup(versionrequested, &data);
  _CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
  system("color 02"); //color green
  system("mode con cols=120"); //cmd size
#endif
  if (argc < 2 || strcmp(argv[1], "--daemon") != 0) {
    ERRORPRINTF("----------------------------------------"
                "----------------------------------------");
    ERRORPRINTF("                                   [WARNING]");
    ERRORPRINTF(" server not run in daemon mode, "
                "it will be auto stop when user logout or session");
    ERRORPRINTF(" disconnect. should add daemon option to start server"
                " as daemon mode, such as:");
    DEBUGPRINTF("     %s --daemon", argv[0]);
    ERRORPRINTF("----------------------------------------"
                "----------------------------------------");
  } else {
    ps_common_sys::process::daemon();
  }
  engine_kernel.setconfig(ENGINE_CONFIG_DB_ISACTIVE, true);
  engine_kernel.setconfig(ENGINE_CONFIG_SCRIPT_ISACTIVE, true);
  //engine_kernel.setconfig(ENGINE_CONFIG_PERFORMANCE_ISACTIVE, true);
  engine_kernel.setconfig(ENGINE_CONFIG_DB_CONNECTION_OR_DBNAME, "sword_user");
  //engine_kernel.setconfig(ENGINE_CONFIG_NET_CONNECTION_MAX, 100);
  engine_kernel.setconfig(ENGINE_CONFIG_NET_LISTEN_PORT, 8080);
  if (!engine_kernel.init()) {
    return 1;
  }
#if __LINUX__  
  signal(SIGINT, signal_handler);
  signal(SIGUSR1, signal_handler);
#endif  
  engine_kernel.run(); //网络线程是来阻塞主线程的，所以不要轻易设置其独立线程模式
  //engine_kernel.stop();
  return 0;
}

void signal_handler(int32_t signal) {
#if __LINUX__
  //处理前台模式信号
  static uint32_t last_signaltime = 0;
  uint32_t currenttime = TIME_MANAGER_POINTER->get_current_time();
  if (signal == SIGINT) {
    if (currenttime - last_signaltime > 10 * 1000) {
      DEBUGPRINTF("[gateway] signal got SIGINT[%d] will reload!", signal);
      //engine_kernel.stop();
    }
    else {
      WARNINGPRINTF("[gateway] signal got SIGINT[%d] will stop!", signal);
      engine_kernel.stop(); 
    }
  }
  //处理后台模式信号
  if (signal == SIGUSR1) {
    WARNINGPRINTF("[gateway] signal got SIGUSR1[%d] will stop!", signal);
    engine_kernel.stop();
  }
  last_signaltime = currenttime;
#endif
}
