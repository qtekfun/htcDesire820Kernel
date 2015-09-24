// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_CRASH_SERVER_INIT_H_
#define CHROME_FRAME_CRASH_SERVER_INIT_H_

#include "breakpad/src/client/windows/handler/exception_handler.h"

extern const wchar_t kChromePipeName[];
extern const wchar_t kGoogleUpdatePipeName[];
extern const wchar_t kSystemPrincipalSid[];

extern const MINIDUMP_TYPE kLargerDumpType;

enum CrashReportingMode {
  HEADLESS,  
  NORMAL     
};

google_breakpad::CustomClientInfo* GetCustomInfo();

google_breakpad::ExceptionHandler* InitializeCrashReporting(
    CrashReportingMode mode);

#endif  
