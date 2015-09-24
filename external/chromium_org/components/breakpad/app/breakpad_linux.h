// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef COMPONENTS_BREAKPAD_APP_BREAKPAD_LINUX_H_
#define COMPONENTS_BREAKPAD_APP_BREAKPAD_LINUX_H_

#include <string>

#include "build/build_config.h"

namespace breakpad {

extern void InitCrashReporter(const std::string& process_type);

#if defined(OS_ANDROID)
extern void InitNonBrowserCrashReporterForAndroid(
    const std::string& process_type);
#endif

bool IsCrashReporterEnabled();

}  

#endif  
