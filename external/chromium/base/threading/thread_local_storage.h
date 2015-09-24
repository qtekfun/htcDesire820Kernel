// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_THREAD_LOCAL_STORAGE_H_
#define BASE_THREADING_THREAD_LOCAL_STORAGE_H_
#pragma once

#include "base/base_api.h"
#include "base/basictypes.h"

#if defined(OS_POSIX)
#include <pthread.h>
#endif

namespace base {

class BASE_API ThreadLocalStorage {
 public:

  
  
  
  typedef void (*TLSDestructorFunc)(void* value);

  
  class BASE_API Slot {
   public:
    explicit Slot(TLSDestructorFunc destructor = NULL);

    
    
    explicit Slot(base::LinkerInitialized x) {}

    
    
    
    
    bool Initialize(TLSDestructorFunc destructor);

    
    
    
    
    void Free();

    
    
    void* Get() const;

    
    
    void Set(void* value);

    bool initialized() const { return initialized_; }

   private:
    
    bool initialized_;
#if defined(OS_WIN)
    int slot_;
#elif defined(OS_POSIX)
    pthread_key_t key_;
#endif

    DISALLOW_COPY_AND_ASSIGN(Slot);
  };

#if defined(OS_WIN)
  
  
  static void ThreadExit();

 private:
  
  static void **Initialize();

 private:
  
  
  
  static const int kThreadLocalStorageSize = 64;

  static long tls_key_;
  static long tls_max_;
  static TLSDestructorFunc tls_destructors_[kThreadLocalStorageSize];
#endif  

  DISALLOW_COPY_AND_ASSIGN(ThreadLocalStorage);
};

}  

#endif  
