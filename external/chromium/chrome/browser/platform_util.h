// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PLATFORM_UTIL_H_
#define CHROME_BROWSER_PLATFORM_UTIL_H_
#pragma once

#include "base/string16.h"
#include "ui/gfx/native_widget_types.h"

class FilePath;
class GURL;

namespace platform_util {

void ShowItemInFolder(const FilePath& full_path);

void OpenItem(const FilePath& full_path);

void OpenExternal(const GURL& url);

gfx::NativeWindow GetTopLevel(gfx::NativeView view);

gfx::NativeView GetParent(gfx::NativeView view);

bool IsWindowActive(gfx::NativeWindow window);

void ActivateWindow(gfx::NativeWindow window);

bool IsVisible(gfx::NativeView view);

void SimpleErrorBox(gfx::NativeWindow parent,
                    const string16& title,
                    const string16& message);

bool SimpleYesNoBox(gfx::NativeWindow parent,
                    const string16& title,
                    const string16& message);

std::string GetVersionStringModifier();

bool CanSetAsDefaultBrowser();

}

#endif  
