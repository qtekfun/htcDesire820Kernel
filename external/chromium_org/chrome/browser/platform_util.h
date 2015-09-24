// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PLATFORM_UTIL_H_
#define CHROME_BROWSER_PLATFORM_UTIL_H_

#include <string>

#include "base/strings/string16.h"
#include "ui/gfx/native_widget_types.h"

class GURL;
class Profile;

namespace base {
class FilePath;
}

namespace platform_util {

void ShowItemInFolder(Profile* profile, const base::FilePath& full_path);

void OpenItem(Profile* profile, const base::FilePath& full_path);

void OpenExternal(Profile* profile, const GURL& url);

gfx::NativeWindow GetTopLevel(gfx::NativeView view);

gfx::NativeView GetParent(gfx::NativeView view);

bool IsWindowActive(gfx::NativeWindow window);

void ActivateWindow(gfx::NativeWindow window);

bool IsVisible(gfx::NativeView view);

#if defined(OS_MACOSX)
bool IsSwipeTrackingFromScrollEventsEnabled();
#endif

}  

#endif  
