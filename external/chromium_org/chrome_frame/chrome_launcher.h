// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_CHROME_LAUNCHER_H_
#define CHROME_FRAME_CHROME_LAUNCHER_H_

#include <string>

template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

#define arraysize(array) (sizeof(ArraySizeHelper(array)))

namespace chrome_launcher {

extern const wchar_t kLauncherExeBaseName[];

bool IsValidCommandLine(const wchar_t* command_line);

bool SanitizeAndLaunchChrome(const wchar_t* command_line);

const wchar_t* GetArgumentsStart(const wchar_t* command_line);

bool GetChromeExecutablePath(std::wstring* chrome_path);

bool IsValidArgument(const std::wstring& argument);

std::wstring TrimWhiteSpace(const wchar_t* input_str);

}  

#endif  
