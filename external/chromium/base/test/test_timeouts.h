// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_TEST_TIMEOUTS_H_
#define BASE_TEST_TEST_TIMEOUTS_H_

#include "base/basictypes.h"

class TestTimeouts {
 public:
  
  
  static void Initialize();

  
  static int tiny_timeout_ms() { return tiny_timeout_ms_; }

  
  
  static int action_timeout_ms() { return action_timeout_ms_; }

  
  
  
  static int action_max_timeout_ms() { return action_max_timeout_ms_; }

  
  static int large_test_timeout_ms() { return large_test_timeout_ms_; }

  
  
  
  static int huge_test_timeout_ms() { return huge_test_timeout_ms_; }

  
  static int wait_for_terminate_timeout_ms() {
    return wait_for_terminate_timeout_ms_;
  }

  
  
  static int live_operation_timeout_ms() {
    return live_operation_timeout_ms_;
  }

 private:
  static bool initialized_;

  static int tiny_timeout_ms_;
  static int action_timeout_ms_;
  static int action_max_timeout_ms_;
  static int large_test_timeout_ms_;
  static int huge_test_timeout_ms_;
  static int wait_for_terminate_timeout_ms_;
  static int live_operation_timeout_ms_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(TestTimeouts);
};

#endif  
