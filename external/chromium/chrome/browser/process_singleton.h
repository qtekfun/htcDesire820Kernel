// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROCESS_SINGLETON_H_
#define CHROME_BROWSER_PROCESS_SINGLETON_H_
#pragma once

#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#endif  

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/threading/non_thread_safe.h"
#include "ui/gfx/native_widget_types.h"

#if defined(OS_POSIX)
#include "base/file_path.h"
#endif  

#if defined(USE_X11)
#include "base/memory/scoped_temp_dir.h"
#endif  

class CommandLine;
class FilePath;


class ProcessSingleton : public base::NonThreadSafe {
 public:
  enum NotifyResult {
    PROCESS_NONE,
    PROCESS_NOTIFIED,
    PROFILE_IN_USE,
    LOCK_ERROR,
  };

  explicit ProcessSingleton(const FilePath& user_data_dir);
  ~ProcessSingleton();

  
  
  
  
  
  
  
  
  NotifyResult NotifyOtherProcess();

  
  
  
  NotifyResult NotifyOtherProcessOrCreate();

#if defined(OS_LINUX)
  
  
  NotifyResult NotifyOtherProcessWithTimeout(const CommandLine& command_line,
                                             int timeout_seconds,
                                             bool kill_unresponsive);
  NotifyResult NotifyOtherProcessWithTimeoutOrCreate(
      const CommandLine& command_line,
      int timeout_seconds);
#endif  

#if defined(OS_WIN)
  
  
  
  
  
  bool FoundOtherProcessWindow() const {
      return (NULL != remote_window_);
  }
#endif  

  
  
  
  bool Create();

  
  void Cleanup();

  
  
  
  void Lock(gfx::NativeWindow foreground_window) {
    DCHECK(CalledOnValidThread());
    locked_ = true;
    foreground_window_ = foreground_window;
  }

  
  void Unlock() {
    DCHECK(CalledOnValidThread());
    locked_ = false;
    foreground_window_ = NULL;
  }

  bool locked() {
    DCHECK(CalledOnValidThread());
    return locked_;
  }

 private:
#if !defined(OS_MACOSX)
  
  
  static const int kTimeoutInSeconds = 20;
#endif

  bool locked_;
  gfx::NativeWindow foreground_window_;

#if defined(OS_WIN)
  
  LRESULT OnCopyData(HWND hwnd, const COPYDATASTRUCT* cds);

  LRESULT CALLBACK WndProc(HWND hwnd,
                           UINT message,
                           WPARAM wparam,
                           LPARAM lparam);

  static LRESULT CALLBACK WndProcStatic(HWND hwnd,
                                        UINT message,
                                        WPARAM wparam,
                                        LPARAM lparam) {
    ProcessSingleton* msg_wnd = reinterpret_cast<ProcessSingleton*>(
        GetWindowLongPtr(hwnd, GWLP_USERDATA));
    return msg_wnd->WndProc(hwnd, message, wparam, lparam);
  }

  HWND remote_window_;  
  HWND window_;  
#elif defined(USE_X11)
  
  FilePath socket_path_;

  
  FilePath lock_path_;

  
  FilePath cookie_path_;

  
  ScopedTempDir socket_dir_;

  
  
  class LinuxWatcher;
  scoped_refptr<LinuxWatcher> watcher_;
#elif defined(OS_MACOSX)
  
  FilePath lock_path_;

  
  
  
  int lock_fd_;
#endif

  DISALLOW_COPY_AND_ASSIGN(ProcessSingleton);
};

#endif  
