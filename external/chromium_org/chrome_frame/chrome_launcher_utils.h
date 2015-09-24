// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_CHROME_LAUNCHER_UTILS_H_
#define CHROME_FRAME_CHROME_LAUNCHER_UTILS_H_

#include <string>
#include "base/memory/scoped_ptr.h"

class CommandLine;

namespace base {
class FilePath;
}

namespace chrome_launcher {

extern const wchar_t kLauncherExeBaseName[];

bool CreateLaunchCommandLine(scoped_ptr<CommandLine>* command_line);

bool CreateUpdateCommandLine(const std::wstring& update_command,
                             scoped_ptr<CommandLine>* command_line);

base::FilePath GetChromeExecutablePath();

}  

#endif  
