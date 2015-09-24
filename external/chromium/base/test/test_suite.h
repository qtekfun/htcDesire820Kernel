// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_TEST_SUITE_H_
#define BASE_TEST_TEST_SUITE_H_
#pragma once


#include <string>

#include "base/at_exit.h"

namespace testing {
class TestInfo;
}

namespace base {

class TestSuite {
 public:
  
  typedef bool (*TestMatch)(const testing::TestInfo&);

  TestSuite(int argc, char** argv);
  virtual ~TestSuite();

  
  static bool IsMarkedFlaky(const testing::TestInfo& test);

  
  static bool IsMarkedFailing(const testing::TestInfo& test);

  
  
  
  static bool IsMarkedMaybe(const testing::TestInfo& test);

  
  static bool ShouldIgnoreFailure(const testing::TestInfo& test);

  
  static bool NonIgnoredFailures(const testing::TestInfo& test);

  
  int GetTestCount(TestMatch test_match);

  void CatchMaybeTests();

  int Run();

  
  
  static const char kStrictFailureHandling[];

 protected:
  
  
  
  static void UnitTestAssertHandler(const std::string& str);

  
  virtual void SuppressErrorDialogs();

  
  

  virtual void Initialize();
  virtual void Shutdown();

  
  
  base::AtExitManager at_exit_manager_;

  DISALLOW_COPY_AND_ASSIGN(TestSuite);
};

}  

using base::TestSuite;

#endif  
