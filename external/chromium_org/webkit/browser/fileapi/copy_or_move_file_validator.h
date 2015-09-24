// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_COPY_OR_MOVE_FILE_VALIDATOR_H_
#define WEBKIT_BROWSER_FILEAPI_COPY_OR_MOVE_FILE_VALIDATOR_H_

#include "base/callback.h"
#include "base/platform_file.h"
#include "webkit/browser/webkit_storage_browser_export.h"

namespace base {
class FilePath;
}

namespace fileapi {

class FileSystemURL;

class WEBKIT_STORAGE_BROWSER_EXPORT CopyOrMoveFileValidator {
 public:
  
  
  typedef base::Callback<void(base::PlatformFileError result)> ResultCallback;

  virtual ~CopyOrMoveFileValidator() {}

  
  
  virtual void StartPreWriteValidation(
      const ResultCallback& result_callback) = 0;

  
  
  virtual void StartPostWriteValidation(
      const base::FilePath& dest_platform_path,
      const ResultCallback& result_callback) = 0;
};

class CopyOrMoveFileValidatorFactory {
 public:
  virtual ~CopyOrMoveFileValidatorFactory() {}

  
  
  
  virtual CopyOrMoveFileValidator* CreateCopyOrMoveFileValidator(
      const FileSystemURL& src_url,
      const base::FilePath& platform_path) = 0;
};

}  

#endif  
