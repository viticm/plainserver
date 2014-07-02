#include "common/base/time_manager.h"
#include "common/base/log.h"
#include "common/base/util.h"
#include "common/script/lua/system.h"
#include "common/engine/kernel.h"

namespace ps_common_engine {

Kernel::Kernel() {
  __ENTER_FUNCTION
    config_int32_.init(100);
    config_string_.init(100);
    config_bool_.init(100);
    registerconfig(ENGINE_CONFIG_DB_ISACTIVE, false);
    registerconfig(ENGINE_CONFIG_NET_ISACTIVE, true);
    registerconfig(ENGINE_CONFIG_SCRIPT_ISACTIVE, false);
    registerconfig(ENGINE_CONFIG_PERFORMANCE_ISACTIVE, false);
    registerconfig(ENGINE_CONFIG_DB_CONNECTOR_TYPE, kDBConnectorTypeODBC);
    registerconfig(ENGINE_CONFIG_DB_CONNECTION_OR_DBNAME, "");
    registerconfig(ENGINE_CONFIG_DB_USERNAME, "");
    registerconfig(ENGINE_CONFIG_DB_PASSWORD, "");
    registerconfig(ENGINE_CONFIG_NET_LISTEN_PORT, 0);
    registerconfig(ENGINE_CONFIG_NET_CONNECTION_MAX, NET_CONNECTION_MAX);
    registerconfig(ENGINE_CONFIG_SCRIPT_ROOTPATH, SCRIPT_ROOT_PATH_DEFAULT);
    registerconfig(ENGINE_CONFIG_SCRIPT_WORKPATH, SCRIPT_WORK_PATH_DEFAULT);
    registerconfig(ENGINE_CONFIG_SCRIPT_GLOBALFILE, 
                   SCRIPT_LUA_GLOBAL_VAR_FILE_DEFAULT);
    db_manager_ = NULL;
    net_manager_ = NULL;
  __LEAVE_FUNCTION
}

Kernel::~Kernel() {
  __ENTER_FUNCTION
    SAFE_DELETE(net_manager_);
    SAFE_DELETE(db_manager_);
    SAFE_DELETE(g_log);
    SAFE_DELETE(g_time_manager);
  __LEAVE_FUNCTION
}

bool Kernel::init() {
  __ENTER_FUNCTION
    //base
    SLOW_LOG("engine", "[engine] (Kernel::init) start base module");
    if (!init_base()) {
      SLOW_ERRORLOG("engine", 
                    "[engine] (Kernel::init) base module failed");
      return false;
    }
    SLOW_LOG("engine", "[engine] (Kernel::init) base module success");
    //db
    if (getconfig_boolvalue(ENGINE_CONFIG_DB_ISACTIVE)) {
      SLOW_LOG("engine", "[engine] (Kernel::init) start db module");
      if (!init_db()) { 
        SLOW_ERRORLOG("engine", "[engine] (Kernel::init) db module failed");
        return false;
      }
      SLOW_LOG("engine", "[engine] (Kernel::init) db module success");
    }
    //net
    if (getconfig_boolvalue(ENGINE_CONFIG_NET_ISACTIVE)) {
      SLOW_LOG("engine", "[engine] (Kernel::init) start net module");
      if (!init_net()) {
        SLOW_ERRORLOG("engine", "[engine] (Kernel::init) net module failed");
        return false;
      }
      SLOW_LOG("engine", "[engine] (Kernel::init) net module success");
    }
    //script
    if (getconfig_boolvalue(ENGINE_CONFIG_SCRIPT_ISACTIVE)) { 
      SLOW_LOG("engine", "[engine] (Kernel::init) start script module"); 
      if (!init_script()) {
        SLOW_ERRORLOG("engine", "[engine] (Kernel::init) script module failed");
        return false;
      }
      SLOW_LOG("engine", "[engine] (Kernel::init) script module success");
    }
    //performance
    if (getconfig_boolvalue(ENGINE_CONFIG_PERFORMANCE_ISACTIVE)) {
      SLOW_LOG("engine", "[engine] (Kernel::init) start performance module");
      if (!init_performance()) {
        SLOW_ERRORLOG("engine",
                      "[engine] (Kernel::init) performance module failed");
        return false;
      }
      SLOW_LOG("engine", "[engine] (Kernel::init) performance module success"); 
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

void Kernel::run() {
  __ENTER_FUNCTION
    //base
    SLOW_LOG("engine", "[engine] (Kernel::run) base module");
    run_base();
    //db
    if (getconfig_boolvalue(ENGINE_CONFIG_DB_ISACTIVE)) {
      SLOW_LOG("engine", "[engine] (Kernel::run) db module");
      run_db();
    }
    //script
    if (getconfig_boolvalue(ENGINE_CONFIG_SCRIPT_ISACTIVE)) {
      SLOW_LOG("engine", "[engine] (Kernel::run) script module");
      run_script();
    }
    //performance
    if (getconfig_boolvalue(ENGINE_CONFIG_PERFORMANCE_ISACTIVE)) {
      SLOW_LOG("engine", "[engine] (Kernel::run) performance module");
      run_performance();
    }
    //net
    if (getconfig_boolvalue(ENGINE_CONFIG_NET_ISACTIVE)) {
      SLOW_LOG("engine", "[engine] (Kernel::run) net module");
      run_net();
    }
  __LEAVE_FUNCTION
}

void Kernel::stop() {
  __ENTER_FUNCTION
    //performance
    if (getconfig_boolvalue(ENGINE_CONFIG_PERFORMANCE_ISACTIVE)) {
      SLOW_LOG("engine", "[engine] (Kernel::stop) performance module");
      stop_performance();
    }
    //script
    if (getconfig_boolvalue(ENGINE_CONFIG_SCRIPT_ISACTIVE)) {
      SLOW_LOG("engine", "[engine] (Kernel::stop) script module");
      stop_script();
    }
    //net
    if (getconfig_boolvalue(ENGINE_CONFIG_NET_ISACTIVE)) {
      SLOW_LOG("engine", "[engine] (Kernel::stop) net module");
      stop_net();
    }
    //db
    if (getconfig_boolvalue(ENGINE_CONFIG_DB_ISACTIVE)) {
      SLOW_LOG("engine", "[engine] (Kernel::stop) db module");
      stop_db();
    }
    //base
    SLOW_LOG("engine", "[engine] (Kernel::stop) base module");
    stop_base();
  __LEAVE_FUNCTION
}

void Kernel::registerconfig(int32_t key, int32_t value) {
  __ENTER_FUNCTION
    if (config_int32_.isfind(key)) {
      SLOW_WARNINGLOG("engine",
                      "[engine] (Kernel::registerconfig) repeat the key: %d",
                      key);
      return;
    }
    config_int32_.add(key, value);
  __LEAVE_FUNCTION
}

void Kernel::registerconfig(int32_t key, bool value) {
  __ENTER_FUNCTION
    if (config_bool_.isfind(key)) {
      SLOW_WARNINGLOG("engine",
                      "[engine] (Kernel::registerconfig) repeat the key: %d",
                      key);
      return;
    }
    config_bool_.add(key, value);
  __LEAVE_FUNCTION
}

void Kernel::registerconfig(int32_t key, const char *value) {
  __ENTER_FUNCTION
    if (config_string_.isfind(key)) {
      SLOW_WARNINGLOG("engine",
                      "[engine] (Kernel::registerconfig) repeat the key: %d",
                      key);
      return;
    }
    config_string_.add(key, value);
  __LEAVE_FUNCTION
}

bool Kernel::setconfig(int32_t key, int32_t value) {
  __ENTER_FUNCTION
    bool result = config_int32_.set(key, value);
    return result;
  __LEAVE_FUNCTION
    return false;
}

bool Kernel::setconfig(int32_t key, bool value) {
  __ENTER_FUNCTION
    bool result = config_bool_.set(key, value);
    return result;
  __LEAVE_FUNCTION
    return false;
}

bool Kernel::setconfig(int32_t key, const char *value) {
  __ENTER_FUNCTION
    bool result = config_string_.set(key, value);
    return result;
  __LEAVE_FUNCTION
    return false;
}

int32_t Kernel::getconfig_int32value(int32_t key) {
  __ENTER_FUNCTION
    int32_t result = config_int32_.get(key);
    return result;
  __LEAVE_FUNCTION
    return 0;
}
   
bool Kernel::getconfig_boolvalue(int32_t key) {
  __ENTER_FUNCTION
    bool result = config_bool_.get(key);
    return result;
  __LEAVE_FUNCTION
    return false;
}
   
const char *Kernel::getconfig_stringvalue(int32_t key) {
  __ENTER_FUNCTION
    const char *result = NULL;
    result = config_string_.get(key);
    return 0 == strlen(result) ? NULL : result;
  __LEAVE_FUNCTION
    return NULL;
}

void Kernel::set_base_logprint(bool flag) {
  __ENTER_FUNCTION
    ps_common_base::g_command_logprint = flag;
  __LEAVE_FUNCTION
}

void Kernel::set_base_logactive(bool flag) {
  __ENTER_FUNCTION
    ps_common_base::g_command_logactive = flag;
  __LEAVE_FUNCTION
}

bool Kernel::init_base() {
  __ENTER_FUNCTION
    using namespace ps_common_base;
    g_time_manager = new TimeManager();
    if (!TIME_MANAGER_POINTER) return false;
    TIME_MANAGER_POINTER->init();
    g_log = new Log();
    if (!LOGSYSTEM_POINTER) return false;
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool Kernel::init_db() {
  __ENTER_FUNCTION
    using namespace ps_common_db;
    if (getconfig_boolvalue(ENGINE_CONFIG_DB_ISACTIVE)) {
      db_manager_ = 
        new Manager(static_cast<dbconnector_type_t>(
              getconfig_int32value(ENGINE_CONFIG_DB_CONNECTOR_TYPE)));
      if (NULL == db_manager_) return false;
      const char *connection_or_dbname = 
        getconfig_stringvalue(ENGINE_CONFIG_DB_CONNECTION_OR_DBNAME);
      if (NULL == connection_or_dbname) {
        SLOW_ERRORLOG("engine",
                      "[engine] (Kernel::init_db) the connection or db name"
                      " is empty!");
        return false;
      }
      const char *username = getconfig_stringvalue(ENGINE_CONFIG_DB_USERNAME);
      const char *password = getconfig_stringvalue(ENGINE_CONFIG_DB_PASSWORD);
      bool result = db_manager_->init(
          NULL == connection_or_dbname ? "" : connection_or_dbname,
          NULL == username ? "" : username,
          NULL == password ? "" : password);
      return result;
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool Kernel::init_net() {
  __ENTER_FUNCTION
    using namespace ps_common_net;
    if (getconfig_boolvalue(ENGINE_CONFIG_NET_ISACTIVE)) {
      int32_t listenport = getconfig_int32value(ENGINE_CONFIG_NET_LISTEN_PORT);
      net_manager_ = new Manager(static_cast<uint16_t>(listenport));
      if (NULL == net_manager_) return false;
      int32_t connectionmax = 
        getconfig_int32value(ENGINE_CONFIG_NET_CONNECTION_MAX);
      if (connectionmax <= 0) {
        SLOW_ERRORLOG("engine",
                      "[engine] (Kernel::init_net)"
                      " the connection maxcount <= 0");
        return false;
      }
      bool result = net_manager_->init(static_cast<uint16_t>(connectionmax));
      if (result) {
        SLOW_LOG("engine",
                 "[engine] (Kernel::init_net) success!"
                 " connection maxcount: %d, listenport: %d",
                 connectionmax,
                 net_manager_->get_listenport());
      }
      return result;
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool Kernel::init_script() {
  __ENTER_FUNCTION
    using namespace ps_common_script;
    if (getconfig_boolvalue(ENGINE_CONFIG_SCRIPT_ISACTIVE)) {
      if (!SCRIPT_LUASYSTEM_POINTER)
        g_script_luasystem = new lua::System();
      if (NULL == g_script_luasystem) return false;
      SCRIPT_LUASYSTEM_POINTER->set_globalfile(
          getconfig_stringvalue(ENGINE_CONFIG_SCRIPT_GLOBALFILE));
      SCRIPT_LUASYSTEM_POINTER->set_rootpath(
          getconfig_stringvalue(ENGINE_CONFIG_SCRIPT_ROOTPATH));
      SCRIPT_LUASYSTEM_POINTER->set_workpath(
          getconfig_stringvalue(ENGINE_CONFIG_SCRIPT_WORKPATH));
      SCRIPT_LUASYSTEM_POINTER->init();    
      SCRIPT_LUASYSTEM_POINTER->registerfunctions();
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool Kernel::init_performance() {
  __ENTER_FUNCTION
    if (getconfig_boolvalue(ENGINE_CONFIG_PERFORMANCE_ISACTIVE)) {

    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

void Kernel::run_base() {
  //do nothing
}

void Kernel::run_db() {
  __ENTER_FUNCTION
    db_manager_->check_db_connect();
  __LEAVE_FUNCTION
}

void Kernel::run_net() {
  __ENTER_FUNCTION
    net_manager_->loop();
  __LEAVE_FUNCTION
}

void Kernel::run_script() {
  //do nothing
}

void Kernel::run_performance() {
  //do nothing
}

void Kernel::stop_base() {
  __ENTER_FUNCTION
    SAFE_DELETE(g_log);
    SAFE_DELETE(g_time_manager);
  __LEAVE_FUNCTION
}

void Kernel::stop_db() {
  __ENTER_FUNCTION
    SAFE_DELETE(db_manager_);    
  __LEAVE_FUNCTION
}

void Kernel::stop_net() {
  __ENTER_FUNCTION
    net_manager_->setactive(false);
    ps_common_base::util::sleep(5000);
    SAFE_DELETE(net_manager_);
  __LEAVE_FUNCTION
}

void Kernel::stop_script() {
  __ENTER_FUNCTION
    SAFE_DELETE(g_script_luasystem);
  __LEAVE_FUNCTION
}

void Kernel::stop_performance() {
  //do nothing
}

} //namespace ps_common_engine
