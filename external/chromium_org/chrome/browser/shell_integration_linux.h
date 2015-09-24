// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SHELL_INTEGRATION_LINUX_H_
#define CHROME_BROWSER_SHELL_INTEGRATION_LINUX_H_

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "chrome/browser/shell_integration.h"
#include "url/gurl.h"

namespace base {
class Environment;
}

namespace ShellIntegrationLinux {

bool GetDataWriteLocation(base::Environment* env, base::FilePath* search_path);

std::vector<base::FilePath> GetDataSearchLocations(base::Environment* env);

std::string GetProgramClassName();

std::string GetDesktopName(base::Environment* env);

std::string GetIconName();

ShellIntegration::ShortcutLocations GetExistingShortcutLocations(
    base::Environment* env,
    const base::FilePath& profile_path,
    const std::string& extension_id);

ShellIntegration::ShortcutLocations GetExistingShortcutLocations(
    base::Environment* env,
    const base::FilePath& profile_path,
    const std::string& extension_id,
    const base::FilePath& desktop_path);

bool GetExistingShortcutContents(base::Environment* env,
                                 const base::FilePath& desktop_filename,
                                 std::string* output);

base::FilePath GetWebShortcutFilename(const GURL& url);

base::FilePath GetExtensionShortcutFilename(const base::FilePath& profile_path,
                                            const std::string& extension_id);

std::vector<base::FilePath> GetExistingProfileShortcutFilenames(
    const base::FilePath& profile_path,
    const base::FilePath& directory);

std::string GetDesktopFileContents(const base::FilePath& chrome_exe_path,
                                   const std::string& app_name,
                                   const GURL& url,
                                   const std::string& extension_id,
                                   const base::FilePath& extension_path,
                                   const base::string16& title,
                                   const std::string& icon_name,
                                   const base::FilePath& profile_path,
                                   bool no_display);

std::string GetDirectoryFileContents(const base::string16& title,
                                     const std::string& icon_name);

bool CreateDesktopShortcut(
    const ShellIntegration::ShortcutInfo& shortcut_info,
    const ShellIntegration::ShortcutLocations& creation_locations);

void DeleteDesktopShortcuts(const base::FilePath& profile_path,
                            const std::string& extension_id);

void DeleteAllDesktopShortcuts(const base::FilePath& profile_path);

}  

#endif  
