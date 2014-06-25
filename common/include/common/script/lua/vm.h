/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id vm.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/25 22:28
 * @uses script lua module Virtual Manufacturing
 */
#ifndef PS_COMMON_SCRIPT_LUA_VM_H_
#define PS_COMMON_SCRIPT_LUA_VM_H_

#include "common/script/lua/config.h"
#include <lua.hpp>

namespace ps_common_script {

namespace lua {

class VM {

 public:
   VM();
   ~VM();

 public:
   typedef enum {
     kErrorCodeCreate = 1,
     kErrorCodeLength = 2,
     kErrorCodeCompile = 3,
     kErrorCodeExecute = 4,
     kErrorCodeNotNumber = 5,
     kErrorCodeNotString = 6,
     kErrorCodeNotTable = 7,
     kErrorCodeStateIsNil = 8,
   } errorcode_t;

 public:
   bool init(int32_t stacksize = 0);
   void release();
   bool register_function(const char *name, void *function);
   bool load(const char *filename);
   bool loadbuffer(unsigned char *buffer, uint64_t length);

 public:
   void run_scriptfunction_enter(int32_t *index);
   bool run_scriptfunction(const char *, int32_t); //0
   bool run_scriptfunction(const char *, int32_t, int64_t); //1
   bool run_scriptfunction(const char *, int32_t, int64_t, int64_t); //2
   //3
   bool run_scriptfunction(const char *, int32_t, int64_t, int64_t, int64_t);
   bool run_scriptfunction(const char *,
                           int32_t, 
                           int64_t, 
                           int64_t, 
                           int64_t, 
                           int64_t); //4
   bool run_scriptfunction(const char *,
                           int32_t,
                           int64_t,
                           int64_t,
                           int64_t,
                           int64_t,
                           int64_t); //5
   bool run_scriptfunction(const char *,
                           int32_t,
                           int64_t,
                           int64_t,
                           int64_t,
                           int64_t,
                           int64_t,
                           int64_t); //6
  bool run_scriptfunction(const char *,
                          int32_t,
                          int64_t,
                          int64_t,
                          int64_t,
                          int64_t,
                          int64_t,
                          int64_t,
                          int64_t); //7
  bool run_scriptfunction(const char *,
                          int32_t,
                          int64_t,
                          int64_t,
                          int64_t,
                          int64_t,
                          int64_t,
                          int64_t,
                          int64_t,
                          int64_t); //8
  bool run_scriptfunction(const char *,
                          int32_t,
                          int64_t,
                          int64_t,
                          float,
                          float); //9
  bool run_scriptfunction(const char *,
                          int32_t,
                          int64_t,
                          int64_t,
                          const char *,
                          const char *); //10
  void run_scriptfunction_leave(int32_t index);

 public:
   static void read_rootpath(char* path);
   static void get_fullpath(char* path, const char* filename);

 private:
   bool executecode();
   void on_init();
   void on_scripterror(int32_t error);
   void on_scripterror(int32_t, int32_t);

 private:
   lua_State *lua_state_;

};

}; //namespace lua

}; //namespace ps_common_script

#endif //PS_COMMON_SCRIPT_LUA_VM_H_
