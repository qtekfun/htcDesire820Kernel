// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// undesirable.  Here's the above code re-written using Immutable<T>:

#ifndef SYNC_UTIL_IMMUTABLE_H_
#define SYNC_UTIL_IMMUTABLE_H_

#include <algorithm>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

namespace syncer {

namespace internal {

template <typename T, typename Traits>
class ImmutableCore
    : public base::RefCountedThreadSafe<ImmutableCore<T, Traits> > {
 public:
  
  

  ImmutableCore() : wrapper_() {
    Traits::InitializeWrapper(&wrapper_);
  }

  explicit ImmutableCore(T* t) : wrapper_() {
    Traits::InitializeWrapper(&wrapper_);
    Traits::Swap(Traits::UnwrapMutable(&wrapper_), t);
  }

  const T& Get() const {
    return Traits::Unwrap(wrapper_);
  }

 private:
  ~ImmutableCore() {
    Traits::DestroyWrapper(&wrapper_);
  }
  friend class base::RefCountedThreadSafe<ImmutableCore<T, Traits> >;

  
  
  typename Traits::Wrapper wrapper_;

  DISALLOW_COPY_AND_ASSIGN(ImmutableCore);
};

}  


template <typename T>
struct DefaultImmutableTraits {
  typedef T Wrapper;

  static void InitializeWrapper(Wrapper* wrapper) {}

  static void DestroyWrapper(Wrapper* wrapper) {}

  static const T& Unwrap(const Wrapper& wrapper) { return wrapper; }

  static T* UnwrapMutable(Wrapper* wrapper) { return wrapper; }

  static void Swap(T* t1, T* t2) {
    
    
    using std::swap;
    swap(*t1, *t2);
  }
};

template <typename T>
struct HasSwapMemFnByRef : public DefaultImmutableTraits<T> {
  static void Swap(T* t1, T* t2) {
    t1->swap(*t2);
  }
};

template <typename T>
struct HasSwapMemFnByPtr : public DefaultImmutableTraits<T> {
  static void Swap(T* t1, T* t2) {
    t1->Swap(t2);
  }
};

template <typename T, typename Traits = DefaultImmutableTraits<T> >
class Immutable {
 public:
  
  Immutable() : core_(new internal::ImmutableCore<T, Traits>()) {}

  

  
  explicit Immutable(T* t)
      : core_(new internal::ImmutableCore<T, Traits>(t)) {}

  const T& Get() const {
    return core_->Get();
  }

 private:
  scoped_refptr<const internal::ImmutableCore<T, Traits> > core_;
};

template <typename T>
Immutable<T> MakeImmutable(T* t) {
  return Immutable<T>(t);
}

}  

#endif  
