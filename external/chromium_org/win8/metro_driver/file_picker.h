// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CHROME_BROWSER_UI_METRO_DRIVER_FILE_PICKER_H_
#define CHROME_BROWSER_UI_METRO_DRIVER_FILE_PICKER_H_

#include <commdlg.h>

extern "C" __declspec(dllexport)
BOOL MetroGetOpenFileName(OPENFILENAME* open_file_name);

extern "C" __declspec(dllexport)
BOOL MetroGetSaveFileName(OPENFILENAME* open_file_name);

#endif  

