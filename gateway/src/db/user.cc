#include "engine/system.h"
#include "db/user.h"

using namespace db::user;

user_t get_fullinfo(const char *name) {
  user_t user;
  __ENTER_FUNCTION
    if (!ENGINE_SYSTEM_POINTER || !ENGINE_SYSTEM_POINTER->get_dbmanager())
      return user;
    enum {
      kDBId = 1,
      kDBName,
      kDBPassword,
      kDBTureName,
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

  __LEAVE_FUNCTION
    return user;
}
