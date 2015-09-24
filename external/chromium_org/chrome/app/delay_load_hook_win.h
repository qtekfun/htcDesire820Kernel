// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_APP_DELAY_LOAD_HOOK_WIN_H_
#define CHROME_APP_DELAY_LOAD_HOOK_WIN_H_

#include <windows.h>

struct DelayLoadInfo;

extern "C" FARPROC WINAPI ChromeDelayLoadHook(unsigned reason,
                                              DelayLoadInfo* info);

#endif  
