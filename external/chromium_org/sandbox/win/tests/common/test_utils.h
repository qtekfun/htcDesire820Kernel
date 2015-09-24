// Copyright (c) 2006-2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_TESTS_COMMON_TEST_UTILS_H_
#define SANDBOX_TESTS_COMMON_TEST_UTILS_H_

#include <windows.h>

bool SetReparsePoint(HANDLE source, const wchar_t* target);

bool DeleteReparsePoint(HANDLE source);

#endif  

