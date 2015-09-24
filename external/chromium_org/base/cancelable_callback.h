// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_CANCELABLE_CALLBACK_H_
#define BASE_CANCELABLE_CALLBACK_H_

#include "base/base_export.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/callback_internal.h"
#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/memory/weak_ptr.h"

namespace base {

template <typename Sig>
class CancelableCallback;

template <>
class CancelableCallback<void(void)> {
 public:
  CancelableCallback() : weak_factory_(this) {}

  
  explicit CancelableCallback(const base::Callback<void(void)>& callback)
      : weak_factory_(this),
        callback_(callback) {
    DCHECK(!callback.is_null());
    InitializeForwarder();
  }

  ~CancelableCallback() {}

  
  void Cancel() {
    weak_factory_.InvalidateWeakPtrs();
    forwarder_.Reset();
    callback_.Reset();
  }

  
  bool IsCancelled() const {
    return callback_.is_null();
  }

  
  
  void Reset(const base::Callback<void(void)>& callback) {
    DCHECK(!callback.is_null());

    
    Cancel();

    
    InitializeForwarder();

    callback_ = callback;
  }

  
  const base::Callback<void(void)>& callback() const {
    return forwarder_;
  }

 private:
  void Forward() {
    callback_.Run();
  }

  
  
  void InitializeForwarder() {
    forwarder_ = base::Bind(&CancelableCallback<void(void)>::Forward,
                            weak_factory_.GetWeakPtr());
  }

  
  base::WeakPtrFactory<CancelableCallback<void(void)> > weak_factory_;

  
  base::Callback<void(void)> forwarder_;

  
  base::Callback<void(void)> callback_;

  DISALLOW_COPY_AND_ASSIGN(CancelableCallback);
};

template <typename A1>
class CancelableCallback<void(A1)> {
 public:
  CancelableCallback() : weak_factory_(this) {}

  
  explicit CancelableCallback(const base::Callback<void(A1)>& callback)
      : weak_factory_(this),
        callback_(callback) {
    DCHECK(!callback.is_null());
    InitializeForwarder();
  }

  ~CancelableCallback() {}

  
  void Cancel() {
    weak_factory_.InvalidateWeakPtrs();
    forwarder_.Reset();
    callback_.Reset();
  }

  
  bool IsCancelled() const {
    return callback_.is_null();
  }

  
  
  void Reset(const base::Callback<void(A1)>& callback) {
    DCHECK(!callback.is_null());

    
    Cancel();

    
    InitializeForwarder();

    callback_ = callback;
  }

  
  const base::Callback<void(A1)>& callback() const {
    return forwarder_;
  }

 private:
  void Forward(A1 a1) const {
    callback_.Run(a1);
  }

  
  
  void InitializeForwarder() {
    forwarder_ = base::Bind(&CancelableCallback<void(A1)>::Forward,
                            weak_factory_.GetWeakPtr());
  }

  
  base::WeakPtrFactory<CancelableCallback<void(A1)> > weak_factory_;

  
  base::Callback<void(A1)> forwarder_;

  
  base::Callback<void(A1)> callback_;

  DISALLOW_COPY_AND_ASSIGN(CancelableCallback);
};

template <typename A1, typename A2>
class CancelableCallback<void(A1, A2)> {
 public:
  CancelableCallback() : weak_factory_(this) {}

  
  explicit CancelableCallback(const base::Callback<void(A1, A2)>& callback)
      : weak_factory_(this),
        callback_(callback) {
    DCHECK(!callback.is_null());
    InitializeForwarder();
  }

  ~CancelableCallback() {}

  
  void Cancel() {
    weak_factory_.InvalidateWeakPtrs();
    forwarder_.Reset();
    callback_.Reset();
  }

  
  bool IsCancelled() const {
    return callback_.is_null();
  }

  
  
  void Reset(const base::Callback<void(A1, A2)>& callback) {
    DCHECK(!callback.is_null());

    
    Cancel();

    
    InitializeForwarder();

    callback_ = callback;
  }

  
  const base::Callback<void(A1, A2)>& callback() const {
    return forwarder_;
  }

 private:
  void Forward(A1 a1, A2 a2) const {
    callback_.Run(a1, a2);
  }

  
  
  void InitializeForwarder() {
    forwarder_ = base::Bind(&CancelableCallback<void(A1, A2)>::Forward,
                            weak_factory_.GetWeakPtr());
  }

  
  base::WeakPtrFactory<CancelableCallback<void(A1, A2)> > weak_factory_;

  
  base::Callback<void(A1, A2)> forwarder_;

  
  base::Callback<void(A1, A2)> callback_;

  DISALLOW_COPY_AND_ASSIGN(CancelableCallback);
};

typedef CancelableCallback<void(void)> CancelableClosure;

}  

#endif  
