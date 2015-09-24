// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_MEMORY_WEAK_PTR_H_
#define BASE_MEMORY_WEAK_PTR_H_
#pragma once

#include "base/base_api.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/threading/thread_checker.h"

namespace base {

namespace internal {

class BASE_API WeakReference {
 public:
  
  
  class Flag : public RefCountedThreadSafe<Flag> {
   public:
    explicit Flag(Flag** handle);

    void Invalidate();
    bool IsValid() const;

    void DetachFromThread() { thread_checker_.DetachFromThread(); }

   private:
    friend class base::RefCountedThreadSafe<Flag>;

    ~Flag();

    ThreadChecker thread_checker_;
    Flag** handle_;
  };

  WeakReference();
  WeakReference(Flag* flag);
  ~WeakReference();

  bool is_valid() const;

 private:
  scoped_refptr<Flag> flag_;
};

class BASE_API WeakReferenceOwner {
 public:
  WeakReferenceOwner();
  ~WeakReferenceOwner();

  WeakReference GetRef() const;

  bool HasRefs() const {
    return flag_ != NULL;
  }

  void Invalidate();

  
  void DetachFromThread() {
    if (flag_) flag_->DetachFromThread();
  }

 private:
  mutable WeakReference::Flag* flag_;
};

class BASE_API WeakPtrBase {
 public:
  WeakPtrBase();
  ~WeakPtrBase();

 protected:
  WeakPtrBase(const WeakReference& ref);

  WeakReference ref_;
};

}  

template <typename T> class SupportsWeakPtr;
template <typename T> class WeakPtrFactory;

template <typename T>
class WeakPtr : public internal::WeakPtrBase {
 public:
  WeakPtr() : ptr_(NULL) {
  }

  
  template <typename U>
  WeakPtr(const WeakPtr<U>& other) : WeakPtrBase(other), ptr_(other.get()) {
  }

  T* get() const { return ref_.is_valid() ? ptr_ : NULL; }
  operator T*() const { return get(); }

  T* operator*() const {
    DCHECK(get() != NULL);
    return *get();
  }
  T* operator->() const {
    DCHECK(get() != NULL);
    return get();
  }

  void reset() {
    ref_ = internal::WeakReference();
    ptr_ = NULL;
  }

 private:
  friend class SupportsWeakPtr<T>;
  friend class WeakPtrFactory<T>;

  WeakPtr(const internal::WeakReference& ref, T* ptr)
      : WeakPtrBase(ref), ptr_(ptr) {
  }

  
  
  T* ptr_;
};

template <class T>
class SupportsWeakPtr {
 public:
  SupportsWeakPtr() {}

  WeakPtr<T> AsWeakPtr() {
    return WeakPtr<T>(weak_reference_owner_.GetRef(), static_cast<T*>(this));
  }

  
  void DetachFromThread() {
    weak_reference_owner_.DetachFromThread();
  }

 private:
  internal::WeakReferenceOwner weak_reference_owner_;
  DISALLOW_COPY_AND_ASSIGN(SupportsWeakPtr);
};

template <class T>
class WeakPtrFactory {
 public:
  explicit WeakPtrFactory(T* ptr) : ptr_(ptr) {
  }

  WeakPtr<T> GetWeakPtr() {
    return WeakPtr<T>(weak_reference_owner_.GetRef(), ptr_);
  }

  
  void InvalidateWeakPtrs() {
    weak_reference_owner_.Invalidate();
  }

  
  bool HasWeakPtrs() const {
    return weak_reference_owner_.HasRefs();
  }

  
  void DetachFromThread() {
    weak_reference_owner_.DetachFromThread();
  }

 private:
  internal::WeakReferenceOwner weak_reference_owner_;
  T* ptr_;
  DISALLOW_IMPLICIT_CONSTRUCTORS(WeakPtrFactory);
};

}  

#endif  
