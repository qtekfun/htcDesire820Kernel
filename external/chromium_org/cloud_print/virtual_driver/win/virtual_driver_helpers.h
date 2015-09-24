// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CLOUD_PRINT_VIRTUAL_DRIVER_WIN_VIRTUAL_DRIVER_HELPERS_H_
#define CLOUD_PRINT_VIRTUAL_DRIVER_WIN_VIRTUAL_DRIVER_HELPERS_H_

#include <windows.h>

#include "base/strings/string16.h"

namespace base {
class FilePath;
}

namespace cloud_print {

bool IsSystem64Bit();

void DisplayWindowsMessage(HWND hwnd, HRESULT hr,
                           const base::string16 &caption);

base::string16 GetPortMonitorDllName();

HRESULT GetPrinterDriverDir(base::FilePath* path);

}  

#endif  



