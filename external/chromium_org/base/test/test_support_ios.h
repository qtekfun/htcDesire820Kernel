// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_TEST_SUPPORT_IOS_H_
#define BASE_TEST_TEST_SUPPORT_IOS_H_

#include "base/test/test_suite.h"

namespace base {

void InitIOSTestMessageLoop();

void InitIOSRunHook(TestSuite* suite, int argc, char* argv[]);

void RunTestsFromIOSApp();

}  

#endif  
