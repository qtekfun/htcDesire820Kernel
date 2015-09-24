// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_TESTS_TEST_FLASH_FILE_H_
#define PPAPI_TESTS_TEST_FLASH_FILE_H_

#include <string>

#include "ppapi/tests/test_case.h"

class TestFlashFile: public TestCase {
 public:
  explicit TestFlashFile(TestingInstance* instance);
  virtual ~TestFlashFile();

  
  virtual bool Init();
  virtual void RunTests(const std::string& filter);

 private:
  
  void SetUp();

  std::string TestOpenFile();
  std::string TestRenameFile();
  std::string TestDeleteFileOrDir();
  std::string TestCreateDir();
  std::string TestQueryFile();
  std::string TestGetDirContents();
  std::string TestCreateTemporaryFile();

  
  
  

  
  
  std::string GetItemCountUnderModuleLocalRoot(size_t* item_count);
};

#endif  
