// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_WRITE_ON_CACHE_FILE_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_WRITE_ON_CACHE_FILE_H_

#include "base/callback_forward.h"
#include "chrome/browser/chromeos/drive/file_errors.h"

namespace base {
class FilePath;
}

namespace drive {

class FileSystemInterface;

typedef base::Callback<void (FileError, const base::FilePath& path)>
    WriteOnCacheFileCallback;

// operations. After it returns, the written content is synced to the server.
void WriteOnCacheFile(FileSystemInterface* file_system,
                      const base::FilePath& path,
                      const std::string& mime_type,
                      const WriteOnCacheFileCallback& callback);

void WriteOnCacheFileAndReply(FileSystemInterface* file_system,
                              const base::FilePath& path,
                              const std::string& mime_type,
                              const WriteOnCacheFileCallback& callback,
                              const FileOperationCallback& reply);

}  

#endif  
