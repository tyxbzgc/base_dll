// Copyright (c) 2012 The Chromium Authors.
// All rights reserved.


#ifndef BASE_THREAD_THREAD_CHECKER_H_
#define BASE_THREAD_THREAD_CHECKER_H_

#include "base/lock.h"

namespace base  {
class ThreadChecker {
 public:
  ThreadChecker()
    : valid_thread_id_(-1) {
    EnsureThreadIdAssigned();
  }

  ~ThreadChecker() {}

  bool CalledOnValidThread() const {
    EnsureThreadIdAssigned();
    AutoLock auto_lock(lock_);
    return valid_thread_id_ == ::GetCurrentThreadId();
  }

  // Changes the thread that is checked for in CalledOnValidThread.  This may
  // be useful when an object may be created on one thread and then used
  // exclusively on another thread.
  void DetachFromThread() {
    AutoLock auto_lock(lock_);
    valid_thread_id_ = ::GetCurrentThreadId();
  }

 private:
  void EnsureThreadIdAssigned() const {
    AutoLock auto_lock(lock_);
    if (-1 == valid_thread_id_) {
      valid_thread_id_ = ::GetCurrentThreadId();
    }
  }

  mutable base::Lock lock_;
  // This is mutable so that CalledOnValidThread can set it.
  // It's guarded by |lock_|.
  mutable ThreadId valid_thread_id_;
};

}  // namespace base

#endif  // BASE_THREAD_THREAD_CHECKER_H_
