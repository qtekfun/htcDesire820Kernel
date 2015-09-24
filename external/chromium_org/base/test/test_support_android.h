// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_TEST_SUPPORT_ANDROID_H_
#define BASE_TEST_TEST_SUPPORT_ANDROID_H_

#include "base/base_export.h"

namespace base {

BASE_EXPORT void InitAndroidTestLogging();

BASE_EXPORT void InitAndroidTestPaths();

BASE_EXPORT void InitAndroidTestMessageLoop();

BASE_EXPORT void InitAndroidTest();

}  

#endif  
