// Copyright (c) 2015 The Baidu Authors.
// All rights reserved.


#ifndef BASE_THREAD_THREAD_LOCAL_POINTER_H_
#define BASE_THREAD_THREAD_LOCAL_POINTER_H_

#include "base/platform.h"

namespace base {
template<class Type>
class ThreadLocalPointer {
 public:
  ThreadLocalPointer()
    : slot_(0) {
    slot_ = TlsAlloc();
  }

  ~ThreadLocalPointer() {
    TlsFree(slot_);
  }

  Type * Get() {
    return static_cast<Type*>(TlsGetValue(slot_));
  }

  VOID Set(Type * ptr) {
    TlsSetValue(slot_, ptr);
  }

 private:
  DWORD slot_;
};
}  // namespace base

#endif  // BASE_THREAD_THREAD_LOCAL_POINTER_H_
