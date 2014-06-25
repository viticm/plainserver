#include "common/sys/memory/dynamic_allocator.h"

namespace ps_common_sys {

namespace memory {

DynamicAllocator::DynamicAllocator() {
  __ENTER_FUNCTION
    pointer_ = NULL;
    length_ = 0;
  __LEAVE_FUNCTION
}

DynamicAllocator::~DynamicAllocator() {
  __ENTER_FUNCTION
    free();
  __LEAVE_FUNCTION
}

void* DynamicAllocator::malloc(int64_t length) {
  __ENTER_FUNCTION
    if (length_ == length) return pointer_;
    if (pointer_) free();
    pointer_ = reinterpret_cast<void*>(new char[length]);
    if (pointer_ != NULL) length_ = length;
    return pointer_;
  __LEAVE_FUNCTION
    return NULL;
}

void DynamicAllocator::free() {
  __ENTER_FUNCTION
    char* pointer = reinterpret_cast<char*>(pointer_);
    SAFE_DELETE_ARRAY(pointer);
    length_ = 0;
  __LEAVE_FUNCTION
}

}; //namespace memory

}; //namespace ps_common_sys
