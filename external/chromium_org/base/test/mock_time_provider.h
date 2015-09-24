// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef BASE_TEST_MOCK_TIME_PROVIDER_H_
#define BASE_TEST_MOCK_TIME_PROVIDER_H_

#include "base/time/time.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace base {

class MockTimeProvider {
 public:
  MockTimeProvider();
  ~MockTimeProvider();

  MOCK_METHOD0(Now, Time());

  static Time StaticNow();

 private:
  static MockTimeProvider* instance_;
  DISALLOW_COPY_AND_ASSIGN(MockTimeProvider);
};

}  

#endif  
