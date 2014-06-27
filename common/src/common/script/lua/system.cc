#include "common/script/lua/system.h"

namespace ps_common_script {

namespace lua {

int32_t call_scriptfunction(lua_State *L) {
  __ENTER_FUNCTION
    int32_t argc = lua_gettop(L);
    AssertEx(argc >= 3, "params lest than 3");
  __LEAVE_FUNCTION
    lua_pushnumber(L, -1);
    return 1;
}

}; //namespace lua

}; //namespace ps_common_script
