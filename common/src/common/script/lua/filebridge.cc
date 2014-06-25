#include "common/base/util.h"
#include "common/base/string.h"
#include "common/base/log.h"
#include "common/script/lua/vm.h"
#include "common/script/lua/filebridge.h"

namespace ps_common_script {

namespace lua {

FileBridge::FileBridge() {
  __ENTER_FUNCTION
    fp_ = NULL;
    length_ = 0;
    position_ = 0;
  __LEAVE_FUNCTION
}

FileBridge::~FileBridge() {
  __ENTER_FUNCTION
    close();
  __LEAVE_FUNCTION
}

bool FileBridge::open(const char* filename) {
  __ENTER_FUNCTION
    using namespace ps_common_base;
    if (fp_) close();
    char filepath[FILENAME_MAX] = {0};
    VM::get_fullpath(filepath, filename);
#if __WINDOWS__
    char* temppointer = filepath;
    util::path_tounix(temppointer, sizeof(filepath) - 1);
    char casepath[FILENAME_MAX] = {0};
    char rootpath[FILENAME_MAX] = {0};
    VM::read_rootpath(rootpath);
    string::safecopy(casepath, filepath, sizeof(casepath) - 1);
    fp_ = fopen(casepath, "rb");
#endif
    fp_ = fp_ ? fp_ : fopen(filepath, "rb");
    if (NULL == fp_) {
      SLOW_ERRORLOG("error", 
                    "[script][lua] FileBridge::open file error: %s", 
                    filepath);  
      return false;
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

void FileBridge::close() {
  __ENTER_FUNCTION
    if (fp_) fclose(fp_);
    fp_ = NULL;
    length_ = 0;
    position_ = 0;
  __LEAVE_FUNCTION
}

uint64_t FileBridge::read(void* buffer, uint64_t read_bytes) {
  __ENTER_FUNCTION
    if (!fp_) return 0;
    uint64_t _read_bytes;
    _read_bytes = fread(buffer, 1, read_bytes, fp_);  
    position_ += _read_bytes;
    return _read_bytes;
  __LEAVE_FUNCTION
    return 0;
}

uint64_t FileBridge::write(void* buffer, uint64_t write_bytes) {
  __ENTER_FUNCTION
    if (!fp_) return 0;
    uint64_t _write_bytes;
    _write_bytes = fwrite(buffer, 1, write_bytes, fp_); 
    position_ += _write_bytes;
    return _write_bytes;
  __LEAVE_FUNCTION
    return 0;
}

int64_t FileBridge::seek(int64_t position, file_accessmode accessmode) {
  __ENTER_FUNCTION
    if (!fp_) return -1;
    fseek(fp_, position_, accessmode);
    position_ = ftell(fp_);
    return position_;
  __LEAVE_FUNCTION
    return -1;
}

int64_t FileBridge::tell() {
  __ENTER_FUNCTION
    if (!fp_) return -1;
    return position_;
  __LEAVE_FUNCTION
    return -1;
}

uint64_t FileBridge::size() {
  __ENTER_FUNCTION
    if (!fp_) return 0;
    if (0 == length_) {
      uint64_t temp = position_;
      length_ = static_cast<uint64_t>(seek(0, kFileAccessModeEnd));
      seek(temp, kFileAccessModeBegin);
    }
    return length_;
  __LEAVE_FUNCTION
    return 0;
}

} //namespace lua

} //namespace ps_common_script
