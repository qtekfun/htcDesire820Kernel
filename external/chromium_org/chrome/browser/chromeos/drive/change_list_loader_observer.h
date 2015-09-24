// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_CHANGE_LIST_LOADER_OBSERVER_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_CHANGE_LIST_LOADER_OBSERVER_H_

namespace base {
class FilePath;
}  

namespace drive {
namespace internal {

class ChangeListLoaderObserver {
 public:
  
  
  
  virtual void OnDirectoryChanged(const base::FilePath& directory_path) {}

  
  virtual void OnLoadFromServerComplete() {}

  
  
  
  
  virtual void OnInitialLoadComplete() {}

 protected:
  virtual ~ChangeListLoaderObserver() {}
};

}  
}  

#endif  
