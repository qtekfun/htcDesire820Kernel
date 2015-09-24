// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_OPERATION_OBSERVER_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_OPERATION_OBSERVER_H_

namespace base {
class FilePath;
}

namespace drive {
namespace file_system {

class OperationObserver {
 public:
  
  
  
  virtual void OnDirectoryChangedByOperation(
      const base::FilePath& directory_path) = 0;

  
  virtual void OnCacheFileUploadNeededByOperation(
      const std::string& local_id) = 0;

  
  virtual void OnEntryUpdatedByOperation(const std::string& local_id) {}
};

}  
}  

#endif  
