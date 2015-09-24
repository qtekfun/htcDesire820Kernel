


// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_CALLBACK_H_
#define BASE_CALLBACK_H_
#pragma once

#include "base/callback_internal.h"
#include "base/callback_old.h"


namespace base {

template <typename Sig>
class Callback;

template <typename R>
class Callback<R(void)> : public internal::CallbackBase {
 public:
  typedef R(*PolymorphicInvoke)(
      internal::InvokerStorageBase*);

  Callback() : CallbackBase(NULL, NULL) { }

  
  
  
  
  
  
  
  template <typename T>
  Callback(const internal::InvokerStorageHolder<T>& invoker_holder)
      : CallbackBase(
          reinterpret_cast<InvokeFuncStorage>(&T::Invoker::DoInvoke),
          &invoker_holder.invoker_storage_) {
  }

  R Run() const {
    PolymorphicInvoke f =
        reinterpret_cast<PolymorphicInvoke>(polymorphic_invoke_);

    return f(invoker_storage_.get());
  }
};

template <typename R, typename A1>
class Callback<R(A1)> : public internal::CallbackBase {
 public:
  typedef R(*PolymorphicInvoke)(
      internal::InvokerStorageBase*,
          typename internal::ParamTraits<A1>::ForwardType);

  Callback() : CallbackBase(NULL, NULL) { }

  
  
  
  
  
  
  
  template <typename T>
  Callback(const internal::InvokerStorageHolder<T>& invoker_holder)
      : CallbackBase(
          reinterpret_cast<InvokeFuncStorage>(&T::Invoker::DoInvoke),
          &invoker_holder.invoker_storage_) {
  }

  R Run(typename internal::ParamTraits<A1>::ForwardType a1) const {
    PolymorphicInvoke f =
        reinterpret_cast<PolymorphicInvoke>(polymorphic_invoke_);

    return f(invoker_storage_.get(), a1);
  }
};

template <typename R, typename A1, typename A2>
class Callback<R(A1, A2)> : public internal::CallbackBase {
 public:
  typedef R(*PolymorphicInvoke)(
      internal::InvokerStorageBase*,
          typename internal::ParamTraits<A1>::ForwardType,
          typename internal::ParamTraits<A2>::ForwardType);

  Callback() : CallbackBase(NULL, NULL) { }

  
  
  
  
  
  
  
  template <typename T>
  Callback(const internal::InvokerStorageHolder<T>& invoker_holder)
      : CallbackBase(
          reinterpret_cast<InvokeFuncStorage>(&T::Invoker::DoInvoke),
          &invoker_holder.invoker_storage_) {
  }

  R Run(typename internal::ParamTraits<A1>::ForwardType a1,
        typename internal::ParamTraits<A2>::ForwardType a2) const {
    PolymorphicInvoke f =
        reinterpret_cast<PolymorphicInvoke>(polymorphic_invoke_);

    return f(invoker_storage_.get(), a1,
             a2);
  }
};

template <typename R, typename A1, typename A2, typename A3>
class Callback<R(A1, A2, A3)> : public internal::CallbackBase {
 public:
  typedef R(*PolymorphicInvoke)(
      internal::InvokerStorageBase*,
          typename internal::ParamTraits<A1>::ForwardType,
          typename internal::ParamTraits<A2>::ForwardType,
          typename internal::ParamTraits<A3>::ForwardType);

  Callback() : CallbackBase(NULL, NULL) { }

  
  
  
  
  
  
  
  template <typename T>
  Callback(const internal::InvokerStorageHolder<T>& invoker_holder)
      : CallbackBase(
          reinterpret_cast<InvokeFuncStorage>(&T::Invoker::DoInvoke),
          &invoker_holder.invoker_storage_) {
  }

  R Run(typename internal::ParamTraits<A1>::ForwardType a1,
        typename internal::ParamTraits<A2>::ForwardType a2,
        typename internal::ParamTraits<A3>::ForwardType a3) const {
    PolymorphicInvoke f =
        reinterpret_cast<PolymorphicInvoke>(polymorphic_invoke_);

    return f(invoker_storage_.get(), a1,
             a2,
             a3);
  }
};

template <typename R, typename A1, typename A2, typename A3, typename A4>
class Callback<R(A1, A2, A3, A4)> : public internal::CallbackBase {
 public:
  typedef R(*PolymorphicInvoke)(
      internal::InvokerStorageBase*,
          typename internal::ParamTraits<A1>::ForwardType,
          typename internal::ParamTraits<A2>::ForwardType,
          typename internal::ParamTraits<A3>::ForwardType,
          typename internal::ParamTraits<A4>::ForwardType);

  Callback() : CallbackBase(NULL, NULL) { }

  
  
  
  
  
  
  
  template <typename T>
  Callback(const internal::InvokerStorageHolder<T>& invoker_holder)
      : CallbackBase(
          reinterpret_cast<InvokeFuncStorage>(&T::Invoker::DoInvoke),
          &invoker_holder.invoker_storage_) {
  }

  R Run(typename internal::ParamTraits<A1>::ForwardType a1,
        typename internal::ParamTraits<A2>::ForwardType a2,
        typename internal::ParamTraits<A3>::ForwardType a3,
        typename internal::ParamTraits<A4>::ForwardType a4) const {
    PolymorphicInvoke f =
        reinterpret_cast<PolymorphicInvoke>(polymorphic_invoke_);

    return f(invoker_storage_.get(), a1,
             a2,
             a3,
             a4);
  }
};

template <typename R, typename A1, typename A2, typename A3, typename A4,
    typename A5>
class Callback<R(A1, A2, A3, A4, A5)> : public internal::CallbackBase {
 public:
  typedef R(*PolymorphicInvoke)(
      internal::InvokerStorageBase*,
          typename internal::ParamTraits<A1>::ForwardType,
          typename internal::ParamTraits<A2>::ForwardType,
          typename internal::ParamTraits<A3>::ForwardType,
          typename internal::ParamTraits<A4>::ForwardType,
          typename internal::ParamTraits<A5>::ForwardType);

  Callback() : CallbackBase(NULL, NULL) { }

  
  
  
  
  
  
  
  template <typename T>
  Callback(const internal::InvokerStorageHolder<T>& invoker_holder)
      : CallbackBase(
          reinterpret_cast<InvokeFuncStorage>(&T::Invoker::DoInvoke),
          &invoker_holder.invoker_storage_) {
  }

  R Run(typename internal::ParamTraits<A1>::ForwardType a1,
        typename internal::ParamTraits<A2>::ForwardType a2,
        typename internal::ParamTraits<A3>::ForwardType a3,
        typename internal::ParamTraits<A4>::ForwardType a4,
        typename internal::ParamTraits<A5>::ForwardType a5) const {
    PolymorphicInvoke f =
        reinterpret_cast<PolymorphicInvoke>(polymorphic_invoke_);

    return f(invoker_storage_.get(), a1,
             a2,
             a3,
             a4,
             a5);
  }
};

template <typename R, typename A1, typename A2, typename A3, typename A4,
    typename A5, typename A6>
class Callback<R(A1, A2, A3, A4, A5, A6)> : public internal::CallbackBase {
 public:
  typedef R(*PolymorphicInvoke)(
      internal::InvokerStorageBase*,
          typename internal::ParamTraits<A1>::ForwardType,
          typename internal::ParamTraits<A2>::ForwardType,
          typename internal::ParamTraits<A3>::ForwardType,
          typename internal::ParamTraits<A4>::ForwardType,
          typename internal::ParamTraits<A5>::ForwardType,
          typename internal::ParamTraits<A6>::ForwardType);

  Callback() : CallbackBase(NULL, NULL) { }

  
  
  
  
  
  
  
  template <typename T>
  Callback(const internal::InvokerStorageHolder<T>& invoker_holder)
      : CallbackBase(
          reinterpret_cast<InvokeFuncStorage>(&T::Invoker::DoInvoke),
          &invoker_holder.invoker_storage_) {
  }

  R Run(typename internal::ParamTraits<A1>::ForwardType a1,
        typename internal::ParamTraits<A2>::ForwardType a2,
        typename internal::ParamTraits<A3>::ForwardType a3,
        typename internal::ParamTraits<A4>::ForwardType a4,
        typename internal::ParamTraits<A5>::ForwardType a5,
        typename internal::ParamTraits<A6>::ForwardType a6) const {
    PolymorphicInvoke f =
        reinterpret_cast<PolymorphicInvoke>(polymorphic_invoke_);

    return f(invoker_storage_.get(), a1,
             a2,
             a3,
             a4,
             a5,
             a6);
  }
};


typedef Callback<void(void)> Closure;

}  

#endif  
