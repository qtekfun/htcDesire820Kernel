// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_TESTS_TEST_UTILS_H_
#define PPAPI_TESTS_TEST_UTILS_H_

#include <string>

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/private/ppb_testing_private.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/message_loop.h"
#include "ppapi/utility/completion_callback_factory.h"

namespace pp {
class NetAddress;
}

extern const int kActionTimeoutMs;

const PPB_Testing_Private* GetTestingInterface();
std::string ReportError(const char* method, int32_t error);
void PlatformSleep(int duration_ms);
bool GetLocalHostPort(PP_Instance instance, std::string* host, uint16_t* port);

uint16_t ConvertFromNetEndian16(uint16_t x);
uint16_t ConvertToNetEndian16(uint16_t x);
bool EqualNetAddress(const pp::NetAddress& addr1, const pp::NetAddress& addr2);
bool ResolveHost(PP_Instance instance,
                 const std::string& host,
                 uint16_t port,
                 pp::NetAddress* addr);
bool ReplacePort(PP_Instance instance,
                 const pp::NetAddress& input_addr,
                 uint16_t port,
                 pp::NetAddress* output_addr);
uint16_t GetPort(const pp::NetAddress& addr);

class NestedEvent {
 public:
  explicit NestedEvent(PP_Instance instance)
      : instance_(instance), waiting_(false), signalled_(false) {
  }
  
  
  void Wait();
  
  
  void Signal();
  
  
  void PostSignal(int32_t wait_ms);

  
  void Reset();
 private:
  void SignalOnMainThread();
  static void SignalThunk(void* async_event, int32_t result);

  PP_Instance instance_;
  bool waiting_;
  bool signalled_;
  
  NestedEvent(const NestedEvent&);
  NestedEvent& operator=(const NestedEvent&);
};

enum CallbackType { PP_REQUIRED, PP_OPTIONAL, PP_BLOCKING };
class TestCompletionCallback {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}
    virtual void OnCallback(void* user_data, int32_t result) = 0;
  };
  explicit TestCompletionCallback(PP_Instance instance);
  
  TestCompletionCallback(PP_Instance instance, bool force_async);

  TestCompletionCallback(PP_Instance instance, CallbackType callback_type);

  
  
  
  void SetDelegate(Delegate* delegate) { delegate_ = delegate; }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void WaitForResult(int32_t result);

  
  
  
  
  
  
  
  
  
  
  void WaitForAbortResult(int32_t result);

  
  
  pp::CompletionCallback GetCallback();

  bool failed() { return !errors_.empty(); }
  const std::string& errors() { return errors_; }

  int32_t result() const { return result_; }

  
  void Reset();

  CallbackType callback_type() { return callback_type_; }
  void set_target_loop(const pp::MessageLoop& loop) { target_loop_ = loop; }
  static void Handler(void* user_data, int32_t result);

 protected:
  void RunMessageLoop();
  void QuitMessageLoop();

  
  
  bool wait_for_result_called_;
  
  bool have_result_;
  
  int32_t result_;
  CallbackType callback_type_;
  bool post_quit_task_;
  std::string errors_;
  PP_Instance instance_;
  Delegate* delegate_;
  pp::MessageLoop target_loop_;
};

namespace internal {

template <typename OutputT, typename CallbackT>
class TestCompletionCallbackWithOutputBase {
 public:
  explicit TestCompletionCallbackWithOutputBase(PP_Instance instance)
      : callback_(instance),
        output_storage_() {
    CallbackT::TraitsType::Initialize(&output_storage_);
  }

  TestCompletionCallbackWithOutputBase(PP_Instance instance, bool force_async)
      : callback_(instance, force_async),
        output_storage_() {
    CallbackT::TraitsType::Initialize(&output_storage_);
  }

  TestCompletionCallbackWithOutputBase(PP_Instance instance,
                                       CallbackType callback_type)
      : callback_(instance, callback_type),
        output_storage_() {
    CallbackT::TraitsType::Initialize(&output_storage_);
  }

  CallbackT GetCallback();
  OutputT output() {
    return CallbackT::TraitsType::StorageToPluginArg(
        output_storage_);
  }

  
  void SetDelegate(TestCompletionCallback::Delegate* delegate) {
    callback_.SetDelegate(delegate);
  }
  void WaitForResult(int32_t result) { callback_.WaitForResult(result); }
  void WaitForAbortResult(int32_t result) {
    callback_.WaitForAbortResult(result);
  }
  bool failed() { return callback_.failed(); }
  const std::string& errors() { return callback_.errors(); }
  int32_t result() const { return callback_.result(); }
  void Reset() {
    CallbackT::TraitsType::Initialize(&output_storage_);
    return callback_.Reset();
  }

 private:
  TestCompletionCallback callback_;
  typename CallbackT::OutputStorageType output_storage_;
};

template <typename OutputT, typename CallbackT>
CallbackT
TestCompletionCallbackWithOutputBase<OutputT, CallbackT>::GetCallback() {
  this->Reset();
  if (callback_.callback_type() == PP_BLOCKING) {
    CallbackT cc(&output_storage_);
    return cc;
  }

  callback_.set_target_loop(pp::MessageLoop::GetCurrent());
  CallbackT cc(&TestCompletionCallback::Handler, this, &output_storage_);
  if (callback_.callback_type() == PP_OPTIONAL)
    cc.set_flags(PP_COMPLETIONCALLBACK_FLAG_OPTIONAL);
  return cc;
}

}  

template <typename OutputT>
class TestCompletionCallbackWithOutput
    : public internal::TestCompletionCallbackWithOutputBase<
        OutputT, pp::CompletionCallbackWithOutput<OutputT> > {
 public:
  explicit TestCompletionCallbackWithOutput(PP_Instance instance)
      : BaseType(instance) {
  }

  TestCompletionCallbackWithOutput(PP_Instance instance, bool force_async)
      : BaseType(instance, force_async) {
  }

  TestCompletionCallbackWithOutput(PP_Instance instance,
                                   CallbackType callback_type)
      : BaseType(instance, callback_type) {
  }

 private:
  typedef internal::TestCompletionCallbackWithOutputBase<
      OutputT, pp::CompletionCallbackWithOutput<OutputT> > BaseType;
};

template <typename OutputT>
class TestExtCompletionCallbackWithOutput
    : public internal::TestCompletionCallbackWithOutputBase<
        OutputT, pp::ext::ExtCompletionCallbackWithOutput<OutputT> > {
 public:
  explicit TestExtCompletionCallbackWithOutput(PP_Instance instance)
      : BaseType(instance) {
  }

  TestExtCompletionCallbackWithOutput(PP_Instance instance, bool force_async)
      : BaseType(instance, force_async) {
  }

  TestExtCompletionCallbackWithOutput(PP_Instance instance,
                                      CallbackType callback_type)
      : BaseType(instance, callback_type) {
  }

 private:
  typedef internal::TestCompletionCallbackWithOutputBase<
      OutputT, pp::ext::ExtCompletionCallbackWithOutput<OutputT> > BaseType;
};

#define CHECK_CALLBACK_BEHAVIOR(callback) \
do { \
  if ((callback).failed()) \
    return MakeFailureMessage(__FILE__, __LINE__, \
                              (callback).errors().c_str()); \
} while (false)

#if defined(__APPLE__)
#define PPAPI_OS_MACOSX 1
#elif defined(ANDROID)
#define PPAPI_OS_ANDROID 1
#elif defined(__native_client__)
#define PPAPI_OS_NACL 1
#elif defined(__linux__)
#define PPAPI_OS_LINUX 1
#elif defined(_WIN32)
#define PPAPI_OS_WIN 1
#elif defined(__FreeBSD__)
#define PPAPI_OS_FREEBSD 1
#elif defined(__OpenBSD__)
#define PPAPI_OS_OPENBSD 1
#elif defined(__sun)
#define PPAPI_OS_SOLARIS 1
#else
#error Please add support for your platform in ppapi/tests/test_utils.h
#endif

#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || \
    defined(__OpenBSD__) || defined(__sun) || defined(__native_client__)
#define PPAPI_POSIX 1
#endif

#endif  
