// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_SETUP_INSTALL_H_
#define CHROME_INSTALLER_SETUP_INSTALL_H_

#include <vector>

#include "base/strings/string16.h"
#include "base/version.h"
#include "chrome/installer/util/installation_state.h"
#include "chrome/installer/util/installer_state.h"
#include "chrome/installer/util/master_preferences.h"
#include "chrome/installer/util/product.h"
#include "chrome/installer/util/util_constants.h"

namespace base {
class FilePath;
}

namespace installer {

class InstallationState;
class InstallerState;
class MasterPreferences;

enum InstallShortcutOperation {
  
  
  INSTALL_SHORTCUT_CREATE_ALL,
  
  
  
  INSTALL_SHORTCUT_CREATE_EACH_IF_NO_SYSTEM_LEVEL,
  
  
  INSTALL_SHORTCUT_REPLACE_EXISTING,
};

enum InstallShortcutLevel {
  
  CURRENT_USER,
  
  
  
  ALL_USERS,
};

void EscapeXmlAttributeValueInSingleQuotes(string16* att_value);

bool CreateVisualElementsManifest(const base::FilePath& src_path,
                                  const Version& version);

void CreateOrUpdateShortcuts(
    const base::FilePath& target,
    const Product& product,
    const MasterPreferences& prefs,
    InstallShortcutLevel install_level,
    InstallShortcutOperation install_operation);

void RegisterChromeOnMachine(const InstallerState& installer_state,
                             const Product& product,
                             bool make_chrome_default);

InstallStatus InstallOrUpdateProduct(
    const InstallationState& original_state,
    const InstallerState& installer_state,
    const base::FilePath& setup_path,
    const base::FilePath& archive_path,
    const base::FilePath& install_temp_path,
    const base::FilePath& src_path,
    const base::FilePath& prefs_path,
    const installer::MasterPreferences& prefs,
    const Version& new_version);

void HandleOsUpgradeForBrowser(const InstallerState& installer_state,
                               const Product& chrome);

void HandleActiveSetupForBrowser(const base::FilePath& installation_root,
                                 const Product& chrome,
                                 bool force);

bool InstallFromWebstore(const std::string& app_code);

}  

#endif  
