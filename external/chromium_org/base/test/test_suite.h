// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_TEST_SUITE_H_
#define BASE_TEST_TEST_SUITE_H_


#include <string>

#include "base/at_exit.h"
#include "base/memory/scoped_ptr.h"

namespace testing {
class TestInfo;
}

namespace base {

int RunUnitTestsUsingBaseTestSuite(int argc, char **argv);

class TestSuite {
 public:
  
  typedef bool (*TestMatch)(const testing::TestInfo&);

  TestSuite(int argc, char** argv);
  virtual ~TestSuite();

  
  
  
  static bool IsMarkedMaybe(const testing::TestInfo& test);

  void CatchMaybeTests();

  void ResetCommandLine();

  void AddTestLauncherResultPrinter();

  int Run();

 protected:
  
  
  
  TestSuite(int argc, char** argv, bool create_at_exit_manager);

  
  
  
  static void UnitTestAssertHandler(const std::string& str);

  
  virtual void SuppressErrorDialogs();

  
  

  virtual void Initialize();
  virtual void Shutdown();

  
  
  scoped_ptr<base::AtExitManager> at_exit_manager_;

 private:
  
  void PreInitialize(int argc, char** argv, bool create_at_exit_manager);

  bool initialized_command_line_;

  DISALLOW_COPY_AND_ASSIGN(TestSuite);
};

}  

using base::TestSuite;

#endif  
