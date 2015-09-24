// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_AUTO_LAUNCH_UTIL_H_
#define CHROME_INSTALLER_UTIL_AUTO_LAUNCH_UTIL_H_

#include "base/strings/string16.h"

namespace base {
class FilePath;
}

namespace auto_launch_util {

bool AutoStartRequested(const string16& profile_directory,
                        bool window_requested,
                        const base::FilePath& application_path);

void DisableAllAutoStartFeatures(const string16& profile_directory);

void EnableForegroundStartAtLogin(const string16& profile_directory,
                                  const base::FilePath& application_path);

void DisableForegroundStartAtLogin(const string16& profile_directory);

void EnableBackgroundStartAtLogin();

void DisableBackgroundStartAtLogin();

}  

#endif  
