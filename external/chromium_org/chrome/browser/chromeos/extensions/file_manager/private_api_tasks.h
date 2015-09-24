// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_TASKS_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_TASKS_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "chrome/browser/chromeos/extensions/file_manager/private_api_base.h"

class PrefService;

namespace drive {
class DriveAppRegistry;
}

namespace extensions {

class FileBrowserPrivateExecuteTaskFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.executeTask",
                             FILEBROWSERPRIVATE_EXECUTETASK)

 protected:
  virtual ~FileBrowserPrivateExecuteTaskFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

  void OnTaskExecuted(bool success);
};

class FileBrowserPrivateGetFileTasksFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.getFileTasks",
                             FILEBROWSERPRIVATE_GETFILETASKS)

 protected:
  virtual ~FileBrowserPrivateGetFileTasksFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class FileBrowserPrivateSetDefaultTaskFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.setDefaultTask",
                             FILEBROWSERPRIVATE_SETDEFAULTTASK)

 protected:
  virtual ~FileBrowserPrivateSetDefaultTaskFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

}  

#endif  
