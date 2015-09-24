// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FILE_MANAGER_FILE_WATCHER_H_
#define CHROME_BROWSER_CHROMEOS_FILE_MANAGER_FILE_WATCHER_H_

#include <map>
#include <string>

#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/files/file_path_watcher.h"
#include "base/memory/weak_ptr.h"

namespace file_manager {

class FileWatcher {
 public:
  typedef base::Callback<void(bool success)> BoolCallback;

  
  explicit FileWatcher(const base::FilePath& virtual_path);

  ~FileWatcher();

  
  
  
  
  
  
  
  void AddExtension(const std::string& extension_id);

  
  
  
  void RemoveExtension(const std::string& extension_id);

  
  
  std::vector<std::string> GetExtensionIds() const;

  
  const base::FilePath& virtual_path() const { return virtual_path_; }

  
  
  
  
  
  void WatchLocalFile(
      const base::FilePath& local_path,
      const base::FilePathWatcher::Callback& file_watcher_callback,
      const BoolCallback& callback);

 private:
  
  
  void OnWatcherStarted(const BoolCallback& callback,
                        base::FilePathWatcher* file_path_watcher);

  base::FilePathWatcher* local_file_watcher_;
  base::FilePath virtual_path_;
  
  
  typedef std::map<std::string, int> ExtensionCountMap;
  ExtensionCountMap extensions_;

  
  
  base::WeakPtrFactory<FileWatcher> weak_ptr_factory_;
};

}  

#endif  
