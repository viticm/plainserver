/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id filebridge.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/25 21:32
 * @uses script lua module file bridge class
 */
#ifndef PS_COMMON_SCRIPT_LUA_FILEBRIDGE_H_
#define PS_COMMON_SCRIPT_LUA_FILEBRIDGE_H_

#include "common/script/lua/config.h"

namespace ps_common_script {

namespace lua {

class FileBridge {

 public:
   FileBridge(const char* rootpath = NULL, const char* workpath = NULL);
   ~FileBridge();

 public:
   enum file_accessmode {
     kFileAccessModeBegin = 0,
     kFileAccessModeCurrent,
     kFileAccessModeEnd
   };

 public:
   void set_rootpath(const char* path);
   void set_workpath(const char* path);
   void get_fullpath(char* path, const char* filename, size_t length);
   const char *get_rootpath();
   
 public:
   bool open(const char*);
   void close();
   uint64_t read(void*, uint64_t);
   uint64_t write(void*, uint64_t);
   int64_t seek(int64_t, file_accessmode);
   int64_t tell();
   uint64_t size();

 private:
   FILE* fp_;
   uint64_t length_;
   int64_t position_;
   char rootpath_[FILENAME_MAX];
   char workpath_[FILENAME_MAX];

};

}; //namespace lua

}; //namespace ps_common_script

#endif //PS_COMMON_SCRIPT_LUA_FILEBRIDGE_H_
