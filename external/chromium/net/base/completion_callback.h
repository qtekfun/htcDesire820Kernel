// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_COMPLETION_CALLBACK_H__
#define NET_BASE_COMPLETION_CALLBACK_H__
#pragma once

#include "base/callback.h"

namespace net {

typedef Callback1<int>::Type CompletionCallback;

template <class T>
class CompletionCallbackImpl :
    public CallbackImpl< T, void (T::*)(int), Tuple1<int> > {
 public:
  CompletionCallbackImpl(T* obj, void (T::* meth)(int))
    : CallbackImpl< T, void (T::*)(int),
                    Tuple1<int> >::CallbackImpl(obj, meth) {
  }
};

template <class T>
class CancelableCompletionCallback :
    public CompletionCallbackImpl<T>,
    public base::RefCounted<CancelableCompletionCallback<T> > {
 public:
  CancelableCompletionCallback(T* obj, void (T::* meth)(int))
    : CompletionCallbackImpl<T>(obj, meth), is_canceled_(false) {
  }

  void Cancel() {
    is_canceled_ = true;
  }

  virtual void RunWithParams(const Tuple1<int>& params) {
    if (is_canceled_) {
      base::RefCounted<CancelableCompletionCallback<T> >::Release();
    } else {
      CompletionCallbackImpl<T>::RunWithParams(params);
    }
  }

 private:
  bool is_canceled_;
};

}  

#endif  
