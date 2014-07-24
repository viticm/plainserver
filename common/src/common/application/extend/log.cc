#include "common/application/extend/log.h"

namespace ps_common_base {

const char *g_extend_log_filename[] = {
  "./log/login", //kLoginLogFile
  "./log/sharememory", //kShareMemoryLogFile
  "./log/gateway", //kGatewayLogFile
  "./log/center", //kCenterLogFile
  "./log/server", //kServerLogFile
  '\0',
};

} //namespace ps_common_base
