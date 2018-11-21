// Copyright (c) 2015 The Baidu Authors.
// All rights reserved.


#ifndef BASE_THREAD_THREAD_DESTRUCTION_OBSERVER_H_
#define BASE_THREAD_THREAD_DESTRUCTION_OBSERVER_H_

#include "base/base_export.h"

namespace base {
class BASE_EXPORT DestructionObserver {
 public:
  virtual ~DestructionObserver() {}
  virtual void WillDestroyCurrentMessageLoop() = 0;
};
}

#endif   // BASE_THREAD_THREAD_DESTRUCTION_OBSERVER_H_
