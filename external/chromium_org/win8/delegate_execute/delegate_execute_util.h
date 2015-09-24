// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WIN8_DELEGATE_EXECUTE_DELEGATE_EXECUTE_UTIL_H_
#define WIN8_DELEGATE_EXECUTE_DELEGATE_EXECUTE_UTIL_H_

#include "base/command_line.h"
#include "base/strings/string16.h"

namespace base {
class FilePath;
}

namespace delegate_execute {

CommandLine CommandLineFromParameters(const wchar_t* params);

CommandLine MakeChromeCommandLine(const base::FilePath& chrome_exe,
                                  const CommandLine& params,
                                  const string16& argument);

string16 ParametersFromSwitch(const char* a_switch);

}  

#endif  
