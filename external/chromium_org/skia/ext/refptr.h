// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_REFPTR_H_
#define SKIA_EXT_REFPTR_H_

#include "third_party/skia/include/core/SkRefCnt.h"

namespace skia {

template<typename T>
class RefPtr {
 public:
  RefPtr() : ptr_(NULL) {}

  RefPtr(const RefPtr& other)
      : ptr_(other.get()) {
    SkSafeRef(ptr_);
  }

  template<typename U>
  RefPtr(const RefPtr<U>& other)
      : ptr_(other.get()) {
    SkSafeRef(ptr_);
  }

  ~RefPtr() {
    clear();
  }

  RefPtr& operator=(const RefPtr& other) {
    SkRefCnt_SafeAssign(ptr_, other.get());
    return *this;
  }

  template<typename U>
  RefPtr& operator=(const RefPtr<U>& other) {
    SkRefCnt_SafeAssign(ptr_, other.get());
    return *this;
  }

  void clear() {
    T* to_unref = ptr_;
    ptr_ = NULL;
    SkSafeUnref(to_unref);
  }

  T* get() const { return ptr_; }
  T& operator*() const { return *ptr_; }
  T* operator->() const { return ptr_; }

  typedef T* RefPtr::*unspecified_bool_type;
  operator unspecified_bool_type() const {
    return ptr_ ? &RefPtr::ptr_ : NULL;
  }

 private:
  T* ptr_;

  
  
  
  
  
  explicit RefPtr(T* ptr) : ptr_(ptr) {}

  template<typename U>
  friend RefPtr<U> AdoptRef(U* ptr);

  template<typename U>
  friend RefPtr<U> SharePtr(U* ptr);
};

template<typename T>
RefPtr<T> AdoptRef(T* ptr) { return RefPtr<T>(ptr); }

template<typename T>
RefPtr<T> SharePtr(T* ptr) { return RefPtr<T>(SkSafeRef(ptr)); }

}  

#endif  
