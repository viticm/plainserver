#include "gateway.h"
#include "common/engine/kernel.h"
#include "common/base/log.h"

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
  ps_common_engine::Kernel engine_kernel;
  //engine_kernel.setconfig(ENGINE_CONFIG_DB_ISACTIVE, true);
  engine_kernel.setconfig(ENGINE_CONFIG_SCRIPT_ISACTIVE, true);
  engine_kernel.setconfig(ENGINE_CONFIG_PERFORMANCE_ISACTIVE, true);
  engine_kernel.setconfig(ENGINE_CONFIG_DB_CONNECTION_OR_DBNAME, "sword_user");
  engine_kernel.setconfig(ENGINE_CONFIG_NET_LISTEN_PORT, 8080);
  if (!engine_kernel.init()) {
    engine_kernel.stop();
    return 1;
  }
  engine_kernel.run(); //网络线程是来阻塞主线程的，所以不要轻易设置其独立线程模式
  DEBUGPRINTF("module is runing");
  engine_kernel.stop();
  return 0;
}
