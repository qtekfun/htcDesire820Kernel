// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HANG_MONITOR_HANG_CRASH_DUMP_WIN_H_
#define CHROME_BROWSER_HANG_MONITOR_HANG_CRASH_DUMP_WIN_H_

#include <windows.h>

void CrashDumpAndTerminateHungChildProcess(HANDLE hprocess);

void CrashDumpForHangDebugging(HANDLE hprocess);

#endif  
