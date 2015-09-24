// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_MEDIA_FILE_VALIDATOR_FACTORY_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_MEDIA_FILE_VALIDATOR_FACTORY_H_

#include "base/basictypes.h"
#include "webkit/browser/fileapi/copy_or_move_file_validator.h"

namespace base {
class FilePath;
}

namespace fileapi {
class FileSystemURL;
}

class MediaFileValidatorFactory
    : public fileapi::CopyOrMoveFileValidatorFactory {
 public:
  MediaFileValidatorFactory();
  virtual ~MediaFileValidatorFactory();

  
  virtual fileapi::CopyOrMoveFileValidator* CreateCopyOrMoveFileValidator(
            const fileapi::FileSystemURL& src,
            const base::FilePath& platform_path) OVERRIDE;
 private:
  DISALLOW_COPY_AND_ASSIGN(MediaFileValidatorFactory);
};

#endif  
