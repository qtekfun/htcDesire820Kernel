// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_DIALOG_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_DIALOG_H_

#include <vector>
#include "chrome/browser/chromeos/extensions/file_manager/private_api_base.h"

namespace ui {
struct SelectedFileInfo;
}

namespace extensions {

class FileBrowserPrivateCancelDialogFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.cancelDialog",
                             FILEBROWSERPRIVATE_CANCELDIALOG)

 protected:
  virtual ~FileBrowserPrivateCancelDialogFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class FileBrowserPrivateSelectFileFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.selectFile",
                             FILEBROWSERPRIVATE_SELECTFILE)

 protected:
  virtual ~FileBrowserPrivateSelectFileFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

 private:
  
  void GetSelectedFileInfoResponse(
      int index,
      const std::vector<ui::SelectedFileInfo>& files);
};

class FileBrowserPrivateSelectFilesFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.selectFiles",
                             FILEBROWSERPRIVATE_SELECTFILES)

 protected:
  virtual ~FileBrowserPrivateSelectFilesFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

 private:
  
  void GetSelectedFileInfoResponse(
      const std::vector<ui::SelectedFileInfo>& files);
};

}  

#endif  
