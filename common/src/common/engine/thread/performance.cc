#include "common/engine/thread/performance.h"

using namespace ps_common_engine::thread;

Performance::Performance() {
  //do nothing
}

Performance::~Performance() {
  //do nothing
}

bool Performance::init() {
  return true;
}

void Performance::run() {
  //do nothing
}

void Performance::stop() {
  isactive_ = false;
}

bool isactive() {
  return isactive_;
}
