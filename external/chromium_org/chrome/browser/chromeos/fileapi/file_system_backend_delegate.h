// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FILEAPI_FILE_SYSTEM_BACKEND_DELEGATE_H_
#define CHROME_BROWSER_CHROMEOS_FILEAPI_FILE_SYSTEM_BACKEND_DELEGATE_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/platform_file.h"
#include "webkit/common/fileapi/file_system_types.h"

namespace base {
class Time;
}  

namespace fileapi {
class AsyncFileUtil;
class FileSystemContext;
class FileSystemOperation;
class FileSystemURL;
class FileStreamWriter;
}  

namespace webkit_blob {
class FileStreamReader;
}  

namespace chromeos {

class FileSystemBackendDelegate {
 public:
  virtual ~FileSystemBackendDelegate() {}

  
  virtual fileapi::AsyncFileUtil* GetAsyncFileUtil(
      fileapi::FileSystemType type) = 0;

  
  virtual scoped_ptr<webkit_blob::FileStreamReader> CreateFileStreamReader(
      const fileapi::FileSystemURL& url,
      int64 offset,
      const base::Time& expected_modification_time,
      fileapi::FileSystemContext* context) = 0;

  
  virtual scoped_ptr<fileapi::FileStreamWriter> CreateFileStreamWriter(
      const fileapi::FileSystemURL& url,
      int64 offset,
      fileapi::FileSystemContext* context) = 0;
};

}  

#endif  
