// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_SETUP_SETUP_UTIL_H_
#define CHROME_INSTALLER_SETUP_SETUP_UTIL_H_

#include <windows.h>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "base/win/scoped_handle.h"
#include "chrome/installer/util/browser_distribution.h"
#include "chrome/installer/util/util_constants.h"

class CommandLine;

namespace base {
class FilePath;
class Version;
}

namespace installer {

class InstallationState;
class InstallerState;
class ProductState;

int CourgettePatchFiles(const base::FilePath& src,
                        const base::FilePath& patch,
                        const base::FilePath& dest);

int BsdiffPatchFiles(const base::FilePath& src,
                     const base::FilePath& patch,
                     const base::FilePath& dest);

Version* GetMaxVersionFromArchiveDir(const base::FilePath& chrome_path);

base::FilePath FindArchiveToPatch(const InstallationState& original_state,
                                  const InstallerState& installer_state);

bool DeleteFileFromTempProcess(const base::FilePath& path,
                               uint32 delay_before_delete_ms);

bool GetExistingHigherInstaller(const InstallationState& original_state,
                                bool system_install,
                                const base::Version& installer_version,
                                base::FilePath* setup_exe);

bool DeferToExistingInstall(const base::FilePath& setup_exe,
                            const CommandLine& command_line,
                            const InstallerState& installer_state,
                            const base::FilePath& temp_path,
                            InstallStatus* install_status);

bool WillProductBePresentAfterSetup(
    const installer::InstallerState& installer_state,
    const installer::InstallationState& machine_state,
    BrowserDistribution::Type type);

bool AdjustProcessPriority();

void MigrateGoogleUpdateStateMultiToSingle(
    bool system_level,
    BrowserDistribution::Type to_migrate,
    const installer::InstallationState& machine_state);

bool IsUninstallSuccess(InstallStatus install_status);

bool ContainsUnsupportedSwitch(const CommandLine& cmd_line);

class ScopedTokenPrivilege {
 public:
  explicit ScopedTokenPrivilege(const wchar_t* privilege_name);
  ~ScopedTokenPrivilege();

  
  bool is_enabled() const { return is_enabled_; }

 private:
  
  bool is_enabled_;

  
  
  base::win::ScopedHandle token_;

  
  
  TOKEN_PRIVILEGES previous_privileges_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ScopedTokenPrivilege);
};

}  

#endif  
