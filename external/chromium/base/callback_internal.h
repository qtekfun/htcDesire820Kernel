// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_CALLBACK_INTERNAL_H_
#define BASE_CALLBACK_INTERNAL_H_
#pragma once

#include <stddef.h>

#include "base/base_api.h"
#include "base/memory/ref_counted.h"

namespace base {
namespace internal {

class InvokerStorageBase : public RefCountedThreadSafe<InvokerStorageBase> {
 protected:
  friend class RefCountedThreadSafe<InvokerStorageBase>;
  virtual ~InvokerStorageBase() {}
};

template <typename T>
struct InvokerStorageHolder {
  explicit InvokerStorageHolder(T* invoker_storage)
      : invoker_storage_(invoker_storage) {
  }

  mutable scoped_refptr<InvokerStorageBase> invoker_storage_;
};

template <typename T>
InvokerStorageHolder<T> MakeInvokerStorageHolder(T* o) {
  return InvokerStorageHolder<T>(o);
}

class BASE_API CallbackBase {
 public:
  
  bool is_null() const;

  
  void Reset();

  bool Equals(const CallbackBase& other) const;

 protected:
  
  
  
  
  typedef void(*InvokeFuncStorage)(void);

  CallbackBase(InvokeFuncStorage polymorphic_invoke,
               scoped_refptr<InvokerStorageBase>* invoker_storage);

  
  
  
  ~CallbackBase();

  scoped_refptr<InvokerStorageBase> invoker_storage_;
  InvokeFuncStorage polymorphic_invoke_;
};

template <typename T>
struct ParamTraits {
  typedef const T& ForwardType;
  typedef T StorageType;
};

template <typename T>
struct ParamTraits<T&> {
  typedef T& ForwardType;
  typedef T StorageType;
};

template <typename T, size_t n>
struct ParamTraits<T[n]> {
  typedef const T* ForwardType;
  typedef const T* StorageType;
};

template <typename T>
struct ParamTraits<T[]> {
  typedef const T* ForwardType;
  typedef const T* StorageType;
};

}  
}  

#endif  
