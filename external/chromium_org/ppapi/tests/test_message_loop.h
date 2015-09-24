// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PAPPI_TESTS_TEST_MESSAGE_LOOP_H_
#define PAPPI_TESTS_TEST_MESSAGE_LOOP_H_

#include <string>

#include "ppapi/c/pp_stdint.h"
#include "ppapi/tests/test_case.h"
#include "ppapi/utility/completion_callback_factory.h"

namespace pp {
class MessageLoop;
}

class TestMessageLoop : public TestCase {
 public:
  explicit TestMessageLoop(TestingInstance* instance);
  virtual ~TestMessageLoop();

 private:
  
  virtual void RunTests(const std::string& filter);

  std::string TestBasics();
  std::string TestPost();

  
  enum TestParam { kInvalid, kMainToMain, kBeforeStart, kAfterStart};

  
  void SetParamAndQuitTask(int32_t result, TestParam param);

  
  
  void EchoParamToMainTask(int32_t result, TestParam param);

  
  
  TestParam param_;
  pp::CompletionCallbackFactory<TestMessageLoop> callback_factory_;
  NestedEvent main_loop_task_ran_;
};

#endif  
