// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#include "build/build_config.h"

RESULT_CODE(NORMAL_EXIT, 0)

RESULT_CODE(KILLED, 1)

RESULT_CODE(HUNG, 2)

RESULT_CODE(KILLED_BAD_MESSAGE, 3)

#if defined(OS_ANDROID)
RESULT_CODE(FAILED_TO_REGISTER_JNI, 4)

RESULT_CODE(NATIVE_LIBRARY_LOAD_FAILED, 5)

RESULT_CODE(NATIVE_LIBRARY_WRONG_VERSION, 6)

RESULT_CODE(NATIVE_STARTUP_FAILED, 7)
#endif
