// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEB_APPLICATIONS_WEB_APP_H_
#define CHROME_BROWSER_WEB_APPLICATIONS_WEB_APP_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/strings/string16.h"
#include "build/build_config.h"
#include "chrome/browser/shell_integration.h"
#include "chrome/common/web_application_info.h"

namespace extensions {
class Extension;
}

namespace gfx {
class ImageFamily;
}

namespace web_app {

enum ShortcutCreationReason {
  SHORTCUT_CREATION_BY_USER,
  SHORTCUT_CREATION_AUTOMATED,
};

base::FilePath GetWebAppDataDirectory(const base::FilePath& profile_path,
                                      const std::string& extension_id,
                                      const GURL& url);

base::FilePath GetWebAppDataDirectory(const base::FilePath& profile_path,
                                      const extensions::Extension& extension);

std::string GenerateApplicationNameFromInfo(
    const ShellIntegration::ShortcutInfo& shortcut_info);

std::string GenerateApplicationNameFromURL(const GURL& url);

std::string GenerateApplicationNameFromExtensionId(const std::string& id);

std::string GetExtensionIdFromApplicationName(const std::string& app_name);

void CreateShortcuts(
    const ShellIntegration::ShortcutInfo& shortcut_info,
    const ShellIntegration::ShortcutLocations& creation_locations,
    ShortcutCreationReason creation_reason);

void DeleteAllShortcuts(const ShellIntegration::ShortcutInfo& shortcut_info);

void UpdateAllShortcuts(const base::string16& old_app_title,
                        const ShellIntegration::ShortcutInfo& shortcut_info);

bool CreateShortcutsOnFileThread(
    const ShellIntegration::ShortcutInfo& shortcut_info,
    const ShellIntegration::ShortcutLocations& creation_locations,
    ShortcutCreationReason creation_reason);

bool IsValidUrl(const GURL& url);

#if defined(TOOLKIT_VIEWS)
typedef std::vector<WebApplicationInfo::IconInfo> IconInfoList;
void GetIconsInfo(const WebApplicationInfo& app_info,
                  IconInfoList* icons);
#endif

#if defined(OS_LINUX)
std::string GetWMClassFromAppName(std::string app_name);
#endif

namespace internals {

#if defined(OS_WIN)
std::vector<base::FilePath> GetShortcutPaths(
    const ShellIntegration::ShortcutLocations& creation_locations);
#endif

bool CreatePlatformShortcuts(
    const base::FilePath& shortcut_data_path,
    const ShellIntegration::ShortcutInfo& shortcut_info,
    const ShellIntegration::ShortcutLocations& creation_locations,
    ShortcutCreationReason creation_reason);

void DeletePlatformShortcuts(
    const base::FilePath& shortcut_data_path,
    const ShellIntegration::ShortcutInfo& shortcut_info);

void UpdatePlatformShortcuts(
    const base::FilePath& shortcut_data_path,
    const base::string16& old_app_title,
    const ShellIntegration::ShortcutInfo& shortcut_info);

void DeleteAllShortcutsForProfile(const base::FilePath& profile_path);

base::FilePath GetSanitizedFileName(const base::string16& name);

}  

}  

#endif  
