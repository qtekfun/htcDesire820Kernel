// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_MOUNT_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_MOUNT_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "chrome/browser/chromeos/extensions/file_manager/private_api_base.h"

namespace ui {
struct SelectedFileInfo;
}

namespace extensions {

class FileBrowserPrivateAddMountFunction : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.addMount",
                             FILEBROWSERPRIVATE_ADDMOUNT)

 protected:
  virtual ~FileBrowserPrivateAddMountFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

 private:
  
  
  void RunAfterMarkCacheFileAsMounted(const base::FilePath& display_name,
                                      drive::FileError error,
                                      const base::FilePath& file_path);
};

class FileBrowserPrivateRemoveMountFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.removeMount",
                             FILEBROWSERPRIVATE_REMOVEMOUNT)

 protected:
  virtual ~FileBrowserPrivateRemoveMountFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

 private:
  
  void GetSelectedFileInfoResponse(
      const std::vector<ui::SelectedFileInfo>& files);
};

class FileBrowserPrivateGetVolumeMetadataListFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.getVolumeMetadataList",
                             FILEBROWSERPRIVATE_GETVOLUMEMETADATALIST)

 protected:
  virtual ~FileBrowserPrivateGetVolumeMetadataListFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

}  

#endif  
