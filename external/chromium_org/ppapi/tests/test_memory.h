// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_TEST_TEST_MEMORY_H_
#define PPAPI_TEST_TEST_MEMORY_H_

#include <string>

#include "ppapi/c/dev/ppb_memory_dev.h"
#include "ppapi/tests/test_case.h"

class TestMemory : public TestCase {
 public:
  explicit TestMemory(TestingInstance* instance) : TestCase(instance) {}

 private:
  
  virtual bool Init();
  virtual void RunTests(const std::string& filter);

  std::string TestMemAlloc();
  std::string TestNullMemFree();

  
  const PPB_Memory_Dev* memory_dev_interface_;
};

#endif  
