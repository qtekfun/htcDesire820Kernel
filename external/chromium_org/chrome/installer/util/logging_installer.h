// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_LOGGING_INSTALLER_H_
#define CHROME_INSTALLER_UTIL_LOGGING_INSTALLER_H_

#include "base/basictypes.h"

namespace base {
class FilePath;
}

namespace installer {

class MasterPreferences;

const int kMaxInstallerLogFileSize = 1024 * 1024;

const int kTruncatedInstallerLogFileSize = kMaxInstallerLogFileSize / 2;

COMPILE_ASSERT(kTruncatedInstallerLogFileSize < kMaxInstallerLogFileSize,
               kTruncatedInstallerLogFileSize_not_lt_kMaxInstallerLogFileSize);

enum TruncateResult {
  LOGFILE_UNTOUCHED,
  LOGFILE_TRUNCATED,
  LOGFILE_DELETED,
};

TruncateResult TruncateLogFileIfNeeded(const base::FilePath& log_file);

void InitInstallerLogging(const installer::MasterPreferences& prefs);

void EndInstallerLogging();

base::FilePath GetLogFilePath(const installer::MasterPreferences& prefs);

}  

#endif  
