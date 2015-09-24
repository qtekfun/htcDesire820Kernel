// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_UPDATE_LAUNCHER_H_
#define CHROME_FRAME_UPDATE_LAUNCHER_H_

#include <string>

typedef unsigned long DWORD;  

namespace update_launcher {

std::wstring GetUpdateCommandFromArguments(const wchar_t* command_line);

DWORD LaunchUpdateCommand(const std::wstring& command);

}  

#endif  
