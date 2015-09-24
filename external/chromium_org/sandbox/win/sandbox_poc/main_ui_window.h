// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_SANDBOX_POC_MAIN_UI_WINDOW_H__
#define SANDBOX_SANDBOX_POC_MAIN_UI_WINDOW_H__

#include <string>

#include "base/basictypes.h"
#include "base/strings/string16.h"

namespace sandbox {
class BrokerServices;
enum ResultCode;
}


class MainUIWindow {
 public:
  MainUIWindow();
  ~MainUIWindow();

  
  
  
  
  
  
  unsigned int CreateMainWindowAndLoop(HINSTANCE instance,
                                       wchar_t* command_line,
                                       int show_command,
                                       sandbox::BrokerServices* broker);

 private:
  
  static const wchar_t kDefaultDll_[];

  
  static const wchar_t kDefaultEntryPoint_[];

  
  static const wchar_t kDefaultLogFile_[];

  
  
  static LRESULT CALLBACK WndProc(HWND window,
                                  UINT message_id,
                                  WPARAM wparam,
                                  LPARAM lparam);

  
  
  static INT_PTR CALLBACK SpawnTargetWndProc(HWND dialog,
                                             UINT message_id,
                                             WPARAM wparam,
                                             LPARAM lparam);

  
  
  
  static MainUIWindow* FromWindow(HWND main_window);

  
  
  static BOOL OnCreate(HWND parent_window, LPCREATESTRUCT);

  
  void OnDestroy(HWND window);

  
  void OnSize(HWND window, UINT state, int cx, int cy);

  
  void OnPaint(HWND window);

  
  void OnFileExit();

  
  void OnCommandsLaunch(HWND window);

  
  
  
  
  
  bool OnLaunchDll(HWND dialog);

  
  
  
  
  
  
  
  
  bool SpawnTarget();

  
  
  base::string16 OnShowBrowseForDllDlg(HWND owner);

  
  
  base::string16 OnShowBrowseForLogFileDlg(HWND owner);

  
  
  
  void AddDebugMessage(const wchar_t* format, ...);

  
  
  
  
  void InsertLineInListView(wchar_t* debug_message);

  
  
  static DWORD WINAPI ListenPipeThunk(void *param);

  
  
  static DWORD WINAPI WaitForTargetThunk(void *param);

  
  DWORD ListenPipe();

  
  DWORD WaitForTarget();

  
  
  sandbox::BrokerServices* broker_;

  
  PROCESS_INFORMATION target_;

  
  
  base::string16 spawn_target_;

  
  
  HINSTANCE instance_handle_;

  
  base::string16 dll_path_;

  
  base::string16 entry_point_;

  
  base::string16 log_file_;

  
  
  
  static HWND list_view_;

  
  HANDLE pipe_handle_;

  DISALLOW_COPY_AND_ASSIGN(MainUIWindow);
};

#endif  
