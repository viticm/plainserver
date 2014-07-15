#include "engine/system.h"
#include "db/user.h"

namespace db {

namespace user {

user_t get_fullinfo(const char *name) {
  user_t user;
  __ENTER_FUNCTION
    if (!ENGINE_SYSTEM_POINTER || !ENGINE_SYSTEM_POINTER->get_dbmanager())
      return user;
    enum {
      kDBId = 1,
      kDBName,
      kDBPassword,
      kDBTrueName,
      kDBBirthday,
      kDBSuperPassword,
      kDBMoney,
      kDBTodayLoginTimes,
      kDBLocked,
      kDBVipLevel,
    };
    const char *kSqlStr = "SELECT `id`, `name`, `truename`, `birthday`,"
                          " `password2`, `money`, `today_logintims`, `locked`,"
                          " `viplevel` FROM "DB_TABLE_USER""
                          " WHERE `name` = '%s'";
    ENGINE_SYSTEM_POINTER
      ->get_dbmanager()
      ->get_internal_query()
      ->parse(kSqlStr, name);
    bool query_result = ENGINE_SYSTEM_POINTER->get_dbmanager()->query();
    if (query_result && ENGINE_SYSTEM_POINTER->get_dbmanager()->fetch()) {
      int32_t error_code = 0;
      user.id = 
        ENGINE_SYSTEM_POINTER->get_dbmanager()->get_int32(kDBId, error_code);
      ENGINE_SYSTEM_POINTER
        ->get_dbmanager()
        ->get_string(kDBName, user.name, sizeof(user.name) - 1, error_code);
      ENGINE_SYSTEM_POINTER
        ->get_dbmanager()
        ->get_string(kDBPassword, 
                     user.password, 
                     sizeof(user.password) - 1, 
                     error_code);
      ENGINE_SYSTEM_POINTER
        ->get_dbmanager()
        ->get_string(kDBTrueName, 
                     user.truename, 
                     sizeof(user.truename) - 1, 
                     error_code);
      ENGINE_SYSTEM_POINTER
        ->get_dbmanager()
        ->get_string(kDBBirthday,
                     user.birthday,
                     sizeof(user.birthday) - 1,
                     error_code);
      ENGINE_SYSTEM_POINTER
        ->get_dbmanager()
        ->get_string(kDBSuperPassword,
                     user.superpassword,
                     sizeof(user.superpassword) - 1,
                     error_code);
      user.money = ENGINE_SYSTEM_POINTER
                   ->get_dbmanager()
                   ->get_uint32(kDBMoney, error_code);

      user.today_logintimes = ENGINE_SYSTEM_POINTER
                              ->get_dbmanager()
                              ->get_uint32(kDBTodayLoginTimes, error_code);
      uint8_t locked = ENGINE_SYSTEM_POINTER
                       ->get_dbmanager()
                       ->get_uint8(kDBLocked, error_code);
      user.locked = 0 == locked ? false : true;
      user.viplevel = ENGINE_SYSTEM_POINTER
                      ->get_dbmanager()
                      ->get_uint8(kDBVipLevel, error_code);
    }
    return user;
  __LEAVE_FUNCTION
    return user;
}

} //namespace user

} //namespace db