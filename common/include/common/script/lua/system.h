/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id system.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2014/06/29 10:51
 * @uses script lua module system
 */
#ifndef PS_COMMON_SCRIPT_LUA_SYSTEM_H_
#define PS_COMMON_SCRIPT_LUA_SYSTEM_H_

#include "common/script/lua/config.h"
#include "common/base/singleton.h"
#include "common/script/lua/interface.h"

namespace ps_common_script {

namespace lua {

class System : public ps_common_base::Singleton<System>, Interface {

 public:
   System();
   ~System();

 public:
   static System *getsingleton_pointer();
   static System &getsingleton();

 public:
   static int32_t call_noclosure(lua_State *L);

};

}; //namespace lua

}; //namespace ps_common_script

#define SCRIPT_LUASYSTEM_POINTER \
ps_common_script::lua::System::getsingleton_pointer()

#endif //PS_COMMON_SCRIPT_LUA_SYSTEM_H_
