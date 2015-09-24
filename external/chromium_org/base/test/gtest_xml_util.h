// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_GTEST_XML_UTIL_H_
#define BASE_TEST_GTEST_XML_UTIL_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {

class FilePath;
struct TestResult;

class XmlUnitTestResultPrinter : public testing::EmptyTestEventListener {
 public:
  XmlUnitTestResultPrinter();
  virtual ~XmlUnitTestResultPrinter();

  
  bool Initialize(const FilePath& output_file_path) WARN_UNUSED_RESULT;

 private:
  
  virtual void OnTestCaseStart(const testing::TestCase& test_case) OVERRIDE;
  virtual void OnTestStart(const testing::TestInfo& test_info) OVERRIDE;
  virtual void OnTestEnd(const testing::TestInfo& test_info) OVERRIDE;
  virtual void OnTestCaseEnd(const testing::TestCase& test_case) OVERRIDE;

  FILE* output_file_;

  DISALLOW_COPY_AND_ASSIGN(XmlUnitTestResultPrinter);
};

bool ProcessGTestOutput(const base::FilePath& output_file,
                        std::vector<TestResult>* results,
                        bool* crashed) WARN_UNUSED_RESULT;

}  

#endif  
