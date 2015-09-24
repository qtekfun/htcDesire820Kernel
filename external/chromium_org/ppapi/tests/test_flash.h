// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PAPPI_TESTS_TEST_FLASH_H_
#define PAPPI_TESTS_TEST_FLASH_H_

#include <string>

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/private/ppb_flash.h"
#include "ppapi/tests/test_case.h"
#include "ppapi/utility/completion_callback_factory.h"

class TestFlash : public TestCase {
 public:
  explicit TestFlash(TestingInstance* instance);

  
  virtual void RunTests(const std::string& filter);

 private:
  
  std::string TestSetInstanceAlwaysOnTop();
  
  std::string TestGetProxyForURL();
  
  std::string TestGetLocalTimeZoneOffset();
  std::string TestGetCommandLineArgs();
  std::string TestGetSetting();
  std::string TestSetCrashData();

  pp::CompletionCallbackFactory<TestFlash> callback_factory_;
};

#endif  
