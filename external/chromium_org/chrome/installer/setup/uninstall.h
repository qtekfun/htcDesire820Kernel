// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_SETUP_UNINSTALL_H_
#define CHROME_INSTALLER_SETUP_UNINSTALL_H_

#include <shlobj.h>

#include "base/strings/string16.h"
#include "chrome/installer/util/util_constants.h"

class BrowserDistribution;
class CommandLine;

namespace base {
class FilePath;
}

namespace installer {

class InstallationState;
class InstallerState;
class Product;

enum DeleteResult {
  DELETE_SUCCEEDED,
  DELETE_NOT_EMPTY,
  DELETE_FAILED,
  DELETE_REQUIRES_REBOOT,
};

DeleteResult DeleteChromeDirectoriesIfEmpty(
    const base::FilePath& application_directory);

bool DeleteChromeRegistrationKeys(const InstallerState& installer_state,
                                  BrowserDistribution* dist,
                                  HKEY root,
                                  const string16& browser_entry_suffix,
                                  InstallStatus* exit_code);

void RemoveChromeLegacyRegistryKeys(BrowserDistribution* dist,
                                    const string16& chrome_exe);

installer::InstallStatus UninstallProduct(
    const InstallationState& original_state,
    const InstallerState& installer_state,
    const base::FilePath& setup_path,
    const Product& dist,
    bool remove_all,
    bool force_uninstall,
    const CommandLine& cmd_line);

void CleanUpInstallationDirectoryAfterUninstall(
    const InstallationState& original_state,
    const InstallerState& installer_state,
    const CommandLine& cmd_line,
    InstallStatus* uninstall_status);

}  

#endif  
