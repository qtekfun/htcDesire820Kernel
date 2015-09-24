// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PAPPI_TESTS_TEST_BUFFER_H_
#define PAPPI_TESTS_TEST_BUFFER_H_

#include <string>

#include "ppapi/c/dev/ppb_buffer_dev.h"
#include "ppapi/tests/test_case.h"

class TestBuffer : public TestCase {
 public:
  explicit TestBuffer(TestingInstance* instance) : TestCase(instance) {}

  
  virtual bool Init();
  virtual void RunTests(const std::string& filter);

 private:
  std::string TestInvalidSize();
  std::string TestInitToZero();
  std::string TestIsBuffer();
  std::string TestBasicLifeCycle();

  
  const PPB_Buffer_Dev* buffer_interface_;
};

#endif  
