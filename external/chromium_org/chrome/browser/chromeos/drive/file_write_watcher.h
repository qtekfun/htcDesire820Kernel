// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_WRITE_WATCHER_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_WRITE_WATCHER_H_

#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/drive/file_system_util.h"

namespace base {
class FilePath;
}  

namespace drive {

namespace internal {

typedef base::Callback<void(bool)> StartWatchCallback;

class FileWriteWatcher {
 public:
  FileWriteWatcher();
  ~FileWriteWatcher();

  
  
  
  
  
  
  
  
  
  
  
  
  
  void StartWatch(const base::FilePath& path,
                  const StartWatchCallback& on_start_callback,
                  const base::Closure& on_write_callback);

  
  
  void DisableDelayForTesting();

 private:
  class FileWriteWatcherImpl;
  scoped_ptr<FileWriteWatcherImpl, util::DestroyHelper> watcher_impl_;

  DISALLOW_COPY_AND_ASSIGN(FileWriteWatcher);
};

}  
}  

#endif  
