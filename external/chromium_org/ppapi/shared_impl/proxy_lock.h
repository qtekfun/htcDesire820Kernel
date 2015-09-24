// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_PROXY_LOCK_H_
#define PPAPI_SHARED_IMPL_PROXY_LOCK_H_

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/threading/thread_checker.h"

#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace base {
class Lock;
}

namespace content {
class HostGlobals;
}

namespace ppapi {

class PPAPI_SHARED_EXPORT ProxyLock {
 public:
  
  
  
  
  static base::Lock* Get();

  
  
  
  
  
  static void Acquire();
  
  static void Release();

  
  
  static void AssertAcquired();
  static void AssertAcquiredDebugOnly() {
#ifndef NDEBUG
    AssertAcquired();
#endif
  }

  
  
  
  static void DisableLockingOnThreadForTest();

  
  
  
  static void EnableLockingOnThreadForTest();

 private:
  friend class content::HostGlobals;
  
  
  
  static void DisableLocking();

  DISALLOW_IMPLICIT_CONSTRUCTORS(ProxyLock);
};

class ProxyAutoLock {
 public:
  ProxyAutoLock() {
    ProxyLock::Acquire();
  }
  ~ProxyAutoLock() {
    ProxyLock::Release();
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(ProxyAutoLock);
};

class ProxyAutoUnlock {
 public:
  ProxyAutoUnlock() {
    ProxyLock::Release();
  }
  ~ProxyAutoUnlock() {
    ProxyLock::Acquire();
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(ProxyAutoUnlock);
};

template <class ReturnType>
ReturnType CallWhileUnlocked(ReturnType (*function)()) {
  ProxyAutoUnlock unlock;
  return function();
}
template <class ReturnType, class P1>
ReturnType CallWhileUnlocked(ReturnType (*function)(P1), const P1& p1) {
  ProxyAutoUnlock unlock;
  return function(p1);
}
template <class ReturnType, class P1, class P2>
ReturnType CallWhileUnlocked(ReturnType (*function)(P1, P2),
                             const P1& p1,
                             const P2& p2) {
  ProxyAutoUnlock unlock;
  return function(p1, p2);
}
template <class ReturnType, class P1, class P2, class P3>
ReturnType CallWhileUnlocked(ReturnType (*function)(P1, P2, P3),
                             const P1& p1,
                             const P2& p2,
                             const P3& p3) {
  ProxyAutoUnlock unlock;
  return function(p1, p2, p3);
}
template <class ReturnType, class P1, class P2, class P3, class P4>
ReturnType CallWhileUnlocked(ReturnType (*function)(P1, P2, P3, P4),
                             const P1& p1,
                             const P2& p2,
                             const P3& p3,
                             const P4& p4) {
  ProxyAutoUnlock unlock;
  return function(p1, p2, p3, p4);
}
template <class ReturnType, class P1, class P2, class P3, class P4, class P5>
ReturnType CallWhileUnlocked(ReturnType (*function)(P1, P2, P3, P4, P5),
                             const P1& p1,
                             const P2& p2,
                             const P3& p3,
                             const P4& p4,
                             const P5& p5) {
  ProxyAutoUnlock unlock;
  return function(p1, p2, p3, p4, p5);
}
void PPAPI_SHARED_EXPORT CallWhileUnlocked(const base::Closure& closure);

namespace internal {

template <typename RunType>
class RunWhileLockedHelper;

template <>
class RunWhileLockedHelper<void ()> {
 public:
  typedef base::Callback<void ()> CallbackType;
  explicit RunWhileLockedHelper(const CallbackType& callback)
      : callback_(new CallbackType(callback)) {
    
    
    ProxyLock::AssertAcquired();
    
    
    thread_checker_.DetachFromThread();
  }
  void CallWhileLocked() {
    
    DCHECK(thread_checker_.CalledOnValidThread());
    ProxyAutoLock lock;
    {
      
      
      
      scoped_ptr<CallbackType> temp_callback(callback_.Pass());
      temp_callback->Run();
    }
  }

  ~RunWhileLockedHelper() {
    
    
    DCHECK(thread_checker_.CalledOnValidThread());
    
    
    
    
    
    
    
    
    
    
    
    
    if (callback_) {
      
      
      
      
      
      
      
      
      ProxyAutoLock lock;
      callback_.reset();
    }
  }
 private:
  scoped_ptr<CallbackType> callback_;

  
  base::ThreadChecker thread_checker_;
};

template <typename P1>
class RunWhileLockedHelper<void (P1)> {
 public:
  typedef base::Callback<void (P1)> CallbackType;
  explicit RunWhileLockedHelper(const CallbackType& callback)
      : callback_(new CallbackType(callback)) {
    ProxyLock::AssertAcquired();
    thread_checker_.DetachFromThread();
  }
  void CallWhileLocked(P1 p1) {
    DCHECK(thread_checker_.CalledOnValidThread());
    ProxyAutoLock lock;
    {
      scoped_ptr<CallbackType> temp_callback(callback_.Pass());
      temp_callback->Run(p1);
    }
  }
  ~RunWhileLockedHelper() {
    DCHECK(thread_checker_.CalledOnValidThread());
    if (callback_) {
      ProxyAutoLock lock;
      callback_.reset();
    }
  }
 private:
  scoped_ptr<CallbackType> callback_;
  base::ThreadChecker thread_checker_;
};

template <typename P1, typename P2>
class RunWhileLockedHelper<void (P1, P2)> {
 public:
  typedef base::Callback<void (P1, P2)> CallbackType;
  explicit RunWhileLockedHelper(const CallbackType& callback)
      : callback_(new CallbackType(callback)) {
    ProxyLock::AssertAcquired();
    thread_checker_.DetachFromThread();
  }
  void CallWhileLocked(P1 p1, P2 p2) {
    DCHECK(thread_checker_.CalledOnValidThread());
    ProxyAutoLock lock;
    {
      scoped_ptr<CallbackType> temp_callback(callback_.Pass());
      temp_callback->Run(p1, p2);
    }
  }
  ~RunWhileLockedHelper() {
    DCHECK(thread_checker_.CalledOnValidThread());
    if (callback_) {
      ProxyAutoLock lock;
      callback_.reset();
    }
  }
 private:
  scoped_ptr<CallbackType> callback_;
  base::ThreadChecker thread_checker_;
};

template <typename P1, typename P2, typename P3>
class RunWhileLockedHelper<void (P1, P2, P3)> {
 public:
  typedef base::Callback<void (P1, P2, P3)> CallbackType;
  explicit RunWhileLockedHelper(const CallbackType& callback)
      : callback_(new CallbackType(callback)) {
    ProxyLock::AssertAcquired();
    thread_checker_.DetachFromThread();
  }
  void CallWhileLocked(P1 p1, P2 p2, P3 p3) {
    DCHECK(thread_checker_.CalledOnValidThread());
    ProxyAutoLock lock;
    {
      scoped_ptr<CallbackType> temp_callback(callback_.Pass());
      temp_callback->Run(p1, p2, p3);
    }
  }
  ~RunWhileLockedHelper() {
    DCHECK(thread_checker_.CalledOnValidThread());
    if (callback_) {
      ProxyAutoLock lock;
      callback_.reset();
    }
  }
 private:
  scoped_ptr<CallbackType> callback_;
  base::ThreadChecker thread_checker_;
};

}  

template <class FunctionType>
inline base::Callback<FunctionType>
RunWhileLocked(const base::Callback<FunctionType>& callback) {
  internal::RunWhileLockedHelper<FunctionType>* helper =
      new internal::RunWhileLockedHelper<FunctionType>(callback);
  return base::Bind(
      &internal::RunWhileLockedHelper<FunctionType>::CallWhileLocked,
      base::Owned(helper));
}

}  

#endif  
