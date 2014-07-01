#include "gateway.h"
#include "common/engine/kernel.h"

int32_t main(int32_t argc, char * argv[]) {
  ps_common_engine::Kernel engine_kernel;
  engine_kernel.init();
  engine_kernel.run();
  engine_kernel.stop();
  return 0;
}
