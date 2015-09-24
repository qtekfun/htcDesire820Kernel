// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROCESS_SINGLETON_MODAL_DIALOG_LOCK_H_
#define CHROME_BROWSER_PROCESS_SINGLETON_MODAL_DIALOG_LOCK_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "chrome/browser/process_singleton.h"
#include "ui/gfx/native_widget_types.h"

class CommandLine;

namespace base {
class FilePath;
}

class ProcessSingletonModalDialogLock {
 public:
  typedef base::Callback<void(gfx::NativeWindow)> SetForegroundWindowHandler;
  explicit ProcessSingletonModalDialogLock(
      const ProcessSingleton::NotificationCallback& original_callback);

  ProcessSingletonModalDialogLock(
      const ProcessSingleton::NotificationCallback& original_callback,
      const SetForegroundWindowHandler& set_foreground_window_handler);

  ~ProcessSingletonModalDialogLock();

  
  
  void SetActiveModalDialog(gfx::NativeWindow active_dialog);

  
  
  
  ProcessSingleton::NotificationCallback AsNotificationCallback();

 private:
  bool NotificationCallbackImpl(const CommandLine& command_line,
                                const base::FilePath& current_directory);

  gfx::NativeWindow active_dialog_;
  ProcessSingleton::NotificationCallback original_callback_;
  SetForegroundWindowHandler set_foreground_window_handler_;

  DISALLOW_COPY_AND_ASSIGN(ProcessSingletonModalDialogLock);
};

#endif  
