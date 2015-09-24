// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_TEST_TEST_VAR_RESOURCE_H_
#define PPAPI_TEST_TEST_VAR_RESOURCE_H_

#include <string>

#include "ppapi/c/dev/ppb_var_resource_dev.h"
#include "ppapi/c/ppb_file_system.h"
#include "ppapi/c/ppb_var.h"
#include "ppapi/tests/test_case.h"

class TestVarResource : public TestCase {
 public:
  explicit TestVarResource(TestingInstance* instance) : TestCase(instance) {}

 private:
  
  virtual bool Init();
  virtual void RunTests(const std::string& filter);

  std::string TestBasicResource();
  std::string TestInvalidAndEmpty();
  std::string TestWrongType();

  
  const PPB_Core* core_interface_;
  const PPB_FileSystem* file_system_interface_;
  const PPB_Var* var_interface_;
  const PPB_VarResource_Dev* var_resource_interface_;
};

#endif  
