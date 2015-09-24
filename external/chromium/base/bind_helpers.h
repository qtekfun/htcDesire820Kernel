// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_BIND_HELPERS_H_
#define BASE_BIND_HELPERS_H_
#pragma once

#include "base/basictypes.h"
#include "base/template_util.h"

namespace base {
namespace internal {

template <typename T>
class SupportsAddRefAndRelease {
  typedef char Yes[1];
  typedef char No[2];

  struct BaseMixin {
    void AddRef();
    void Release();
  };

#if defined(OS_WIN)
#pragma warning(disable:4624)
#endif
  struct Base : public T, public BaseMixin {
  };
#if defined(OS_WIN)
#pragma warning(default:4624)
#endif

  template <void(BaseMixin::*)(void)>  struct Helper {};

  template <typename C>
  static No& Check(Helper<&C::AddRef>*, Helper<&C::Release>*);

  template <typename >
  static Yes& Check(...);

 public:
  static const bool value = sizeof(Check<Base>(0,0)) == sizeof(Yes);
};


template <bool IsClasstype, typename T>
struct UnsafeBindtoRefCountedArgHelper : false_type {
};

template <typename T>
struct UnsafeBindtoRefCountedArgHelper<true, T>
    : integral_constant<bool, SupportsAddRefAndRelease<T>::value> {
};

template <typename T>
struct UnsafeBindtoRefCountedArg : false_type {
};

template <typename T>
struct UnsafeBindtoRefCountedArg<T*>
    : UnsafeBindtoRefCountedArgHelper<is_class<T>::value, T> {
};


template <typename T>
class UnretainedWrapper {
 public:
  explicit UnretainedWrapper(T* o) : obj_(o) {}
  T* get() { return obj_; }
 private:
  T* obj_;
};

template <typename T>
class ConstRefWrapper {
 public:
  explicit ConstRefWrapper(const T& o) : ptr_(&o) {}
  const T& get() { return *ptr_; }
 private:
  const T* ptr_;
};


template <typename T>
T Unwrap(T o) { return o; }

template <typename T>
T* Unwrap(UnretainedWrapper<T> unretained) { return unretained.get(); }

template <typename T>
const T& Unwrap(ConstRefWrapper<T> const_ref) {
  return const_ref.get();
}


template <typename ref, typename T>
struct MaybeRefcount;

template <typename T>
struct MaybeRefcount<base::false_type, T> {
  static void AddRef(const T&) {}
  static void Release(const T&) {}
};

template <typename T, size_t n>
struct MaybeRefcount<base::false_type, T[n]> {
  static void AddRef(const T*) {}
  static void Release(const T*) {}
};

template <typename T>
struct MaybeRefcount<base::true_type, UnretainedWrapper<T> > {
  static void AddRef(const UnretainedWrapper<T>&) {}
  static void Release(const UnretainedWrapper<T>&) {}
};

template <typename T>
struct MaybeRefcount<base::true_type, T*> {
  static void AddRef(T* o) { o->AddRef(); }
  static void Release(T* o) { o->Release(); }
};

template <typename T>
struct MaybeRefcount<base::true_type, const T*> {
  static void AddRef(const T* o) { o->AddRef(); }
  static void Release(const T* o) { o->Release(); }
};

}  

template <typename T>
inline internal::UnretainedWrapper<T> Unretained(T* o) {
  return internal::UnretainedWrapper<T>(o);
}

template <typename T>
inline internal::ConstRefWrapper<T> ConstRef(const T& o) {
  return internal::ConstRefWrapper<T>(o);
}

}  

#endif  
