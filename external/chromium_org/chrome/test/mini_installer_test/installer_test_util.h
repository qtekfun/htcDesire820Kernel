// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_MINI_INSTALLER_TEST_INSTALLER_TEST_UTIL_H_
#define CHROME_TEST_MINI_INSTALLER_TEST_INSTALLER_TEST_UTIL_H_

#include <vector>

#include "base/command_line.h"
#include "base/files/file_path.h"
#include "chrome/installer/util/installation_validator.h"
#include "chrome/test/mini_installer_test/switch_builder.h"

namespace installer_test {

struct InstalledProduct {
  std::string version;
  installer::InstallationValidator::InstallationType type;
  bool system;
};


bool DeleteInstallDirectory(
    bool system_level,
    installer::InstallationValidator::InstallationType type);

bool DeleteRegistryKey(
    bool system_level,
    installer::InstallationValidator::InstallationType type);

bool GetChromeInstallDirectory(bool system_level, base::FilePath* path);

bool GetInstallDirectory(bool system_level,
                         BrowserDistribution::Type type, base::FilePath* path);

std::string GetVersion(
    installer::InstallationValidator::InstallationType product);

bool GetInstalledProducts(std::vector<InstalledProduct>* products);

bool Install(const base::FilePath& installer);
bool Install(const base::FilePath& installer, const SwitchBuilder& switches);
bool LaunchChrome(bool close_after_launch, bool system_level);
bool LaunchIE(const std::string& url);

bool UninstallAll();

bool Uninstall(
    bool system_level,
    installer::InstallationValidator::InstallationType type);

bool Uninstall(
    bool system_level,
    BrowserDistribution::Type product);

bool ValidateInstall(
     bool system_level,
     installer::InstallationValidator::InstallationType expected,
     const std::string& version);

bool RunAndWaitForCommandToFinish(CommandLine command);

}  

#endif  

