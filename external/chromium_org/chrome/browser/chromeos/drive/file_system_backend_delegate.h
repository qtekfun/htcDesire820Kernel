// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_BACKEND_DELEGATE_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_BACKEND_DELEGATE_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/fileapi/file_system_backend_delegate.h"

namespace content {
class BrowserContext;
}  

namespace fileapi {
class AsyncFileUtil;
}  

namespace drive {

class FileSystemBackendDelegate : public chromeos::FileSystemBackendDelegate {
 public:
  
  explicit FileSystemBackendDelegate(content::BrowserContext* browser_context);
  virtual ~FileSystemBackendDelegate();

  
  virtual fileapi::AsyncFileUtil* GetAsyncFileUtil(
      fileapi::FileSystemType type) OVERRIDE;
  virtual scoped_ptr<webkit_blob::FileStreamReader> CreateFileStreamReader(
      const fileapi::FileSystemURL& url,
      int64 offset,
      const base::Time& expected_modification_time,
      fileapi::FileSystemContext* context) OVERRIDE;
  virtual scoped_ptr<fileapi::FileStreamWriter> CreateFileStreamWriter(
      const fileapi::FileSystemURL& url,
      int64 offset,
      fileapi::FileSystemContext* context) OVERRIDE;

 private:
  
  void* profile_id_;
  scoped_ptr<fileapi::AsyncFileUtil> async_file_util_;

  DISALLOW_COPY_AND_ASSIGN(FileSystemBackendDelegate);
};

}  

#endif  
