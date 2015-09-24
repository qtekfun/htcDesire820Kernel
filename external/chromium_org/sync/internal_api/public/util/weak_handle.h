// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef SYNC_UTIL_WEAK_HANDLE_H_
#define SYNC_UTIL_WEAK_HANDLE_H_

#include <cstddef>

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/location.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "sync/base/sync_export.h"

namespace base {
class MessageLoopProxy;
}  

namespace tracked_objects {
class Location;
}  

namespace syncer {

template <typename T> class WeakHandle;

namespace internal {


template <typename T>
struct ParamTraits {
  typedef const T& ForwardType;
};

template <typename T>
struct ParamTraits<T&> {
  typedef T& ForwardType;
};

template <typename T, size_t n>
struct ParamTraits<T[n]> {
  typedef const T* ForwardType;
};

template <typename T>
struct ParamTraits<T[]> {
  typedef const T* ForwardType;
};

class SYNC_EXPORT WeakHandleCoreBase {
 public:
  
  WeakHandleCoreBase();

  
  bool IsOnOwnerThread() const;

 protected:
  
  ~WeakHandleCoreBase();

  
  void PostToOwnerThread(const tracked_objects::Location& from_here,
                         const base::Closure& fn) const;

 private:
  
  const scoped_refptr<base::MessageLoopProxy> owner_loop_proxy_;

  DISALLOW_COPY_AND_ASSIGN(WeakHandleCoreBase);
};

template <typename T>
class WeakHandleCore
    : public WeakHandleCoreBase,
      public base::RefCountedThreadSafe<WeakHandleCore<T> > {
 public:
  
  
  explicit WeakHandleCore(const base::WeakPtr<T>& ptr) : ptr_(ptr) {}

  
  base::WeakPtr<T> Get() const {
    CHECK(IsOnOwnerThread());
    return ptr_;
  }

  
  

  template <typename U>
  void Call(const tracked_objects::Location& from_here,
            void (U::*fn)(void)) const {
    PostToOwnerThread(
        from_here,
        Bind(&WeakHandleCore::template DoCall0<U>, this, fn));
  }

  template <typename U, typename A1>
  void Call(const tracked_objects::Location& from_here,
            void (U::*fn)(A1),
            typename ParamTraits<A1>::ForwardType a1) const {
    PostToOwnerThread(
        from_here,
        Bind(&WeakHandleCore::template DoCall1<U, A1>,
             this, fn, a1));
  }

  template <typename U, typename A1, typename A2>
  void Call(const tracked_objects::Location& from_here,
            void (U::*fn)(A1, A2),
            typename ParamTraits<A1>::ForwardType a1,
            typename ParamTraits<A2>::ForwardType a2) const {
    PostToOwnerThread(
        from_here,
        Bind(&WeakHandleCore::template DoCall2<U, A1, A2>,
             this, fn, a1, a2));
  }

  template <typename U, typename A1, typename A2, typename A3>
  void Call(const tracked_objects::Location& from_here,
            void (U::*fn)(A1, A2, A3),
            typename ParamTraits<A1>::ForwardType a1,
            typename ParamTraits<A2>::ForwardType a2,
            typename ParamTraits<A3>::ForwardType a3) const {
    PostToOwnerThread(
        from_here,
        Bind(&WeakHandleCore::template DoCall3<U, A1, A2, A3>,
             this, fn, a1, a2, a3));
  }

  template <typename U, typename A1, typename A2, typename A3, typename A4>
  void Call(const tracked_objects::Location& from_here,
            void (U::*fn)(A1, A2, A3, A4),
            typename ParamTraits<A1>::ForwardType a1,
            typename ParamTraits<A2>::ForwardType a2,
            typename ParamTraits<A3>::ForwardType a3,
            typename ParamTraits<A4>::ForwardType a4) const {
    PostToOwnerThread(
        from_here,
        Bind(&WeakHandleCore::template DoCall4<U, A1, A2, A3, A4>,
             this, fn, a1, a2, a3, a4));
  }

 private:
  friend class base::RefCountedThreadSafe<WeakHandleCore<T> >;

  
  ~WeakHandleCore() {}

  
  

  template <typename U>
  void DoCall0(void (U::*fn)(void)) const {
    CHECK(IsOnOwnerThread());
    if (!Get()) {
      return;
    }
    (Get().get()->*fn)();
  }

  template <typename U, typename A1>
  void DoCall1(void (U::*fn)(A1),
               typename ParamTraits<A1>::ForwardType a1) const {
    CHECK(IsOnOwnerThread());
    if (!Get()) {
      return;
    }
    (Get().get()->*fn)(a1);
  }

  template <typename U, typename A1, typename A2>
  void DoCall2(void (U::*fn)(A1, A2),
               typename ParamTraits<A1>::ForwardType a1,
               typename ParamTraits<A2>::ForwardType a2) const {
    CHECK(IsOnOwnerThread());
    if (!Get()) {
      return;
    }
    (Get().get()->*fn)(a1, a2);
  }

  template <typename U, typename A1, typename A2, typename A3>
  void DoCall3(void (U::*fn)(A1, A2, A3),
               typename ParamTraits<A1>::ForwardType a1,
               typename ParamTraits<A2>::ForwardType a2,
               typename ParamTraits<A3>::ForwardType a3) const {
    CHECK(IsOnOwnerThread());
    if (!Get()) {
      return;
    }
    (Get().get()->*fn)(a1, a2, a3);
  }

  template <typename U, typename A1, typename A2, typename A3, typename A4>
  void DoCall4(void (U::*fn)(A1, A2, A3, A4),
               typename ParamTraits<A1>::ForwardType a1,
               typename ParamTraits<A2>::ForwardType a2,
               typename ParamTraits<A3>::ForwardType a3,
               typename ParamTraits<A4>::ForwardType a4) const {
    CHECK(IsOnOwnerThread());
    if (!Get()) {
      return;
    }
    (Get().get()->*fn)(a1, a2, a3, a4);
  }

  
  
  base::WeakPtr<T> ptr_;

  DISALLOW_COPY_AND_ASSIGN(WeakHandleCore);
};

}  

template <typename T>
class WeakHandle {
 public:
  
  WeakHandle() {}

  
  explicit WeakHandle(const base::WeakPtr<T>& ptr)
      : core_(new internal::WeakHandleCore<T>(ptr)) {}

  
  
  
  
  template <typename U>
  WeakHandle(const WeakHandle<U>& other)  
      : core_(
          other.IsInitialized() ?
          new internal::WeakHandleCore<T>(other.Get()) :
          NULL) {}

  
  
  
  bool IsInitialized() const {
    return core_.get() != NULL;
  }

  
  void Reset() {
    core_ = NULL;
  }

  
  base::WeakPtr<T> Get() const {
    CHECK(IsInitialized());
    CHECK(core_->IsOnOwnerThread());
    return core_->Get();
  }

  
  

  template <typename U>
  void Call(const tracked_objects::Location& from_here,
            void (U::*fn)(void)) const {
    CHECK(IsInitialized());
    core_->Call(from_here, fn);
  }

  template <typename U, typename A1>
  void Call(const tracked_objects::Location& from_here,
            void (U::*fn)(A1),
            typename internal::ParamTraits<A1>::ForwardType a1) const {
    CHECK(IsInitialized());
    core_->Call(from_here, fn, a1);
  }

  template <typename U, typename A1, typename A2>
  void Call(const tracked_objects::Location& from_here,
            void (U::*fn)(A1, A2),
            typename internal::ParamTraits<A1>::ForwardType a1,
            typename internal::ParamTraits<A2>::ForwardType a2) const {
    CHECK(IsInitialized());
    core_->Call(from_here, fn, a1, a2);
  }

  template <typename U, typename A1, typename A2, typename A3>
  void Call(const tracked_objects::Location& from_here,
            void (U::*fn)(A1, A2, A3),
            typename internal::ParamTraits<A1>::ForwardType a1,
            typename internal::ParamTraits<A2>::ForwardType a2,
            typename internal::ParamTraits<A3>::ForwardType a3) const {
    CHECK(IsInitialized());
    core_->Call(from_here, fn, a1, a2, a3);
  }

  template <typename U, typename A1, typename A2, typename A3, typename A4>
  void Call(const tracked_objects::Location& from_here,
            void (U::*fn)(A1, A2, A3, A4),
            typename internal::ParamTraits<A1>::ForwardType a1,
            typename internal::ParamTraits<A2>::ForwardType a2,
            typename internal::ParamTraits<A3>::ForwardType a3,
            typename internal::ParamTraits<A4>::ForwardType a4) const {
    CHECK(IsInitialized());
    core_->Call(from_here, fn, a1, a2, a3, a4);
  }

 private:
  FRIEND_TEST_ALL_PREFIXES(WeakHandleTest,
                           TypeConversionConstructor);
  FRIEND_TEST_ALL_PREFIXES(WeakHandleTest,
                           TypeConversionConstructorAssignment);

  scoped_refptr<internal::WeakHandleCore<T> > core_;
};

template <typename T>
WeakHandle<T> MakeWeakHandle(const base::WeakPtr<T>& ptr) {
  return WeakHandle<T>(ptr);
}

}  

#endif  
