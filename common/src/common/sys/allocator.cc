#include "common/sys/allocator.h"
#include "common/base/log.h"

namespace ps_common_sys {

Allocator::Allocator() {
  __ENTER_FUNCTION
    buffer_ = NULL;
    size_ = 0;
    offset_ = 0;
  __LEAVE_FUNCTION
}

Allocator::~Allocator() {
  //do nothing
}

void Allocator::init(char* buffer, size_t size) {
  __ENTER_FUNCTION
    buffer_ = buffer;
    size_ = size;
  __LEAVE_FUNCTION
}

void* Allocator::malloc(size_t size) {
  __ENTER_FUNCTION
    if (this->offset_ + size > this->size_) {
      ERRORPRINTF("[sys]Allocator::malloc: out of memory allocating %d bytes",
                  size);
      Assert(false);
      return NULL;
    }
    char* pointer = &this->buffer_[this->offset_]; 
    this->offset_ += size;
    return reinterpret_cast<void*>(pointer);
  __LEAVE_FUNCTION
    return NULL;
}

}; //namespace ps_common_sys
