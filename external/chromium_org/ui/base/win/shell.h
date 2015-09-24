// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_WIN_SHELL_H_
#define UI_BASE_WIN_SHELL_H_

#include <windows.h>

#include "base/strings/string16.h"
#include "ui/base/ui_export.h"

namespace base {
class FilePath;
}

namespace ui {
namespace win {

UI_EXPORT bool OpenItemViaShell(const base::FilePath& full_path);

UI_EXPORT bool OpenItemViaShellNoZoneCheck(const base::FilePath& full_path);

UI_EXPORT bool OpenAnyViaShell(const string16& full_path,
                               const string16& directory,
                               const string16& args,
                               DWORD mask);

bool OpenItemWithExternalApp(const string16& full_path);

UI_EXPORT bool PreventWindowFromPinning(HWND hwnd);

UI_EXPORT void SetAppDetailsForWindow(const string16& app_id,
                                      const string16& app_icon,
                                      const string16& relaunch_command,
                                      const string16& relaunch_display_name,
                                      HWND hwnd);

UI_EXPORT void SetAppIdForWindow(const string16& app_id, HWND hwnd);

UI_EXPORT void SetAppIconForWindow(const string16& app_icon, HWND hwnd);

UI_EXPORT void SetRelaunchDetailsForWindow(const string16& relaunch_command,
                                           const string16& display_name,
                                           HWND hwnd);

UI_EXPORT bool IsAeroGlassEnabled();

}  
}  

#endif  
