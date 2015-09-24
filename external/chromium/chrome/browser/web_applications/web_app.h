// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEB_APPLICATIONS_WEB_APP_H_
#define CHROME_BROWSER_WEB_APPLICATIONS_WEB_APP_H_
#pragma once

#include <vector>

#include "base/callback.h"
#include "base/file_path.h"
#include "build/build_config.h"
#include "chrome/browser/shell_integration.h"
#include "chrome/common/web_apps.h"

class Profile;

namespace web_app {

std::string GenerateApplicationNameFromInfo(
    const ShellIntegration::ShortcutInfo& shortcut_info);

std::string GenerateApplicationNameFromURL(const GURL& url);

std::string GenerateApplicationNameFromExtensionId(const std::string& id);

typedef Callback1<bool>::Type CreateShortcutCallback;

void CreateShortcut(
    const FilePath& profile_path,
    const ShellIntegration::ShortcutInfo& shortcut_info,
    CreateShortcutCallback* callback);

bool IsValidUrl(const GURL& url);

FilePath GetDataDir(const FilePath& profile_path);

#if defined(TOOLKIT_VIEWS)
typedef std::vector<WebApplicationInfo::IconInfo> IconInfoList;
void GetIconsInfo(const WebApplicationInfo& app_info,
                  IconInfoList* icons);
#endif

#if defined(TOOLKIT_USES_GTK)
std::string GetWMClassFromAppName(std::string app_name);
#endif

namespace internals {

#if defined(OS_WIN)
FilePath GetSanitizedFileName(const string16& name);

bool CheckAndSaveIcon(const FilePath& icon_file, const SkBitmap& image);
#endif

FilePath GetWebAppDataDirectory(const FilePath& root_dir,
                                const ShellIntegration::ShortcutInfo& info);
}  

}  

#endif  
