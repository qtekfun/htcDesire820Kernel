// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_TESTS_VALIDATION_TESTS_COMMANDS_H__
#define SANDBOX_TESTS_VALIDATION_TESTS_COMMANDS_H__

#include <windows.h>

#include "base/strings/string16.h"

namespace sandbox {

int TestValidWindow(HWND window);

int TestOpenProcess(DWORD process_id, DWORD access_mask);

int TestOpenThread(DWORD thread_id);

int TestOpenReadFile(const base::string16& path);

int TestOpenWriteFile(const base::string16& path);

int TestOpenKey(HKEY base_key, base::string16 subkey);

int TestOpenInputDesktop();

int TestSwitchDesktop();

int TestOpenAlternateDesktop(wchar_t *desktop_name);

int TestEnumAlternateWinsta();

}  

#endif  
