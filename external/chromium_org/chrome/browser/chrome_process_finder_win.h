// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROME_PROCESS_FINDER_WIN_H_
#define CHROME_BROWSER_CHROME_PROCESS_FINDER_WIN_H_

#include <windows.h>

namespace base {
class FilePath;
}

namespace chrome {

enum NotifyChromeResult {
  NOTIFY_SUCCESS,
  NOTIFY_FAILED,
  NOTIFY_WINDOW_HUNG,
};

HWND FindRunningChromeWindow(const base::FilePath& user_data_dir);

NotifyChromeResult AttemptToNotifyRunningChrome(HWND remote_window,
                                                bool fast_start);

}  

#endif  
