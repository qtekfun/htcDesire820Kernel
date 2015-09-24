// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_LOGGING_CHROME_H__
#define CHROME_COMMON_LOGGING_CHROME_H__

#include <string>
#include <vector>

#include "base/logging.h"
#include "base/time/time.h"

class CommandLine;

namespace base {
class FilePath;
}

namespace logging {

void InitChromeLogging(const CommandLine& command_line,
                       OldFileDeletionState delete_old_log_file);

#if defined(OS_CHROMEOS)
base::FilePath GetSessionLogFile(const CommandLine& command_line);

void RedirectChromeLogging(const CommandLine& command_line);
#endif

void CleanupChromeLogging();

base::FilePath GetLogFileName();

bool DialogsAreSuppressed();

typedef std::vector<std::wstring> AssertionList;

size_t GetFatalAssertions(AssertionList* assertions);

base::FilePath GenerateTimestampedName(const base::FilePath& base_path,
                                       base::Time timestamp);
}  

#endif  
