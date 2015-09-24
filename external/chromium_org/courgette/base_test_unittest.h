// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COURGETTE_BASE_TEST_UNITTEST_H_
#define COURGETTE_BASE_TEST_UNITTEST_H_

#include <list>
#include <string>

#include "base/files/file_path.h"
#include "testing/gtest/include/gtest/gtest.h"

class BaseTest : public testing::Test {
 public:
  std::string FileContents(const char* file_name) const;

  
  
  std::string FilesContents(std::list<std::string> file_names) const;

 private:
  virtual void SetUp();
  virtual void TearDown();

  base::FilePath test_dir_;
};

#endif  
