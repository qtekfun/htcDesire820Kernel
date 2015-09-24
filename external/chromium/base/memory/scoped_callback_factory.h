// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_MEMORY_SCOPED_CALLBACK_FACTORY_H_
#define BASE_MEMORY_SCOPED_CALLBACK_FACTORY_H_

#include "base/callback.h"
#include "base/memory/weak_ptr.h"

namespace base {

template <class T>
class ScopedCallbackFactory {
 public:
  explicit ScopedCallbackFactory(T* obj) : weak_factory_(obj) {
  }

  typename Callback0::Type* NewCallback(
      void (T::*method)()) {
    return new CallbackImpl<void (T::*)(), Tuple0 >(
        weak_factory_.GetWeakPtr(), method);
  }

  template <typename Arg1>
  typename Callback1<Arg1>::Type* NewCallback(
      void (T::*method)(Arg1)) {
    return new CallbackImpl<void (T::*)(Arg1), Tuple1<Arg1> >(
        weak_factory_.GetWeakPtr(), method);
  }

  template <typename Arg1, typename Arg2>
  typename Callback2<Arg1, Arg2>::Type* NewCallback(
      void (T::*method)(Arg1, Arg2)) {
    return new CallbackImpl<void (T::*)(Arg1, Arg2), Tuple2<Arg1, Arg2> >(
        weak_factory_.GetWeakPtr(), method);
  }

  template <typename Arg1, typename Arg2, typename Arg3>
  typename Callback3<Arg1, Arg2, Arg3>::Type* NewCallback(
      void (T::*method)(Arg1, Arg2, Arg3)) {
    return new CallbackImpl<void (T::*)(Arg1, Arg2, Arg3),
                            Tuple3<Arg1, Arg2, Arg3> >(
        weak_factory_.GetWeakPtr(), method);
  }

  template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
  typename Callback4<Arg1, Arg2, Arg3, Arg4>::Type* NewCallback(
      void (T::*method)(Arg1, Arg2, Arg3, Arg4)) {
    return new CallbackImpl<void (T::*)(Arg1, Arg2, Arg3, Arg4),
                            Tuple4<Arg1, Arg2, Arg3, Arg4> >(
        weak_factory_.GetWeakPtr(), method);
  }

  template <typename Arg1, typename Arg2, typename Arg3, typename Arg4,
            typename Arg5>
  typename Callback5<Arg1, Arg2, Arg3, Arg4, Arg5>::Type* NewCallback(
      void (T::*method)(Arg1, Arg2, Arg3, Arg4, Arg5)) {
    return new CallbackImpl<void (T::*)(Arg1, Arg2, Arg3, Arg4, Arg5),
                            Tuple5<Arg1, Arg2, Arg3, Arg4, Arg5> >(
        weak_factory_.GetWeakPtr(), method);
  }

  void RevokeAll() { weak_factory_.InvalidateWeakPtrs(); }
  bool HasPendingCallbacks() const { return weak_factory_.HasWeakPtrs(); }

 private:
  template <typename Method>
  class CallbackStorage {
   public:
    CallbackStorage(const WeakPtr<T>& obj, Method meth)
        : obj_(obj),
          meth_(meth) {
    }

   protected:
    WeakPtr<T> obj_;
    Method meth_;
  };

  template <typename Method, typename Params>
  class CallbackImpl : public CallbackStorage<Method>,
                       public CallbackRunner<Params> {
   public:
    CallbackImpl(const WeakPtr<T>& obj, Method meth)
        : CallbackStorage<Method>(obj, meth) {
    }
    virtual void RunWithParams(const Params& params) {
      
      
      if (!this->obj_)
        return;
      DispatchToMethod(this->obj_.get(), this->meth_, params);
    }
  };

  WeakPtrFactory<T> weak_factory_;
};

}  

#endif  
