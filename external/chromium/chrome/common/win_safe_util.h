// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_WIN_SAFE_UTIL_H__
#define CHROME_COMMON_WIN_SAFE_UTIL_H__
#pragma once

#include <string>
#include <windows.h>

class FilePath;

namespace win_util {

bool SaferOpenItemViaShell(HWND hwnd, const std::wstring& window_title,
                           const FilePath& full_path,
                           const std::wstring& source_url);

bool SetInternetZoneIdentifier(const FilePath& full_path);

}  

#endif  
