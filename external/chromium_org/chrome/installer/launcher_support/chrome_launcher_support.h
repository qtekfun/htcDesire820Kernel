// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_LAUNCHER_SUPPORT_CHROME_LAUNCHER_SUPPORT_H_
#define CHROME_INSTALLER_LAUNCHER_SUPPORT_CHROME_LAUNCHER_SUPPORT_H_

namespace base {
class FilePath;
}

namespace chrome_launcher_support {

enum InstallationLevel {
  USER_LEVEL_INSTALLATION,
  SYSTEM_LEVEL_INSTALLATION,
};

enum InstallationState {
  NOT_INSTALLED,
  INSTALLED_AT_USER_LEVEL,
  INSTALLED_AT_SYSTEM_LEVEL,
};

base::FilePath GetSetupExeForInstallationLevel(InstallationLevel level);

base::FilePath GetChromePathForInstallationLevel(InstallationLevel level);

base::FilePath GetAppHostPathForInstallationLevel(InstallationLevel level);

base::FilePath GetAnyChromePath();

base::FilePath GetAnyAppHostPath();

void UninstallLegacyAppLauncher(InstallationLevel level);

bool IsAppHostPresent();

InstallationState GetAppLauncherInstallationState();

bool IsAppLauncherPresent();

bool IsChromeBrowserPresent();

}  

#endif  
