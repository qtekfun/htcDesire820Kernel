// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_TEST_TIMEOUTS_H_
#define BASE_TEST_TEST_TIMEOUTS_H_

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/time/time.h"

class TestTimeouts {
 public:
  
  
  static void Initialize();

  
  static base::TimeDelta tiny_timeout() {
    DCHECK(initialized_);
    return base::TimeDelta::FromMilliseconds(tiny_timeout_ms_);
  }

  
  
  static base::TimeDelta action_timeout() {
    DCHECK(initialized_);
    return base::TimeDelta::FromMilliseconds(action_timeout_ms_);
  }

  
  
  
  static base::TimeDelta action_max_timeout() {
    DCHECK(initialized_);
    return base::TimeDelta::FromMilliseconds(action_max_timeout_ms_);
  }

  
  static base::TimeDelta large_test_timeout() {
    DCHECK(initialized_);
    return base::TimeDelta::FromMilliseconds(large_test_timeout_ms_);
  }

  
  
  static base::TimeDelta test_launcher_timeout() {
    DCHECK(initialized_);
    return base::TimeDelta::FromMilliseconds(test_launcher_timeout_ms_);
  }

 private:
  static bool initialized_;

  static int tiny_timeout_ms_;
  static int action_timeout_ms_;
  static int action_max_timeout_ms_;
  static int large_test_timeout_ms_;
  static int test_launcher_timeout_ms_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(TestTimeouts);
};

#endif  
