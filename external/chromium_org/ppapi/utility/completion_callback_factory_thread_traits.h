// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_UTILITY_THREAD_SAFE_THREAD_TRAITS_H_
#define PPAPI_UTILITY_THREAD_SAFE_THREAD_TRAITS_H_

#include "ppapi/cpp/logging.h"
#include "ppapi/cpp/module.h"
#include "ppapi/utility/threading/lock.h"


namespace pp {

class ThreadSafeThreadTraits {
 public:
  class RefCount {
   public:
    
    
    RefCount() : ref_(0) {
    }

    
    
    
    int32_t AddRef() {
      AutoLock lock(lock_);
      return ++ref_;
    }

    
    
    
    int32_t Release() {
      AutoLock lock(lock_);
      PP_DCHECK(ref_ > 0);
      return --ref_;
    }

   private:
    Lock lock_;
    int32_t ref_;
  };

  typedef pp::Lock Lock;
  typedef pp::AutoLock AutoLock;
};

class NonThreadSafeThreadTraits {
 public:
  
  
  
  
  class RefCount {
   public:
    
    
    RefCount() : ref_(0) {
#ifndef NDEBUG
      is_main_thread_ = Module::Get()->core()->IsMainThread();
#endif
    }

    
    ~RefCount() {
      PP_DCHECK(is_main_thread_ == Module::Get()->core()->IsMainThread());
    }

    
    
    
    int32_t AddRef() {
      PP_DCHECK(is_main_thread_ == Module::Get()->core()->IsMainThread());
      return ++ref_;
    }

    
    
    
    int32_t Release() {
      PP_DCHECK(is_main_thread_ == Module::Get()->core()->IsMainThread());
      return --ref_;
    }

   private:
    int32_t ref_;
#ifndef NDEBUG
    bool is_main_thread_;
#endif
  };

  
  
  
  
  
  class Lock {
   public:
    Lock() {
#ifndef NDEBUG
      is_main_thread_ = Module::Get()->core()->IsMainThread();
      lock_held_ = false;
#endif
    }

    ~Lock() {
      PP_DCHECK(is_main_thread_ == Module::Get()->core()->IsMainThread());
    }

    
    
    void Acquire() {
#ifndef NDEBUG
      PP_DCHECK(!lock_held_);
      lock_held_ = true;
#endif
    }

    
    
    void Release() {
#ifndef NDEBUG
      PP_DCHECK(lock_held_);
      lock_held_ = false;
#endif
    }

   private:
#ifndef NDEBUG
    bool is_main_thread_;
    bool lock_held_;
#endif
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
  };
};

}  

#endif  
