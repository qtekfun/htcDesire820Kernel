// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_TESTS_TEST_CASE_H_
#define PPAPI_TESTS_TEST_CASE_H_

#include <cmath>
#include <limits>
#include <map>
#include <set>
#include <string>

#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_time.h"
#include "ppapi/c/private/ppb_testing_private.h"
#include "ppapi/cpp/dev/scrollbar_dev.h"
#include "ppapi/cpp/message_loop.h"
#include "ppapi/cpp/view.h"
#include "ppapi/tests/test_utils.h"
#include "ppapi/tests/testing_instance.h"

#if (defined __native_client__)
#include "ppapi/cpp/var.h"
#else
#include "ppapi/cpp/private/var_private.h"
#endif

class TestingInstance;

namespace pp {
namespace deprecated {
class ScriptableObject;
}
}

class TestCase {
 public:
  explicit TestCase(TestingInstance* instance);
  virtual ~TestCase();

  
  
  virtual bool Init();

  
  
  
  
  
  
  
  
  
  virtual void RunTests(const std::string& test_filter) = 0;

  static std::string MakeFailureMessage(const char* file, int line,
                                        const char* cmd);

#if !(defined __native_client__)
  
  
  pp::VarPrivate GetTestObject();
#endif

  
  
  
  
  virtual void HandleMessage(const pp::Var& message_data);

  
  
  
  virtual void DidChangeView(const pp::View& view);

  
  
  
  virtual bool HandleInputEvent(const pp::InputEvent& event);

  void IgnoreLeakedVar(int64_t id);

  TestingInstance* instance() { return instance_; }

  const PPB_Testing_Private* testing_interface() { return testing_interface_; }

  static void QuitMainMessageLoop(PP_Instance instance);

  const std::map<std::string, bool>& remaining_tests() {
    return remaining_tests_;
  }
  const std::set<std::string>& skipped_tests() {
    return skipped_tests_;
  }

 protected:
#if !(defined __native_client__)
  
  
  
  
  
  
  virtual pp::deprecated::ScriptableObject* CreateTestObject();
#endif

  
  
  
  bool CheckTestingInterface();

  
  bool EnsureRunningOverHTTP();

  
  
  
  bool ShouldRunAllTests(const std::string& filter);

  
  
  
  bool ShouldRunTest(const std::string& test_name, const std::string& filter);

  
  
  
  
  
  
  std::string CheckResourcesAndVars(std::string errors);

  PP_TimeTicks NowInTimeTicks();

  
  template <class T>
  std::string RunOnThread(std::string(T::*test_to_run)()) {
    if (!testing_interface_) {
      return "Testing blocking callbacks requires the testing interface. In "
             "Chrome, use the --enable-pepper-testing flag.";
    }
    
    
    if (!testing_interface_->IsOutOfProcess())
      return std::string();
    pp::MessageLoop background_loop(instance_);
    ThreadedTestRunner<T> runner(instance_->pp_instance(),
        static_cast<T*>(this), test_to_run, background_loop);
    RunOnThreadInternal(&ThreadedTestRunner<T>::ThreadFunction, &runner,
                        testing_interface_);
    return runner.result();
  }

  
  TestingInstance* instance_;

  
  const PPB_Testing_Private* testing_interface_;

  void set_callback_type(CallbackType callback_type) {
    callback_type_ = callback_type;
  }
  CallbackType callback_type() const {
    return callback_type_;
  }

 private:
  template <class T>
  class ThreadedTestRunner {
   public:
    typedef std::string(T::*TestMethodType)();
    ThreadedTestRunner(PP_Instance instance,
                       T* test_case,
                       TestMethodType test_to_run,
                       pp::MessageLoop loop)
        : instance_(instance),
          test_case_(test_case),
          test_to_run_(test_to_run),
          loop_(loop) {
    }
    const std::string& result() { return result_; }
    static void ThreadFunction(void* runner) {
      static_cast<ThreadedTestRunner<T>*>(runner)->Run();
    }

   private:
    void Run() {
      int32_t result = loop_.AttachToCurrentThread();
      static_cast<void>(result); 
      PP_DCHECK(PP_OK == result);
      result_ = (test_case_->*test_to_run_)();
      
      loop_.PostQuit(true );
      loop_.Run();
      
      
      TestCase::QuitMainMessageLoop(instance_);
    }

    std::string result_;
    PP_Instance instance_;
    T* test_case_;
    TestMethodType test_to_run_;
    pp::MessageLoop loop_;
  };

  
  
  
  
  
  void RunOnThreadInternal(void (*thread_func)(void*),
                           void* thread_param,
                           const PPB_Testing_Private* testing_interface);

  static void DoQuitMainMessageLoop(void* pp_instance, int32_t result);

  
  
  CallbackType callback_type_;

  
  std::set<int64_t> ignored_leaked_vars_;

  
  
  
  
  
  std::map<std::string, bool> filter_tests_;
  
  bool have_populated_filter_tests_;
  
  
  
  
  
  
  std::map<std::string, bool> remaining_tests_;

  
  
  
  std::set<std::string> skipped_tests_;

#if !(defined __native_client__)
  
  pp::VarPrivate test_object_;
#endif
};

class TestCaseFactory {
 public:
  typedef TestCase* (*Method)(TestingInstance* instance);

  TestCaseFactory(const char* name, Method method)
      : next_(head_),
        name_(name),
        method_(method) {
    head_ = this;
  }

 private:
  friend class TestingInstance;

  TestCaseFactory* next_;
  const char* name_;
  Method method_;

  static TestCaseFactory* head_;
};

#define REGISTER_TEST_CASE(name)                                            \
  static TestCase* Test##name##_FactoryMethod(TestingInstance* instance) {  \
    return new Test##name(instance);                                        \
  }                                                                         \
  static TestCaseFactory g_Test##name_factory(                              \
    #name, &Test##name##_FactoryMethod                                      \
  )

#define RUN_TEST(name, test_filter) \
  if (ShouldRunTest(#name, test_filter)) { \
    set_callback_type(PP_OPTIONAL); \
    PP_TimeTicks start_time(NowInTimeTicks()); \
    instance_->LogTest(#name, \
                       CheckResourcesAndVars(Test##name()), \
                       start_time); \
  }

#define RUN_TEST_FORCEASYNC(name, test_filter) \
  if (ShouldRunTest(#name, test_filter)) { \
    set_callback_type(PP_REQUIRED); \
    PP_TimeTicks start_time(NowInTimeTicks()); \
    instance_->LogTest(#name"ForceAsync", \
                       CheckResourcesAndVars(Test##name()), \
                       start_time); \
  }

#define RUN_TEST_BLOCKING(test_case, name, test_filter) \
  if (ShouldRunTest(#name, test_filter)) { \
    set_callback_type(PP_BLOCKING); \
    PP_TimeTicks start_time(NowInTimeTicks()); \
    instance_->LogTest( \
        #name"Blocking", \
        CheckResourcesAndVars(RunOnThread(&test_case::Test##name)), \
        start_time); \
  }

#define RUN_TEST_BACKGROUND(test_case, name, test_filter) \
  if (ShouldRunTest(#name, test_filter)) { \
    PP_TimeTicks start_time(NowInTimeTicks()); \
    instance_->LogTest( \
        #name"Background", \
        CheckResourcesAndVars(RunOnThread(&test_case::Test##name)), \
        start_time); \
  }

#define RUN_TEST_FORCEASYNC_AND_NOT(name, test_filter) \
  do { \
    RUN_TEST_FORCEASYNC(name, test_filter); \
    RUN_TEST(name, test_filter); \
  } while (false)

#define RUN_CALLBACK_TEST(test_case, name, test_filter) \
  do { \
    RUN_TEST_FORCEASYNC(name, test_filter); \
    RUN_TEST(name, test_filter); \
    RUN_TEST_BLOCKING(test_case, name, test_filter); \
    RUN_TEST_BACKGROUND(test_case, name, test_filter); \
  } while (false)

#define RUN_TEST_WITH_REFERENCE_CHECK(name, test_filter) \
  if (ShouldRunTest(#name, test_filter)) { \
    set_callback_type(PP_OPTIONAL); \
    uint32_t objects = testing_interface_->GetLiveObjectsForInstance( \
        instance_->pp_instance()); \
    std::string error_message = Test##name(); \
    if (error_message.empty() && \
        testing_interface_->GetLiveObjectsForInstance( \
            instance_->pp_instance()) != objects) \
      error_message = MakeFailureMessage(__FILE__, __LINE__, \
          "reference leak check"); \
    PP_TimeTicks start_time(NowInTimeTicks()); \
    instance_->LogTest(#name, \
                       CheckResourcesAndVars(error_message), \
                       start_time); \
  }

#define ASSERT_TRUE(cmd) \
  do { \
    if (!(cmd)) \
      return MakeFailureMessage(__FILE__, __LINE__, #cmd); \
  } while (false)
#define ASSERT_FALSE(cmd) ASSERT_TRUE(!(cmd))
#define ASSERT_EQ(a, b) ASSERT_TRUE((a) == (b))
#define ASSERT_NE(a, b) ASSERT_TRUE((a) != (b))
#define ASSERT_LT(a, b) ASSERT_TRUE((a) < (b))
#define ASSERT_LE(a, b) ASSERT_TRUE((a) <= (b))
#define ASSERT_GT(a, b) ASSERT_TRUE((a) > (b))
#define ASSERT_GE(a, b) ASSERT_TRUE((a) >= (b))

#define ASSERT_DOUBLE_EQ(a, b) ASSERT_TRUE( \
    std::fabs((a)-(b)) <= std::numeric_limits<double>::epsilon())

#define ASSERT_SUBTEST_SUCCESS(function) \
  do { \
    std::string result = (function); \
    if (!result.empty()) \
      return result; \
  } while (false)

#define PASS() return std::string()

#endif  
