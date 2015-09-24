// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_COMPLETION_CALLBACK_H_
#define PPAPI_CPP_COMPLETION_CALLBACK_H_

#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_errors.h"
#include "ppapi/cpp/extensions/ext_output_traits.h"
#include "ppapi/cpp/logging.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/output_traits.h"

namespace pp {

class CompletionCallback {
 public:
  
  
  
  
  
  
  
  CompletionCallback() {
    cc_ = PP_BlockUntilComplete();
  }

  
  
  
  
  
  
  CompletionCallback(PP_CompletionCallback_Func func, void* user_data) {
    cc_ = PP_MakeCompletionCallback(func, user_data);
  }

  
  
  
  
  
  
  
  
  
  
  CompletionCallback(PP_CompletionCallback_Func func, void* user_data,
                     int32_t flags) {
    cc_ = PP_MakeCompletionCallback(func, user_data);
    cc_.flags = flags;
  }

  
  
  
  
  
  
  void set_flags(int32_t flags) { cc_.flags = flags; }

  
  
  
  
  
  
  
  
  
  
  
  void Run(int32_t result) {
    PP_DCHECK(cc_.func);
    PP_RunCompletionCallback(&cc_, result);
  }

  
  
  
  
  
  
  
  
  void RunAndClear(int32_t result) {
    PP_DCHECK(cc_.func);
    PP_RunAndClearCompletionCallback(&cc_, result);
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  bool IsOptional() const {
    return (cc_.func == NULL ||
            (cc_.flags & PP_COMPLETIONCALLBACK_FLAG_OPTIONAL) != 0);
  }

  
  
  
  
  const PP_CompletionCallback& pp_completion_callback() const { return cc_; }

  
  
  
  
  
  int32_t flags() const { return cc_.flags; }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t MayForce(int32_t result) const {
    if (result == PP_OK_COMPLETIONPENDING || IsOptional())
      return result;
    
    Module::Get()->core()->CallOnMainThread(0, *this, result);
    return PP_OK_COMPLETIONPENDING;
  }

 protected:
  PP_CompletionCallback cc_;
};

namespace internal {

template <typename T, typename Traits>
class CompletionCallbackWithOutputBase : public CompletionCallback {
 public:
  typedef typename Traits::StorageType OutputStorageType;
  typedef typename Traits::APIArgType APIArgType;
  typedef Traits TraitsType;

  explicit CompletionCallbackWithOutputBase(OutputStorageType* output)
      : CompletionCallback(),
        output_(output) {
  }

  CompletionCallbackWithOutputBase(PP_CompletionCallback_Func func,
                                   void* user_data,
                                   OutputStorageType* output)
      : CompletionCallback(func, user_data),
        output_(output) {
  }

  CompletionCallbackWithOutputBase(PP_CompletionCallback_Func func,
                                   void* user_data,
                                   int32_t flags,
                                   OutputStorageType* output)
      : CompletionCallback(func, user_data, flags),
        output_(output) {
  }

  APIArgType output() const {
    return Traits::StorageToAPIArg(*output_);
  }

 private:
  OutputStorageType* output_;
};

}  

template <typename T>
class CompletionCallbackWithOutput
    : public internal::CompletionCallbackWithOutputBase<
        T, internal::CallbackOutputTraits<T> > {
 public:
  typedef internal::CompletionCallbackWithOutputBase<
      T, internal::CallbackOutputTraits<T> > BaseType;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  explicit CompletionCallbackWithOutput(
      typename BaseType::OutputStorageType* output)
      : BaseType(output) {
  }

  
  
  
  
  
  
  
  
  
  CompletionCallbackWithOutput(PP_CompletionCallback_Func func,
                               void* user_data,
                               typename BaseType::OutputStorageType* output)
      : BaseType(func, user_data, output) {
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  CompletionCallbackWithOutput(PP_CompletionCallback_Func func,
                               void* user_data,
                               int32_t flags,
                               typename BaseType::OutputStorageType* output)
      : BaseType(func, user_data, flags, output) {
  }
};

namespace ext {

template <typename T>
class ExtCompletionCallbackWithOutput
    : public ::pp::internal::CompletionCallbackWithOutputBase<
        T, internal::ExtCallbackOutputTraits<T> > {
 public:
  typedef ::pp::internal::CompletionCallbackWithOutputBase<
      T, internal::ExtCallbackOutputTraits<T> > BaseType;

  
  
  
  
  
  
  
  
  
  
  
  
  
  explicit ExtCompletionCallbackWithOutput(
      typename BaseType::OutputStorageType* output)
      : BaseType(output) {
  }

  
  
  
  
  
  
  
  
  
  ExtCompletionCallbackWithOutput(PP_CompletionCallback_Func func,
                                  void* user_data,
                                  typename BaseType::OutputStorageType* output)
      : BaseType(func, user_data, output) {
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  ExtCompletionCallbackWithOutput(PP_CompletionCallback_Func func,
                                  void* user_data,
                                  int32_t flags,
                                  typename BaseType::OutputStorageType* output)
      : BaseType(func, user_data, flags, output) {
  }
};

}  

inline CompletionCallback BlockUntilComplete() {
  
  
  return CompletionCallback();
}

}  

#endif  
