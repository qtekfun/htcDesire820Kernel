// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROME_PROCESS_SINGLETON_H_
#define CHROME_BROWSER_CHROME_PROCESS_SINGLETON_H_

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "chrome/browser/process_singleton.h"
#include "chrome/browser/process_singleton_modal_dialog_lock.h"
#include "chrome/browser/process_singleton_startup_lock.h"
#include "ui/gfx/native_widget_types.h"

class ChromeProcessSingleton {
 public:
  ChromeProcessSingleton(
      const base::FilePath& user_data_dir,
      const ProcessSingleton::NotificationCallback& notification_callback);

  ChromeProcessSingleton(
      const base::FilePath& user_data_dir,
      const ProcessSingleton::NotificationCallback& notification_callback,
      const ProcessSingletonModalDialogLock::SetForegroundWindowHandler&
          set_foreground_window_handler);

  ~ChromeProcessSingleton();

  
  
  
  
  
  ProcessSingleton::NotifyResult NotifyOtherProcessOrCreate();

  
  void Cleanup();

  
  
  void SetActiveModalDialog(gfx::NativeWindow active_dialog);

  
  
  
  void Unlock();

 private:
  
  
  
  
  
  
  ProcessSingletonStartupLock startup_lock_;
  ProcessSingletonModalDialogLock modal_dialog_lock_;

  
  ProcessSingleton process_singleton_;

  DISALLOW_COPY_AND_ASSIGN(ChromeProcessSingleton);
};

#endif  
