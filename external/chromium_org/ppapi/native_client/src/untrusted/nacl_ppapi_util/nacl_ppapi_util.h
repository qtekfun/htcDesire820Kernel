/*
 * Copyright (c) 2011 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef NATIVE_CLIENT_SRC_UNTRUSTED_NACL_PPAPI_UTIL_NACL_PPAPI_UTIL_H_
#define NATIVE_CLIENT_SRC_UNTRUSTED_NACL_PPAPI_UTIL_NACL_PPAPI_UTIL_H_

#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"

#include "native_client/src/include/nacl_base.h"
#include "native_client/src/include/nacl_scoped_ptr.h"
#include "native_client/src/shared/platform/nacl_sync.h"
#include "native_client/src/shared/platform/nacl_sync_checked.h"
#include "native_client/src/shared/platform/nacl_sync_raii.h"

#include "native_client/src/trusted/weak_ref/weak_ref.h"
#include "ppapi/native_client/src/trusted/weak_ref/call_on_main_thread.h"




namespace nacl_ppapi {

template <typename R> class EventThreadWorkStateWrapper;  

template <typename R>
class EventThreadWorkState {
 public:
  EventThreadWorkState()
      : done_(false),
        result_(NULL) {
    NaClXMutexCtor(&mu_);
    NaClXCondVarCtor(&cv_);
  }

  virtual ~EventThreadWorkState() {
    NaClMutexDtor(&mu_);
    NaClCondVarDtor(&cv_);
  }

  
  
  
  void SetResult(R *result) {
    nacl::MutexLocker take(&mu_);
    result_.reset(result);
  }

  
  R *WaitForCompletion() {
    nacl::MutexLocker take(&mu_);
    while (!done_) {
      NaClXCondVarWait(&cv_, &mu_);
    }
    return result_.release();
  }

 private:
  friend class EventThreadWorkStateWrapper<R>;
  void EventThreadWorkDone() {
    nacl::MutexLocker take(&mu_);
    done_ = true;
    NaClXCondVarBroadcast(&cv_);
  }

  NaClMutex mu_;
  NaClCondVar cv_;
  bool done_;
  nacl::scoped_ptr<R> result_;

  DISALLOW_COPY_AND_ASSIGN(EventThreadWorkState);
};


template <typename R>
class EventThreadWorkStateWrapper {
 public:
  explicit EventThreadWorkStateWrapper(EventThreadWorkState<R> *ws):
      ws_(ws) {}
  virtual ~EventThreadWorkStateWrapper() {
    ws_->EventThreadWorkDone();
  };

  void SetResult(R *result) {
    ws_->SetResult(result);
  }
 private:
  EventThreadWorkState<R> *ws_;

  DISALLOW_COPY_AND_ASSIGN(EventThreadWorkStateWrapper);
};

class VoidResult;

extern VoidResult *const g_void_result;

class VoidResult {
 public:
  VoidResult() {}
  void *operator new(size_t size) { return g_void_result; }
  void operator delete(void *p) {}
 private:
  DISALLOW_COPY_AND_ASSIGN(VoidResult);
};


class NaClPpapiPluginInstance : public pp::Instance {
 public:
  explicit NaClPpapiPluginInstance(PP_Instance instance);
  virtual ~NaClPpapiPluginInstance();
  nacl::WeakRefAnchor* anchor() const { return anchor_; }
 protected:
  nacl::WeakRefAnchor* anchor_;
  DISALLOW_COPY_AND_ASSIGN(NaClPpapiPluginInstance);
};

}  

#endif
