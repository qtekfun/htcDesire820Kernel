// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROCESS_SINGLETON_H_
#define CHROME_BROWSER_PROCESS_SINGLETON_H_

#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#endif  

#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/process/process.h"
#include "base/threading/non_thread_safe.h"
#include "ui/gfx/native_widget_types.h"

#if defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(OS_ANDROID)
#include "base/files/scoped_temp_dir.h"
#endif

#if defined(OS_WIN)
#include "base/win/message_window.h"
#endif  

class CommandLine;


class ProcessSingleton : public base::NonThreadSafe {
 public:
  enum NotifyResult {
    PROCESS_NONE,
    PROCESS_NOTIFIED,
    PROFILE_IN_USE,
    LOCK_ERROR,
    NUM_NOTIFY_RESULTS,
  };

  
  
  
  
  
  typedef base::Callback<bool(
      const CommandLine& command_line,
      const base::FilePath& current_directory)> NotificationCallback;

  ProcessSingleton(const base::FilePath& user_data_dir,
                   const NotificationCallback& notification_callback);
  ~ProcessSingleton();

  
  
  
  
  
  
  
  NotifyResult NotifyOtherProcessOrCreate();

  
  
  
  
  
  
  bool Create();

  
  void Cleanup();

#if defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(OS_ANDROID)
  static void DisablePromptForTesting();
#endif

 protected:
  
  
  
  
  NotifyResult NotifyOtherProcess();

#if defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(OS_ANDROID)
  
  
  NotifyResult NotifyOtherProcessWithTimeout(const CommandLine& command_line,
                                             int timeout_seconds,
                                             bool kill_unresponsive);
  NotifyResult NotifyOtherProcessWithTimeoutOrCreate(
      const CommandLine& command_line,
      int timeout_seconds);
  void OverrideCurrentPidForTesting(base::ProcessId pid);
  void OverrideKillCallbackForTesting(
      const base::Callback<void(int)>& callback);
#endif

 private:
#if !defined(OS_MACOSX)
  
  
  static const int kTimeoutInSeconds = 20;
#endif

  NotificationCallback notification_callback_;  

#if defined(OS_WIN)
  bool EscapeVirtualization(const base::FilePath& user_data_dir);

  HWND remote_window_;  
  base::win::MessageWindow window_;  
  bool is_virtualized_;  
  HANDLE lock_file_;
  base::FilePath user_data_dir_;
#elif defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(OS_ANDROID)
  
  
  
  bool IsSameChromeInstance(pid_t pid);

  
  
  
  
  
  bool KillProcessByLockPath();

  
  void KillProcess(int pid);

  
  base::ProcessId current_pid_;

  
  
  base::Callback<void(int)> kill_callback_;

  
  base::FilePath socket_path_;

  
  base::FilePath lock_path_;

  
  base::FilePath cookie_path_;

  
  base::ScopedTempDir socket_dir_;

  
  
  class LinuxWatcher;
  scoped_refptr<LinuxWatcher> watcher_;
#elif defined(OS_MACOSX)
  
  base::FilePath lock_path_;

  
  
  
  int lock_fd_;
#endif

  DISALLOW_COPY_AND_ASSIGN(ProcessSingleton);
};

#endif  
