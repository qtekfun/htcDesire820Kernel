// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_CHROME_FRAME_HELPER_UTIL_H_
#define CHROME_FRAME_CHROME_FRAME_HELPER_UTIL_H_

#include <windows.h>

bool UtilIsWebBrowserWindow(HWND window_to_check);

HRESULT UtilGetWebBrowserObjectFromWindow(HWND window,
                                          REFIID iid,
                                          void** web_browser_object);

bool IsWindowOfClass(HWND hwnd_to_match, const wchar_t* window_class);

bool IsNamedProcess(const wchar_t* process_name);

bool IsNamedWindow(HWND window, const wchar_t* window_name);

HWND RecurseFindWindow(HWND parent,
                       const wchar_t* class_name,
                       const wchar_t* window_name,
                       DWORD thread_id_to_match,
                       DWORD process_id_to_match);

LONG ReadValue(HKEY key,
               const wchar_t* value_name,
               size_t value_size,
               wchar_t* value);


bool IsBHOLoadingPolicyRegistered();

bool IsSystemLevelChromeFrameInstalled();

#endif  
