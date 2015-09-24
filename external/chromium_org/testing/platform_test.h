// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TESTING_PLATFORM_TEST_H_
#define TESTING_PLATFORM_TEST_H_

#include <gtest/gtest.h>

#if defined(GTEST_OS_MAC)
#ifdef __OBJC__
@class NSAutoreleasePool;
#else
class NSAutoreleasePool;
#endif

class PlatformTest : public testing::Test {
 public:
  virtual ~PlatformTest();

 protected:
  PlatformTest();

 private:
  NSAutoreleasePool* pool_;
};
#else
typedef testing::Test PlatformTest;
#endif 

#endif 
