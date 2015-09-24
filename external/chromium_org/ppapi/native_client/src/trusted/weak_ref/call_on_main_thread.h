/*
 * Copyright (c) 2011 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef NATIVE_CLIENT_SRC_TRUSTED_WEAK_REF_CALL_ON_MAIN_THREAD_H_
#define NATIVE_CLIENT_SRC_TRUSTED_WEAK_REF_CALL_ON_MAIN_THREAD_H_

#include "native_client/src/trusted/weak_ref/weak_ref.h"

#include "native_client/src/include/nacl_scoped_ptr.h"
#include "native_client/src/include/nacl_compiler_annotations.h"
#include "native_client/src/include/portability.h"

#include "ppapi/c/pp_errors.h"  
#include "ppapi/cpp/completion_callback.h"  
#include "ppapi/cpp/core.h"  
#include "ppapi/cpp/module.h"  

namespace plugin {


static char const* const kPpWeakRefModuleName = "pp_weak_ref";

template <typename R> pp::CompletionCallback WeakRefNewCallback(
    nacl::WeakRefAnchor* anchor,
    void callback_fn(nacl::WeakRef<R>* weak_data, int32_t err),
    R* raw_data) {
  nacl::WeakRef<R>* wp = anchor->MakeWeakRef<R>(raw_data);
  
  
  pp::CompletionCallback cc_nrvo(
      reinterpret_cast<void (*)(void*, int32_t)>(
          callback_fn),
      reinterpret_cast<void*>(wp));
  return cc_nrvo;
}

template <typename R> void WeakRefCallOnMainThread(
    nacl::WeakRefAnchor* anchor,
    int32_t delay_in_milliseconds,
    void callback_fn(nacl::WeakRef<R>* weak_data, int32_t err),
    R* raw_data) {
  pp::CompletionCallback cc =
      WeakRefNewCallback(anchor, callback_fn, raw_data, &cc);

  pp::Module::Get()->core()->CallOnMainThread(
      delay_in_milliseconds,
      cc,
      PP_OK);
}

template <typename R> class WeakRefAutoAbandonWrapper {
 public:
  WeakRefAutoAbandonWrapper(void (*callback_fn)(R* raw_data,
                                                int32_t err),
                            R* raw_data)
      : orig_callback_fn(callback_fn),
        orig_data(raw_data) {}

  void (*orig_callback_fn)(R* raw_data, int32_t err);
  nacl::scoped_ptr<R> orig_data;
};

template <typename R> void WeakRefAutoAbandoner(
    nacl::WeakRef<WeakRefAutoAbandonWrapper<R> >* wr,
    int32_t err) {
  nacl::scoped_ptr<WeakRefAutoAbandonWrapper<R> > p;
  wr->ReleaseAndUnref(&p);
  if (p == NULL) {
    NaClLog2(kPpWeakRefModuleName, 4,
             "WeakRefAutoAbandoner: weak ref NULL, anchor was abandoned\n");
    return;
  }
  NaClLog2(kPpWeakRefModuleName, 4,
           "WeakRefAutoAbandoner: weak ref okay, invoking callback\n");
  (*p->orig_callback_fn)(p->orig_data.get(), err);
  return;
}

template <typename R> pp::CompletionCallback WeakRefNewCallback(
    nacl::WeakRefAnchor* anchor,
    void (*raw_callback_fn)(R* raw_data, int32_t err),
    R* raw_data) {

  WeakRefAutoAbandonWrapper<R>* wref_auto_wrapper =
      new WeakRefAutoAbandonWrapper<R>(raw_callback_fn, raw_data);

  CHECK(wref_auto_wrapper != NULL);

  nacl::WeakRef<WeakRefAutoAbandonWrapper<R> >* wp =
      anchor->MakeWeakRef<WeakRefAutoAbandonWrapper<R> >(
          wref_auto_wrapper);
  void (*weak_ref_auto_abandoner_ptr)(
      nacl::WeakRef<WeakRefAutoAbandonWrapper<R> >* wr,
      int32_t err) = WeakRefAutoAbandoner<R>;
  
  pp::CompletionCallback cc_nrvo(
      reinterpret_cast<void (*)(void*, int32_t)>(weak_ref_auto_abandoner_ptr),
      reinterpret_cast<void*>(wp));
  return cc_nrvo;
}

template <typename R> void WeakRefCallOnMainThread(
    nacl::WeakRefAnchor* anchor,
    int32_t delay_in_milliseconds,
    void raw_callback_fn(R* raw_data, int32_t err),
    R* raw_data) {
  pp::CompletionCallback cc =
      WeakRefNewCallback(anchor, raw_callback_fn, raw_data, &cc);
  pp::Module::Get()->core()->CallOnMainThread(
      delay_in_milliseconds,
      cc,
      PP_OK);
}


template <typename R, typename E>
class WeakRefMemberFuncBinder {
 public:
  WeakRefMemberFuncBinder(E* object,
                          void (E::*raw_callback_fn)(R* raw_data,
                                                     int32_t err),
                          R* raw_data)
      : object_(object),
        raw_callback_fn_(raw_callback_fn),
        data_(raw_data) {}
  void Invoke(int32_t err) {
    NaClLog2(kPpWeakRefModuleName, 4,
             ("WeakRefMemberFuncBinder: Invoke obj 0x%" NACL_PRIxPTR
              ", err%" NACL_PRId32 "\n"),
             reinterpret_cast<uintptr_t>(object_), err);
    (object_->*raw_callback_fn_)(data_.get(), err);
    NaClLog2(kPpWeakRefModuleName, 4,
             "WeakRefMemberFuncBinder: done\n");
  }
 private:
  E* object_;
  void (E::*raw_callback_fn_)(R* raw_data, int32_t err);
  nacl::scoped_ptr<R> data_;
};

template <typename R, typename E>
void WeakRefMemberFuncInvoker(
    WeakRefMemberFuncBinder<R, E> *binder, int32_t err) {
  NaClLog2(kPpWeakRefModuleName, 4,
           "WeakRefMemberFuncInvoker: %" NACL_PRIxPTR " %" NACL_PRId32 "\n",
           (uintptr_t) binder,
           err);
  binder->Invoke(err);
  
  
}


template <typename R, typename E>
pp::CompletionCallback WeakRefNewCallback(
    nacl::WeakRefAnchor* anchor,
    E* object,
    void (E::*raw_callback_fn)(R* raw_data, int32_t err),
    R* raw_data) {
  NaClLog2(kPpWeakRefModuleName, 4,
           "Entered WeakRefNewCallback\n");
  NaClLog2(kPpWeakRefModuleName, 4,
           "object 0x%" NACL_PRIxPTR "\n",
           reinterpret_cast<uintptr_t>(object));
  WeakRefMemberFuncBinder<R, E>* binder =
      new WeakRefMemberFuncBinder<R, E>(object,
                                        raw_callback_fn,
                                        raw_data);
  CHECK(binder != NULL);
  NaClLog2(kPpWeakRefModuleName, 4,
           "WeakRefNewCallback: binder %" NACL_PRIxPTR "\n",
           (uintptr_t) binder);
  void (*weak_ref_member_func_invoker_ptr)(
      WeakRefMemberFuncBinder<R, E>* binder,
      int32_t err) = WeakRefMemberFuncInvoker<R, E>;
  return WeakRefNewCallback(anchor, weak_ref_member_func_invoker_ptr,
                             binder);
}

template <typename R, typename E> void WeakRefCallOnMainThread(
    nacl::WeakRefAnchor* anchor,
    int32_t delay_in_milliseconds,
    E* object,
    void (E::*raw_callback_fn)(R* raw_data, int32_t err),
    R* raw_data) {
  NaClLog2(kPpWeakRefModuleName, 4,
           "Entered WeakRefCallOnMainThread\n");
  pp::CompletionCallback cc =
      WeakRefNewCallback(anchor, object, raw_callback_fn, raw_data);
  NaClLog2(kPpWeakRefModuleName, 4,
           "WeakRefCallOnMainThread: got cc\n");
  pp::Module::Get()->core()->CallOnMainThread(
      delay_in_milliseconds,
      cc,
      PP_OK);
  NaClLog2(kPpWeakRefModuleName, 4,
           "WeakRefCallOnMainThread: invoked PP_CallOnMainThread\n");
}

}  

#endif
