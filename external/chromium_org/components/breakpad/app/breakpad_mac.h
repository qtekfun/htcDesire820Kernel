// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_BREAKPAD_APP_BREAKPAD_MAC_H_
#define COMPONENTS_BREAKPAD_APP_BREAKPAD_MAC_H_

#include <string>


namespace breakpad {

void InitCrashReporter(const std::string& process_type);

void InitCrashProcessInfo(const std::string& process_type_switch);

bool IsCrashReporterEnabled();

}  

#endif  
