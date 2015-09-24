// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_LINUX_TESTS_UNIT_TESTS_H__
#define SANDBOX_LINUX_TESTS_UNIT_TESTS_H__

#include "base/basictypes.h"
#include "build/build_config.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace sandbox {

bool IsAndroid();

bool IsArchitectureArm();

bool IsRunningOnValgrind();

#if defined(THREAD_SANITIZER)
#define DISABLE_ON_TSAN(test_name) DISABLED_##test_name
#else
#define DISABLE_ON_TSAN(test_name) test_name
#endif  

#define DEATH_SUCCESS() sandbox::UnitTests::DeathSuccess, NULL
#define DEATH_MESSAGE(msg)          \
  sandbox::UnitTests::DeathMessage, \
      static_cast<const void*>(static_cast<const char*>(msg))
#define DEATH_EXIT_CODE(rc)          \
  sandbox::UnitTests::DeathExitCode, \
      reinterpret_cast<void*>(static_cast<intptr_t>(rc))
#define DEATH_BY_SIGNAL(s)           \
  sandbox::UnitTests::DeathExitCode, \
      reinterpret_cast<void*>(static_cast<intptr_t>(s))

#define SANDBOX_DEATH_TEST(test_case_name, test_name, death)             \
  void TEST_##test_name(void*);                                          \
  TEST(test_case_name, test_name) {                                      \
    sandbox::UnitTests::RunTestInProcess(TEST_##test_name, NULL, death); \
  }                                                                      \
  void TEST_##test_name(void*)

#define SANDBOX_TEST(test_case_name, test_name) \
  SANDBOX_DEATH_TEST(test_case_name, test_name, DEATH_SUCCESS())

#define SANDBOX_STR(x) #x
#define SANDBOX_ASSERT(expr)                                             \
  ((expr) ? static_cast<void>(0) : sandbox::UnitTests::AssertionFailure( \
                                       SANDBOX_STR(expr), __FILE__, __LINE__))

class UnitTests {
 public:
  typedef void (*Test)(void*);
  typedef void (*DeathCheck)(int status,
                             const std::string& msg,
                             const void* aux);

  
  
  
  
  static void RunTestInProcess(Test test,
                               void* arg,
                               DeathCheck death,
                               const void* death_aux);

  
  
  
  static void AssertionFailure(const char* expr, const char* file, int line);

  
  
  
  
  
  
  static void IgnoreThisTest();

  
  
  
  static void DeathSuccess(int status, const std::string& msg, const void*);

  
  
  
  
  
  static void DeathMessage(int status, const std::string& msg, const void* aux);

  
  
  
  
  static void DeathExitCode(int status,
                            const std::string& msg,
                            const void* aux);

  
  
  
  
  static void DeathBySignal(int status,
                            const std::string& msg,
                            const void* aux);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(UnitTests);
};

}  

#endif  
