// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_UTILITY_THREADING_LOCK_H_
#define PPAPI_UTILITY_THREADING_LOCK_H_

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

namespace pp {

class Lock {
 public:
  
  Lock();

  
  ~Lock();

  
  
  
  
  
  
  void Acquire();

  
  void Release();

 private:
#if defined(WIN32)
  typedef CRITICAL_SECTION OSLockType;
#else
  typedef pthread_mutex_t OSLockType;
#endif

  OSLockType os_lock_;

  
  Lock(const Lock&);
  Lock& operator=(const Lock&);
};

class AutoLock {
 public:
  explicit AutoLock(Lock& lock) : lock_(lock) {
    lock_.Acquire();
  }

  ~AutoLock() {
    lock_.Release();
  }

 private:
  Lock& lock_;

  
  AutoLock(const AutoLock&);
  AutoLock& operator=(const AutoLock&);
};

}  

#endif  
