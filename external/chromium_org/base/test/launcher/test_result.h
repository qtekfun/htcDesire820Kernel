// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_LAUNCHER_TEST_RESULT_H_
#define BASE_TEST_LAUNCHER_TEST_RESULT_H_

#include <string>

#include "base/time/time.h"

namespace base {

struct TestResult {
  enum Status {
    TEST_UNKNOWN,          
    TEST_SUCCESS,          
    TEST_FAILURE,          
    TEST_FAILURE_ON_EXIT,  
    TEST_TIMEOUT,          
    TEST_CRASH,            
    TEST_SKIPPED,          
  };

  TestResult();
  ~TestResult();

  
  std::string StatusAsString() const;

  
  std::string GetTestName() const;

  
  std::string GetTestCaseName() const;

  
  
  
  bool completed() const {
    return status == TEST_SUCCESS ||
        status == TEST_FAILURE ||
        status == TEST_FAILURE_ON_EXIT;
  }

  
  std::string full_name;

  Status status;

  
  base::TimeDelta elapsed_time;

  
  std::string output_snippet;
};

}  

#endif  
