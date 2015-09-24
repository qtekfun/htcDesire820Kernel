// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_TEST_UNITTEST_TEST_SUITE_H_
#define CONTENT_PUBLIC_TEST_UNITTEST_TEST_SUITE_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class TestSuite;
}

namespace content {

class UnitTestTestSuite {
 public:
   
  explicit UnitTestTestSuite(base::TestSuite* test_suite);
  ~UnitTestTestSuite();

  int Run();

 private:
  scoped_ptr<base::TestSuite> test_suite_;

#if !defined(OS_IOS)
  class UnitTestWebKitPlatformSupport;
  scoped_ptr<UnitTestWebKitPlatformSupport> webkit_platform_support_;
#endif

  DISALLOW_COPY_AND_ASSIGN(UnitTestTestSuite);
};

}  

#endif  
