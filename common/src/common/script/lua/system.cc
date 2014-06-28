#include "common/base/string.h"
#include "common/script/cache/manager.h"
#include "common/script/lua/system.h"

namespace ps_common_script {

namespace lua {

#define LUA_DISABLE_VERIFYFUNCTION

bool System::lua_reloadscript_always_ = true;
bool System::lua_recordscript_step_ = false;
const char *System::kLuaScriptStepFileName = "SCRIPT_STEP_FILENAME";
const int32_t System::kLuaScriptCountMax = 4096;

template <> System *ps_common_base::Singleton<System>::singleton_ = NULL;

System *System::getsingleton_pointer() {
  return singleton_;
}

System &System::getsingleton() {
  Assert(singleton_);
  return *singleton_;
}

System::System() {
  __ENTER_FUNCTION
    using namespace ps_common_base;
    string::safecopy(global_filename_, 
                     SCRIPT_LUA_GLOBAL_VAR_FILE_DEFAULT, 
                     sizeof(global_filename_));
  __LEAVE_FUNCTION
}

System::~System() {
  __ENTER_FUNCTION
    release();
  __LEAVE_FUNCTION
}

void System::set_globalfile(const char *filename) {
  __ENTER_FUNCTION
    using namespace ps_common_base;
    string::safecopy(global_filename_, filename, sizeof(global_filename_));
  __LEAVE_FUNCTION
}

void System::init() {
  __ENTER_FUNCTION
    bool result = VM_.init(4 * 1024);
    Assert(result && "VM_.init(4 * 1024) failed");
    register_function();
    result = loadscript(global_filename_);
    script_loaded_.init(kLuaScriptCountMax);
  __LEAVE_FUNCTION
}

void System::release() {
  __ENTER_FUNCTION
    VM_.release();
  __LEAVE_FUNCTION
}

bool System::find_function(lua_State *L, const char *functionname) {
  __ENTER_FUNCTION
    lua_getglobal(L, functionname);
    if (lua_isnil(L, -1)) {
      lua_pop(L, 1);
      return false;
    }
    lua_pop(L, 1);
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool System::verify_function(lua_State *L, const char **functionname) {
  __ENTER_FUNCTION
    if (!find_function(L, *functionname)) {
#ifdef LUA_DISABLE_VERIFYFUNCTION /* { */
      char *src = strchr(*functionname, '_') + 1;
      memmove(*functionname, src, strlen(src) + 1);
      if (!find_function(L, *functionname)) {
#ifdef _DEBUG
        char buffer[256] = {0};
        lua_getglobal(L, kLuaScriptStepFileName);
        const char *filename = lua_tostring(L, 1);
        snprintf(buffer, 
                 sizeof(buffer) - 1, 
                 "can't find function(%s) in file(%s)",
                 *functionname,
                 filename);
        lua_pop(L, 1);
        AssertEx(false, buffer);
#endif
        return false;
      }
#else /* }{ */

#ifdef _DEBUG
      char buffer[256] = {0};
      char *filename = lua_getglobal(L, kLuaScriptStepFileName);
      snprintf(buffer, 
               sizeof(buffer) - 1, 
               "can't find function(%s) in file(%s)",
               *functionname,
               filename);
      lua_pop(L, 1);
      AssertEx(false, buffer);
#endif

#endif /* } */
      return false;
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

int64_t System::run_filefunction(const char *filename, 
                                 const char *functionname, 
                                 bool load) {
  __ENTER_FUNCTION
    mark_scriptname(VM_.lua_state_, filename);
    if (!verify_function(VM_.lua_state_, &functionname)) return 0;
    int32_t topindex = 0;
    VM_.callfunction_enter(&topindex);
    bool result = VM_.callfunction(functionname, 1);
    int64_t _result = static_cast<int64_t>(lua_tonumber(VM_.lua_state_, -1));
    VM_.callfunction_leave(topindex);
    if (!result) {
      FAST_ERRORLOG(kErrorLogFile,
                    "[script][lua] (System::run_filefunction) lua stack leave"
                    "have some error, file: %s, functionname: %s",
                    filename,
                    functionname);
    }
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_filefunction(const char *filename, 
                                 const char *functionname, 
                                 int64_t param0,
                                 bool load) {
  __ENTER_FUNCTION
    mark_scriptname(VM_.lua_state_, filename);
    if (!verify_function(VM_.lua_state_, &functionname)) return 0;
    int32_t topindex = 0;
    VM_.callfunction_enter(&topindex);
    bool result = VM_.callfunction(functionname, 1, param0);
    int64_t _result = static_cast<int64_t>(lua_tonumber(VM_.lua_state_, -1));
    VM_.callfunction_leave(topindex);
    if (!result) {
      FAST_ERRORLOG(kErrorLogFile,
                    "[script][lua] (System::run_filefunction) lua stack leave"
                    " have some error, file: %s, functionname: %s"
                    " params: (%d)",
                    filename,
                    functionname,
                    param0);
    }
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_filefunction(const char *filename, 
                                 const char *functionname, 
                                 int64_t param0,
                                 int64_t param1,
                                 bool load) {
  __ENTER_FUNCTION
    mark_scriptname(VM_.lua_state_, filename);
    if (!verify_function(VM_.lua_state_, &functionname)) return 0;
    int32_t topindex = 0;
    VM_.callfunction_enter(&topindex);
    bool result = VM_.callfunction(functionname, 1, param0, param1);
    int64_t _result = static_cast<int64_t>(lua_tonumber(VM_.lua_state_, -1));
    VM_.callfunction_leave(topindex);
    if (!result) {
      FAST_ERRORLOG(kErrorLogFile,
                    "[script][lua] (System::run_filefunction) lua stack leave"
                    " have some error, file: %s, functionname: %s"
                    " params: (%d, %d)",
                    filename,
                    functionname,
                    param0,
                    param1);
    }
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_filefunction(const char *filename, 
                                 const char *functionname, 
                                 int64_t param0,
                                 int64_t param1,
                                 int64_t param2,
                                 bool load) {
  __ENTER_FUNCTION
    mark_scriptname(VM_.lua_state_, filename);
    if (!verify_function(VM_.lua_state_, &functionname)) return 0;
    int32_t topindex = 0;
    VM_.callfunction_enter(&topindex);
    bool result = VM_.callfunction(functionname, 1, param0, param1, param2);
    int64_t _result = static_cast<int64_t>(lua_tonumber(VM_.lua_state_, -1));
    VM_.callfunction_leave(topindex);
    if (!result) {
      FAST_ERRORLOG(kErrorLogFile,
                    "[script][lua] (System::run_filefunction) lua stack leave"
                    " have some error, file: %s, functionname: %s"
                    " params: (%d, %d, %d)",
                    filename,
                    functionname,
                    param0,
                    param1,
                    param2);
    }
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_filefunction(const char *filename, 
                                 const char *functionname, 
                                 int64_t param0,
                                 int64_t param1,
                                 int64_t param2,
                                 int64_t param3,
                                 bool load) {
  __ENTER_FUNCTION
    mark_scriptname(VM_.lua_state_, filename);
    if (!verify_function(VM_.lua_state_, &functionname)) return 0;
    int32_t topindex = 0;
    VM_.callfunction_enter(&topindex);
    bool result = VM_.callfunction(functionname, 
                                   1, 
                                   param0, 
                                   param1,
                                   param2,
                                   param3);
    int64_t _result = static_cast<int64_t>(lua_tonumber(VM_.lua_state_, -1));
    VM_.callfunction_leave(topindex);
    if (!result) {
      FAST_ERRORLOG(kErrorLogFile,
                    "[script][lua] (System::run_filefunction) lua stack leave"
                    " have some error, file: %s, functionname: %s"
                    " params: (%d, %d, %d, %d)",
                    filename,
                    functionname,
                    param0,
                    param1,
                    param2,
                    param3);
    }
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_filefunction(const char *filename, 
                                 const char *functionname, 
                                 int64_t param0,
                                 int64_t param1,
                                 int64_t param2,
                                 int64_t param3,
                                 int64_t param4,
                                 bool load) {
  __ENTER_FUNCTION
    mark_scriptname(VM_.lua_state_, filename);
    if (!verify_function(VM_.lua_state_, &functionname)) return 0;
    int32_t topindex = 0;
    VM_.callfunction_enter(&topindex);
    bool result = VM_.callfunction(functionname, 
                                   1, 
                                   param0, 
                                   param1,
                                   param2,
                                   param3,
                                   param4);
    int64_t _result = static_cast<int64_t>(lua_tonumber(VM_.lua_state_, -1));
    VM_.callfunction_leave(topindex);
    if (!result) {
      FAST_ERRORLOG(kErrorLogFile,
                    "[script][lua] (System::run_filefunction) lua stack leave"
                    " have some error, file: %s, functionname: %s"
                    " params: (%d, %d, %d, %d, %d)",
                    filename,
                    functionname,
                    param0,
                    param1,
                    param2,
                    param3,
                    param4);
    }
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_filefunction(const char *filename, 
                                 const char *functionname, 
                                 int64_t param0,
                                 int64_t param1,
                                 int64_t param2,
                                 int64_t param3,
                                 int64_t param4,
                                 int64_t param5,
                                 int64_t param6,
                                 bool load) {
  __ENTER_FUNCTION
    mark_scriptname(VM_.lua_state_, filename);
    if (!verify_function(VM_.lua_state_, &functionname)) return 0;
    int32_t topindex = 0;
    VM_.callfunction_enter(&topindex);
    bool result = VM_.callfunction(functionname, 
                                   1, 
                                   param0, 
                                   param1,
                                   param2,
                                   param3,
                                   param4,
                                   param5,
                                   param6);
    int64_t _result = static_cast<int64_t>(lua_tonumber(VM_.lua_state_, -1));
    VM_.callfunction_leave(topindex);
    if (!result) {
      FAST_ERRORLOG(kErrorLogFile,
                    "[script][lua] (System::run_filefunction) lua stack leave"
                    " have some error, file: %s, functionname: %s"
                    " params: (%d, %d, %d, %d, %d, %d, %d)",
                    filename,
                    functionname,
                    param0,
                    param1,
                    param2,
                    param3,
                    param4,
                    param5,
                    param6);
    }
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_filefunction(const char *filename, 
                                 const char *functionname, 
                                 int64_t param0,
                                 int64_t param1,
                                 int64_t param2,
                                 int64_t param3,
                                 int64_t param4,
                                 int64_t param5,
                                 int64_t param6,
                                 int64_t param7,
                                 bool load) {
  __ENTER_FUNCTION
    mark_scriptname(VM_.lua_state_, filename);
    if (!verify_function(VM_.lua_state_, &functionname)) return 0;
    int32_t topindex = 0;
    VM_.callfunction_enter(&topindex);
    bool result = VM_.callfunction(functionname, 
                                   1, 
                                   param0, 
                                   param1,
                                   param2,
                                   param3,
                                   param4,
                                   param5,
                                   param6,
                                   param7);
    int64_t _result = static_cast<int64_t>(lua_tonumber(VM_.lua_state_, -1));
    VM_.callfunction_leave(topindex);
    if (!result) {
      FAST_ERRORLOG(kErrorLogFile,
                    "[script][lua] (System::run_filefunction) lua stack leave"
                    " have some error, file: %s, functionname: %s"
                    " params: (%d, %d, %d, %d, %d, %d, %d, %d, %d)",
                    filename,
                    functionname,
                    param0,
                    param1,
                    param2,
                    param3,
                    param4,
                    param5,
                    param6,
                    param7);
    }
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_filefunction(const char *filename, 
                                 const char *functionname, 
                                 int64_t param0,
                                 int64_t param1,
                                 float param2,
                                 float param3,
                                 bool load) {
  __ENTER_FUNCTION
    mark_scriptname(VM_.lua_state_, filename);
    if (!verify_function(VM_.lua_state_, &functionname)) return 0;
    int32_t topindex = 0;
    VM_.callfunction_enter(&topindex);
    bool result = VM_.callfunction(functionname, 
                                   1, 
                                   param0, 
                                   param1,
                                   param2,
                                   param3);
    int64_t _result = static_cast<int64_t>(lua_tonumber(VM_.lua_state_, -1));
    VM_.callfunction_leave(topindex);
    if (!result) {
      FAST_ERRORLOG(kErrorLogFile,
                    "[script][lua] (System::run_filefunction) lua stack leave"
                    " have some error, file: %s, functionname: %s"
                    " params: (%d, %d, %f, %f)",
                    filename,
                    functionname,
                    param0,
                    param1,
                    param2,
                    param3);
    }
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_filefunction(const char *filename, 
                                 const char *functionname, 
                                 int64_t param0,
                                 int64_t param1,
                                 const char *param2,
                                 const char *param3,
                                 bool load) {
  __ENTER_FUNCTION
    mark_scriptname(VM_.lua_state_, filename);
    if (!verify_function(VM_.lua_state_, &functionname)) return 0;
    int32_t topindex = 0;
    VM_.callfunction_enter(&topindex);
    bool result = VM_.callfunction(functionname, 
                                   1, 
                                   param0, 
                                   param1,
                                   param2,
                                   param3);
    int64_t _result = static_cast<int64_t>(lua_tonumber(VM_.lua_state_, -1));
    VM_.callfunction_leave(topindex);
    if (!result) {
      FAST_ERRORLOG(kErrorLogFile,
                    "[script][lua] (System::run_filefunction) lua stack leave"
                    " have some error, file: %s, functionname: %s"
                    " params: (%d, %d, %s, %s)",
                    filename,
                    functionname,
                    param0,
                    param1,
                    param2,
                    param3);
    }
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_scriptfunction(int32_t scriptid, 
                                   const char *functionname) {
  __ENTER_FUNCTION
    //进入步骤
    enter_runstep(scriptid, functionname);
    //取得脚本载入状态
    bool loaded = false;
    cache::Base cachedata = reinterpret_cast<Base *>(getscript_byid(scriptid));
    if (NULL == cachedata) {
      cachedata = getscript_filedata(scriptid);
      script_loaded_.add(scriptid, cachedata);
      loaded = true;
    }
    //检查脚本载入结果
    check_scriptvalid(cachedata, scriptid, functionname);
    //载入失败
    if (NULL == cachedata) {
      leave_runstep(scriptid, functionname);
      return 0;
    }
    //载入成功
    const char *filename = cachedata->get_filename();
    if (NULL == filename) {
      leave_runstep(filename);
      return 0;
    }
    char functionname_x[128] = {0};
    snprintf(functionname_x, 
             sizeof(functionname_x) - 1, 
             "x%.6d_%s", 
             scriptid, 
             functionname);
    if (lua_reloadscript_always_) loaded = true;
    bool result = true;
    if (loaded) {
      result = loadscript(filename);
      if (!result) {
        leave_runstep(scriptid, functionname);
        return 0;
      }
      if (!lua_reloadscript_always_) {
        FAST_LOG(kDebugLogFile,
                 "[script][lua] (System::run_scriptfunction)"
                 " scriptid: %d, functionname: %s"
                 " params: ()",
                 scriptid,
                 functionname);
      }
    }
    int64_t _result = run_filefunction(filename, functionname_x, loaded);
    leave_runstep(scriptid, functionname);
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_scriptfunction(int32_t scriptid, 
                                   const char *functionname,
                                   int64_t param0) {
  __ENTER_FUNCTION
    //进入步骤
    enter_runstep(scriptid, functionname);
    //取得脚本载入状态
    bool loaded = false;
    cache::Base cachedata = reinterpret_cast<Base *>(getscript_byid(scriptid));
    if (NULL == cachedata) {
      cachedata = getscript_filedata(scriptid);
      script_loaded_.add(scriptid, cachedata);
      loaded = true;
    }
    //检查脚本载入结果
    check_scriptvalid(cachedata, scriptid, functionname);
    //载入失败
    if (NULL == cachedata) {
      leave_runstep(scriptid, functionname);
      return 0;
    }
    //载入成功
    const char *filename = cachedata->get_filename();
    if (NULL == filename) {
      leave_runstep(filename);
      return 0;
    }
    char functionname_x[128] = {0};
    snprintf(functionname_x, 
             sizeof(functionname_x) - 1, 
             "x%.6d_%s", 
             scriptid, 
             functionname);
    if (lua_reloadscript_always_) loaded = true;
    bool result = true;
    if (loaded) {
      result = loadscript(filename);
      if (!result) {
        leave_runstep(scriptid, functionname);
        return 0;
      }
      if (!lua_reloadscript_always_) {
        FAST_LOG(kDebugLogFile,
                 "[script][lua] (System::run_scriptfunction)"
                 " scriptid: %d, functionname: %s"
                 " params: (%d)",
                 scriptid,
                 functionname,
                 param0);
      }
    }
    int64_t _result = run_filefunction(filename, 
                                       functionname_x, 
                                       param0, 
                                       loaded);
    leave_runstep(scriptid, functionname);
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_scriptfunction(int32_t scriptid, 
                                   const char *functionname,
                                   int64_t param0,
                                   int64_t param1) {
  __ENTER_FUNCTION
    //进入步骤
    enter_runstep(scriptid, functionname);
    //取得脚本载入状态
    bool loaded = false;
    cache::Base cachedata = reinterpret_cast<Base *>(getscript_byid(scriptid));
    if (NULL == cachedata) {
      cachedata = getscript_filedata(scriptid);
      script_loaded_.add(scriptid, cachedata);
      loaded = true;
    }
    //检查脚本载入结果
    check_scriptvalid(cachedata, scriptid, functionname);
    //载入失败
    if (NULL == cachedata) {
      leave_runstep(scriptid, functionname);
      return 0;
    }
    //载入成功
    const char *filename = cachedata->get_filename();
    if (NULL == filename) {
      leave_runstep(filename);
      return 0;
    }
    char functionname_x[128] = {0};
    snprintf(functionname_x, 
             sizeof(functionname_x) - 1, 
             "x%.6d_%s", 
             scriptid, 
             functionname);
    if (lua_reloadscript_always_) loaded = true;
    bool result = true;
    if (loaded) {
      result = loadscript(filename);
      if (!result) {
        leave_runstep(scriptid, functionname);
        return 0;
      }
      if (!lua_reloadscript_always_) {
        FAST_LOG(kDebugLogFile,
                 "[script][lua] (System::run_scriptfunction)"
                 " scriptid: %d, functionname: %s"
                 " params: (%d, %d)",
                 scriptid,
                 functionname,
                 param0,
                 param1);
      }
    }
    int64_t _result = run_filefunction(filename, 
                                       functionname_x, 
                                       param0, 
                                       param1, 
                                       loaded);
    leave_runstep(scriptid, functionname);
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_scriptfunction(int32_t scriptid, 
                                   const char *functionname,
                                   int64_t param0,
                                   int64_t param1,
                                   int64_t param2) {
  __ENTER_FUNCTION
    //进入步骤
    enter_runstep(scriptid, functionname);
    //取得脚本载入状态
    bool loaded = false;
    cache::Base cachedata = reinterpret_cast<Base *>(getscript_byid(scriptid));
    if (NULL == cachedata) {
      cachedata = getscript_filedata(scriptid);
      script_loaded_.add(scriptid, cachedata);
      loaded = true;
    }
    //检查脚本载入结果
    check_scriptvalid(cachedata, scriptid, functionname);
    //载入失败
    if (NULL == cachedata) {
      leave_runstep(scriptid, functionname);
      return 0;
    }
    //载入成功
    const char *filename = cachedata->get_filename();
    if (NULL == filename) {
      leave_runstep(filename);
      return 0;
    }
    char functionname_x[128] = {0};
    snprintf(functionname_x, 
             sizeof(functionname_x) - 1, 
             "x%.6d_%s", 
             scriptid, 
             functionname);
    if (lua_reloadscript_always_) loaded = true;
    bool result = true;
    if (loaded) {
      result = loadscript(filename);
      if (!result) {
        leave_runstep(scriptid, functionname);
        return 0;
      }
      if (!lua_reloadscript_always_) {
        FAST_LOG(kDebugLogFile,
                 "[script][lua] (System::run_scriptfunction)"
                 " scriptid: %d, functionname: %s"
                 " params: (%d, %d, %d)",
                 scriptid,
                 functionname,
                 param0,
                 param1,
                 param2);
      }
    }
    int64_t _result = run_filefunction(filename, 
                                       functionname_x, 
                                       param0, 
                                       param1, 
                                       param2, 
                                       loaded);
    leave_runstep(scriptid, functionname);
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_scriptfunction(int32_t scriptid, 
                                   const char *functionname,
                                   int64_t param0,
                                   int64_t param1,
                                   int64_t param2,
                                   int64_t param3) {
  __ENTER_FUNCTION
    //进入步骤
    enter_runstep(scriptid, functionname);
    //取得脚本载入状态
    bool loaded = false;
    cache::Base cachedata = reinterpret_cast<Base *>(getscript_byid(scriptid));
    if (NULL == cachedata) {
      cachedata = getscript_filedata(scriptid);
      script_loaded_.add(scriptid, cachedata);
      loaded = true;
    }
    //检查脚本载入结果
    check_scriptvalid(cachedata, scriptid, functionname);
    //载入失败
    if (NULL == cachedata) {
      leave_runstep(scriptid, functionname);
      return 0;
    }
    //载入成功
    const char *filename = cachedata->get_filename();
    if (NULL == filename) {
      leave_runstep(filename);
      return 0;
    }
    char functionname_x[128] = {0};
    snprintf(functionname_x, 
             sizeof(functionname_x) - 1, 
             "x%.6d_%s", 
             scriptid, 
             functionname);
    if (lua_reloadscript_always_) loaded = true;
    bool result = true;
    if (loaded) {
      result = loadscript(filename);
      if (!result) {
        leave_runstep(scriptid, functionname);
        return 0;
      }
      if (!lua_reloadscript_always_) {
        FAST_LOG(kDebugLogFile,
                 "[script][lua] (System::run_scriptfunction)"
                 " scriptid: %d, functionname: %s"
                 " params: (%d, %d, %d, %d)",
                 scriptid,
                 functionname,
                 param0,
                 param1,
                 param2,
                 param3);
      }
    }
    int64_t _result = run_filefunction(filename, 
                                       functionname_x, 
                                       param0, 
                                       param1, 
                                       param2, 
                                       param3, 
                                       loaded);
    leave_runstep(scriptid, functionname);
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_scriptfunction(int32_t scriptid, 
                                   const char *functionname,
                                   int64_t param0,
                                   int64_t param1,
                                   int64_t param2,
                                   int64_t param3,
                                   int64_t param4) {
  __ENTER_FUNCTION
    //进入步骤
    enter_runstep(scriptid, functionname);
    //取得脚本载入状态
    bool loaded = false;
    cache::Base cachedata = reinterpret_cast<Base *>(getscript_byid(scriptid));
    if (NULL == cachedata) {
      cachedata = getscript_filedata(scriptid);
      script_loaded_.add(scriptid, cachedata);
      loaded = true;
    }
    //检查脚本载入结果
    check_scriptvalid(cachedata, scriptid, functionname);
    //载入失败
    if (NULL == cachedata) {
      leave_runstep(scriptid, functionname);
      return 0;
    }
    //载入成功
    const char *filename = cachedata->get_filename();
    if (NULL == filename) {
      leave_runstep(filename);
      return 0;
    }
    char functionname_x[128] = {0};
    snprintf(functionname_x, 
             sizeof(functionname_x) - 1, 
             "x%.6d_%s", 
             scriptid, 
             functionname);
    if (lua_reloadscript_always_) loaded = true;
    bool result = true;
    if (loaded) {
      result = loadscript(filename);
      if (!result) {
        leave_runstep(scriptid, functionname);
        return 0;
      }
      if (!lua_reloadscript_always_) {
        FAST_LOG(kDebugLogFile,
                 "[script][lua] (System::run_scriptfunction)"
                 " scriptid: %d, functionname: %s"
                 " params: (%d, %d, %d, %d, %d)",
                 scriptid,
                 functionname,
                 param0,
                 param1,
                 param2,
                 param3,
                 param4);
      }
    }
    int64_t _result = run_filefunction(filename, 
                                       functionname_x, 
                                       param0, 
                                       param1, 
                                       param2, 
                                       param3, 
                                       param4, 
                                       loaded);
    leave_runstep(scriptid, functionname);
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_scriptfunction(int32_t scriptid, 
                                   const char *functionname,
                                   int64_t param0,
                                   int64_t param1,
                                   int64_t param2,
                                   int64_t param3,
                                   int64_t param4,
                                   int64_t param5) {
  __ENTER_FUNCTION
    //进入步骤
    enter_runstep(scriptid, functionname);
    //取得脚本载入状态
    bool loaded = false;
    cache::Base cachedata = reinterpret_cast<Base *>(getscript_byid(scriptid));
    if (NULL == cachedata) {
      cachedata = getscript_filedata(scriptid);
      script_loaded_.add(scriptid, cachedata);
      loaded = true;
    }
    //检查脚本载入结果
    check_scriptvalid(cachedata, scriptid, functionname);
    //载入失败
    if (NULL == cachedata) {
      leave_runstep(scriptid, functionname);
      return 0;
    }
    //载入成功
    const char *filename = cachedata->get_filename();
    if (NULL == filename) {
      leave_runstep(filename);
      return 0;
    }
    char functionname_x[128] = {0};
    snprintf(functionname_x, 
             sizeof(functionname_x) - 1, 
             "x%.6d_%s", 
             scriptid, 
             functionname);
    if (lua_reloadscript_always_) loaded = true;
    bool result = true;
    if (loaded) {
      result = loadscript(filename);
      if (!result) {
        leave_runstep(scriptid, functionname);
        return 0;
      }
      if (!lua_reloadscript_always_) {
        FAST_LOG(kDebugLogFile,
                 "[script][lua] (System::run_scriptfunction)"
                 " scriptid: %d, functionname: %s"
                 " params: (%d, %d, %d, %d, %d, %d)",
                 scriptid,
                 functionname,
                 param0,
                 param1,
                 param2,
                 param3,
                 param4,
                 param5);
      }
    }
    int64_t _result = run_filefunction(filename, 
                                       functionname_x, 
                                       param0, 
                                       param1, 
                                       param2, 
                                       param3, 
                                       param4, 
                                       param5, 
                                       loaded);
    leave_runstep(scriptid, functionname);
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_scriptfunction(int32_t scriptid, 
                                   const char *functionname,
                                   int64_t param0,
                                   int64_t param1,
                                   int64_t param2,
                                   int64_t param3,
                                   int64_t param4,
                                   int64_t param5,
                                   int64_t param6) {
  __ENTER_FUNCTION
    //进入步骤
    enter_runstep(scriptid, functionname);
    //取得脚本载入状态
    bool loaded = false;
    cache::Base cachedata = reinterpret_cast<Base *>(getscript_byid(scriptid));
    if (NULL == cachedata) {
      cachedata = getscript_filedata(scriptid);
      script_loaded_.add(scriptid, cachedata);
      loaded = true;
    }
    //检查脚本载入结果
    check_scriptvalid(cachedata, scriptid, functionname);
    //载入失败
    if (NULL == cachedata) {
      leave_runstep(scriptid, functionname);
      return 0;
    }
    //载入成功
    const char *filename = cachedata->get_filename();
    if (NULL == filename) {
      leave_runstep(filename);
      return 0;
    }
    char functionname_x[128] = {0};
    snprintf(functionname_x, 
             sizeof(functionname_x) - 1, 
             "x%.6d_%s", 
             scriptid, 
             functionname);
    if (lua_reloadscript_always_) loaded = true;
    bool result = true;
    if (loaded) {
      result = loadscript(filename);
      if (!result) {
        leave_runstep(scriptid, functionname);
        return 0;
      }
      if (!lua_reloadscript_always_) {
        FAST_LOG(kDebugLogFile,
                 "[script][lua] (System::run_scriptfunction)"
                 " scriptid: %d, functionname: %s"
                 " params: (%d, %d, %d, %d, %d, %d, %d)",
                 scriptid,
                 functionname,
                 param0,
                 param1,
                 param2,
                 param3,
                 param4,
                 param5,
                 param6);
      }
    }
    int64_t _result = run_filefunction(filename, 
                                       functionname_x, 
                                       param0, 
                                       param1, 
                                       param2, 
                                       param3, 
                                       param4, 
                                       param5, 
                                       param6, 
                                       loaded);
    leave_runstep(scriptid, functionname);
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_scriptfunction(int32_t scriptid, 
                                   const char *functionname,
                                   int64_t param0,
                                   int64_t param1,
                                   int64_t param2,
                                   int64_t param3,
                                   int64_t param4,
                                   int64_t param5,
                                   int64_t param6,
                                   int64_t param7) {
  __ENTER_FUNCTION
    //进入步骤
    enter_runstep(scriptid, functionname);
    //取得脚本载入状态
    bool loaded = false;
    cache::Base cachedata = reinterpret_cast<Base *>(getscript_byid(scriptid));
    if (NULL == cachedata) {
      cachedata = getscript_filedata(scriptid);
      script_loaded_.add(scriptid, cachedata);
      loaded = true;
    }
    //检查脚本载入结果
    check_scriptvalid(cachedata, scriptid, functionname);
    //载入失败
    if (NULL == cachedata) {
      leave_runstep(scriptid, functionname);
      return 0;
    }
    //载入成功
    const char *filename = cachedata->get_filename();
    if (NULL == filename) {
      leave_runstep(filename);
      return 0;
    }
    char functionname_x[128] = {0};
    snprintf(functionname_x, 
             sizeof(functionname_x) - 1, 
             "x%.6d_%s", 
             scriptid, 
             functionname);
    if (lua_reloadscript_always_) loaded = true;
    bool result = true;
    if (loaded) {
      result = loadscript(filename);
      if (!result) {
        leave_runstep(scriptid, functionname);
        return 0;
      }
      if (!lua_reloadscript_always_) {
        FAST_LOG(kDebugLogFile,
                 "[script][lua] (System::run_scriptfunction)"
                 " scriptid: %d, functionname: %s"
                 " params: (%d, %d, %d, %d, %d, %d, %d, %d)",
                 scriptid,
                 functionname,
                 param0,
                 param1,
                 param2,
                 param3,
                 param4,
                 param5,
                 param6,
                 param7);
      }
    }
    int64_t _result = run_filefunction(filename, 
                                       functionname_x, 
                                       param0, 
                                       param1, 
                                       param2, 
                                       param3, 
                                       param4, 
                                       param5, 
                                       param6, 
                                       param7, 
                                       loaded);
    leave_runstep(scriptid, functionname);
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_scriptfunction(int32_t scriptid, 
                                   const char *functionname,
                                   int64_t param0,
                                   int64_t param1,
                                   float param2,
                                   float param3) {
  __ENTER_FUNCTION
    //进入步骤
    enter_runstep(scriptid, functionname);
    //取得脚本载入状态
    bool loaded = false;
    cache::Base cachedata = reinterpret_cast<Base *>(getscript_byid(scriptid));
    if (NULL == cachedata) {
      cachedata = getscript_filedata(scriptid);
      script_loaded_.add(scriptid, cachedata);
      loaded = true;
    }
    //检查脚本载入结果
    check_scriptvalid(cachedata, scriptid, functionname);
    //载入失败
    if (NULL == cachedata) {
      leave_runstep(scriptid, functionname);
      return 0;
    }
    //载入成功
    const char *filename = cachedata->get_filename();
    if (NULL == filename) {
      leave_runstep(filename);
      return 0;
    }
    char functionname_x[128] = {0};
    snprintf(functionname_x, 
             sizeof(functionname_x) - 1, 
             "x%.6d_%s", 
             scriptid, 
             functionname);
    if (lua_reloadscript_always_) loaded = true;
    bool result = true;
    if (loaded) {
      result = loadscript(filename);
      if (!result) {
        leave_runstep(scriptid, functionname);
        return 0;
      }
      if (!lua_reloadscript_always_) {
        FAST_LOG(kDebugLogFile,
                 "[script][lua] (System::run_scriptfunction)"
                 " scriptid: %d, functionname: %s"
                 " params: (%d, %d, %f, %f)",
                 scriptid,
                 functionname,
                 param0,
                 param1,
                 param2,
                 param3);
      }
    }
    int64_t _result = run_filefunction(filename, 
                                       functionname_x, 
                                       param0, 
                                       param1, 
                                       param2, 
                                       param3, 
                                       loaded);
    leave_runstep(scriptid, functionname);
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

int64_t System::run_scriptfunction(int32_t scriptid, 
                                   const char *functionname,
                                   int64_t param0,
                                   int64_t param1,
                                   const char *param2,
                                   const char *param3) {
  __ENTER_FUNCTION
    //进入步骤
    enter_runstep(scriptid, functionname);
    //取得脚本载入状态
    bool loaded = false;
    cache::Base cachedata = reinterpret_cast<Base *>(getscript_byid(scriptid));
    if (NULL == cachedata) {
      cachedata = getscript_filedata(scriptid);
      script_loaded_.add(scriptid, cachedata);
      loaded = true;
    }
    //检查脚本载入结果
    check_scriptvalid(cachedata, scriptid, functionname);
    //载入失败
    if (NULL == cachedata) {
      leave_runstep(scriptid, functionname);
      return 0;
    }
    //载入成功
    const char *filename = cachedata->get_filename();
    if (NULL == filename) {
      leave_runstep(filename);
      return 0;
    }
    char functionname_x[128] = {0};
    snprintf(functionname_x, 
             sizeof(functionname_x) - 1, 
             "x%.6d_%s", 
             scriptid, 
             functionname);
    if (lua_reloadscript_always_) loaded = true;
    bool result = true;
    if (loaded) {
      result = loadscript(filename);
      if (!result) {
        leave_runstep(scriptid, functionname);
        return 0;
      }
      if (!lua_reloadscript_always_) {
        FAST_LOG(kDebugLogFile,
                 "[script][lua] (System::run_scriptfunction)"
                 " scriptid: %d, functionname: %s"
                 " params: (%d, %d, %s, %s)",
                 scriptid,
                 functionname,
                 param0,
                 param1,
                 param2,
                 param3);
      }
    }
    int64_t _result = run_filefunction(filename, 
                                       functionname_x, 
                                       param0, 
                                       param1, 
                                       param2, 
                                       param3, 
                                       loaded);
    leave_runstep(scriptid, functionname);
    return _result;
  __LEAVE_FUNCTION
    return 0;
}

void System::enter_runstep(int32_t scriptid, const char *functionname) {
  __ENTER_FUNCTION
    if (!lua_recordscript_step_) return;
    lua_Debug debug;
    int32_t topindex = lua_gettop(VM_.lua_state_);
    int32_t stackspace = lua_getstack(VM_.lua_state_, 2, &debug);
    FAST_LOG(kFunctionLogFile,
             "[script][lua] (System::enter_runstep)"
             " topindex: %d, stackspace: %d, scriptid: %d, functionname: %s",
             topindex,
             stackspace,
             scriptid,
             functionname);
  __LEAVE_FUNCTION
}

void System::leave_runstep(int32_t scriptid, const char *functionname) {
  __ENTER_FUNCTION
    if (!lua_recordscript_step_) return;
    lua_Debug debug;
    int32_t topindex = lua_gettop(VM_.lua_state_);
    int32_t stackspace = lua_getstack(VM_.lua_state_, 2, &debug);
    FAST_LOG(kFunctionLogFile,
             "[script][lua] (System::enter_runstep)"
             " topindex: %d, stackspace: %d, scriptid: %d, functionname: %s",
             topindex,
             stackspace,
             scriptid,
             functionname);
  __LEAVE_FUNCTION
}

bool System::reloadscript(int32_t scriptid) {
  __ENTER_FUNCTION
    cache::Base *cachedata = 
      reinterpret_cast<cache::Base *>(getscript_byid(scriptid));
    if (NULL == cachedata) {
      cachedata = getscript_filedata(scriptid);
      if (NULL == cachedata) return false;
      script_loaded_.add(scriptid, cachedata);
    }
    if (cachedata) {
      const char *filename = cachedata->get_filename();
      AssertEx(filename, filename);
      if (filename) {
        bool result = loadscript(filename);
        if (!result && !lua_reloadscript_always_) {
          FAST_LOG(kDebugLogFile,
                   "[script][lua] (System::reloadscript)"
                   " filename: %s, scriptid: %d",
                   filename,
                   scriptid);
        }
        return result;
      }
    }
    return false;
  __LEAVE_FUNCTION
    return false;
}

void *System::getscript_byid(int32_t scriptid) {
  __ENTER_FUNCTION
    return script_loaded_.get(scriptid);
  __LEAVE_FUNCTION
    return NULL;
}

bool System::addscript(int32_t scriptid, void *data) {
  __ENTER_FUNCTION
    return script_loaded_.add(scriptid, data);
  __LEAVE_FUNCTION
    return false;
}

bool System::loadscript(const char *filename) {
  __ENTER_FUNCTION
    return VM_.load(filename);
  __LEAVE_FUNCTION
    return false;
}

bool System::check_paramnumber(lua_State *L, int32_t count) {
  __ENTER_FUNCTION
    if (!L) return false;
    if (lua_gettop(L) < count) return false;
    return true;
  __LEAVE_FUNCTION
    return false;
}

int32_t System::getscript_stepid(lua_State *L) {
  return -1;
}

bool System::is_paramnumber(lua_State *L, 
                            int32_t index, 
                            const char *functionname) {
  __ENTER_FUNCTION
    if (!L) {
      FAST_ERRORLOG(kErrorLogFile,
                    "[script][lua] (System::is_paramnumber)"
                    " lua state is nil");
    }
    if (!lua_isnumber(L, index)) {
      FAST_ERRORLOG(kErrorLogFile,
                    "[script][lua] (System::is_paramnumber)"
                    " function: %s, param %d is illegal!",
                    functionname,
                    index);
      return false;
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool System::is_paramstring(lua_State *L, 
                            int32_t index, 
                            const char *functionname) {
  __ENTER_FUNCTION
    if (!L) {
      FAST_ERRORLOG(kErrorLogFile,
                    "[script][lua] (System::is_paramnumber)"
                    " lua state is nil");
    }
    if (!lua_isstring(L, index)) {
      FAST_ERRORLOG(kErrorLogFile,
                    "[script][lua] (System::is_paramnumber)"
                    " function: %s, param %d is illegal!",
                    functionname,
                    index);
      return false;
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

void System::mark_scriptname(lua_State *L, const char *name) {
#ifdef _DEBUG
  __ENTER_FUNCTION
    if (!L || !name) return;
    lua_pushstring(L, name);
    lua_setglobal(L, kLuaScriptStepFileName);
  __LEAVE_FUNCTION
#endif
}

void System::check_scriptvalid(void *file, 
                               int32_t scriptid, 
                               const char *functionname) {
#ifdef _DEBUG
  __ENTER_FUNCTION
    char buffer[256] = {0};
    snprintf(buffer, 
             sizeof(buffer) - 1, 
             "%s, scriptid=%d", 
             functionname, 
             scriptid);
    AssertEx(file, buffer);
  __LEAVE_FUNCTION
#endif
}

int32_t System::call_noclosure(lua_State *L) {
  __ENTER_FUNCTION
    int32_t argc = lua_gettop(L);
    AssertEx(argc >= 2, "params lest than 2");
    if (!is_paramnumber(L, 1, "call_noclosure")) {
      lua_pushnumber(L, -1);
      return 1;
    }
    if (!is_paramstring(L, 2, "call_noclosure")) {
      lua_pushnumber(L, -1);
      return 1;
    }
    if (!SCRIPT_LUASYSTEM_POINTER) {
      lua_pushnumber(L, -1);
      return 1;
    }
    int32_t scriptid = lua_tonumber(L, 1);
    const char *functionname = lua_tostring(L, 2);
    try {
      SCRIPT_LUASYSTEM_POINTER->enter_runstep(scriptid, functionname);
      bool loaded = false;
      cache::Base *cachedata = reinterpret_cast<cache::Base *>(
          SCRIPT_LUASYSTEM_POINTER->getscript_byid(scriptid));
      if (NULL == cachedata) {
        cachedata = getscript_filedata(scriptid);
        if (cachedata) {
          SCRIPT_LUASYSTEM_POINTER->addscript(scriptid, cachedata);
          loaded = true;
        } else {
          lua_pushnumber(L, -1);
          SCRIPT_LUASYSTEM_POINTER->leave_runstep(scriptid, functionname);
          return 1;
        }
      }
      const char *filename = cachedata->get_filename();
      if (lua_reloadscript_always_) loaded = true;
      if (loaded && filename) {
        bool result = SCRIPT_LUASYSTEM_POINTER->loadscript(filename);
        if (!result) {
          FAST_ERRORLOG(kErrorLogFile, 
                        "[script][lua] (System::call_noclosure)"
                        " load script: %s failed", 
                        filename);
          lua_pushnumber(L, -1);
          SCRIPT_LUASYSTEM_POINTER->leave_runstep(scriptid, functionname);
          return 1;
        }
        if (false == lua_reloadscript_always_) {
          FAST_LOG(kDebugLogFile,
                   "[script][lua] (System::call_noclosure)"
                   " script(%d), function(%s)",
                   scriptid,
                   functionname);
        }
      }
      if (cachedata && filename) {
        char functionname_x[128] = {0};
        snprintf(functionname_x,
                 sizeof(functionname_x) - 1,
                 "x%.6d_%s",
                 scriptid,
                 functionname);
        char *functionname_x_pointer = functionname_x;
        mark_scriptname(L, filename);
        if (!SCRIPT_LUASYSTEM_POINTER->verify_function(
              L, &functionname_x_pointer)) {
          FAST_ERRORLOG(kErrorLogFile,
                        "[script][lua] (System::call_noclosure)"
                        " SCRIPT_LUASYSTEM_POINTER->verify_function(%s) error",
                        functionname_x);
          lua_pushnumber(L, -1);
          SCRIPT_LUASYSTEM_POINTER->leave_runstep(scriptid, functionname);
          return 1;
        }
        try {
          lua_getglobal(L, functionname_x_pointer);
          int32_t paramindex_begin = 3;
          for (int32_t index = paramindex_begin; i < argc; ++index) {
            switch (lua_type(L, index)) {
              case LUA_TUSERDATA:
                Assert(false);
                break;
              case LUA_TNIL:
                lua_pushnil(L);
                break;
              case LUA_TNUMBER: {
                int32_t number = lua_tonumber(L, index);
                lua_pushnumber(L, number);
                break;
              }
              case LUA_TSTRING: {
                const char *str = lua_tostring(L, index);
                lua_pushstring(L, str);
                break;
              }
              case LUA_TTABLE:
                Assert(false);
                break;
              case LUA_TFUNCTION: 
                lua_pushcfunction(L, lua_tocfunction(L, index));
                break;
              default:
                lua_pushnil(L);
                break;
            } //switch
          } //for
          int32_t _result = -1;
          int32_t call_result = 
            lua_pcall(L, argc - paramindex_begin, _result, NULL);
          int32_t argnow = lua_gettop(L);
          _result = argnow - argc;
          for (int32_t index = 1; index <= _result; ++index) {
            switch (lua_type(L, index)) {
              case LUA_TUSERDATA:
                Assert(false);
                break;
              case LUA_TNIL:
                lua_pushnil(L);
                break;
              case LUA_TNUMBER: {
                int32_t number = lua_tonumber(L, index);
                lua_pushnumber(L, number);
                break;
              }
              case LUA_TSTRING: {
                const char *str = lua_tostring(L, index);
                lua_pushstring(L, str);
                break;
              }
              case LUA_TTABLE:
                Assert(false);
                break;
              case LUA_TFUNCTION: 
                lua_pushcfunction(L, lua_tocfunction(L, index));
                break;
              default:
                lua_pushnil(L);
                break;
            } //switch
          } //for
          if (call_result != 0) {
            FAST_ERRORLOG(kErrorLogFile,
                          "[script][lua] (System::call_noclosure) error"
                          " call_result: %d, file: %s, functionname: %s",
                          call_result,
                          filename,
                          functionname_x_pointer);
            SCRIPT_LUASYSTEM_POINTER->leave_runstep(scriptid, functionname);
            return _result;
          }
        } catch(...) {
          FAST_ERRORLOG(kErrorLogFile,
                        "[script][lua] (System::call_noclosure) error"
                        "lua_call get a exception, file: %s, functionname: %s",
                        filename,
                        functionname);
        }
      }
    } catch (...) {
      FAST_ERRORLOG(kErrorLogFile,
                    "[script][lua] (System::call_noclosure) error"
                    "lua_call get a exception, scriptid: %d, functionname: %s",
                    scriptid,
                    functionname);

    }
    lua_pushnumber(L, -1);
    return 1;
  __LEAVE_FUNCTION
    lua_pushnumber(L, -1);
    return 1;
}

}; //namespace lua

}; //namespace ps_common_script
