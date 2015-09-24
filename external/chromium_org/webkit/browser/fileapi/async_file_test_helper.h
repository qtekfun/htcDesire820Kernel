// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_ASYNC_FILE_TEST_HELPER_H_
#define WEBKIT_BROWSER_FILEAPI_ASYNC_FILE_TEST_HELPER_H_

#include "base/basictypes.h"
#include "webkit/browser/fileapi/file_system_operation.h"
#include "webkit/common/fileapi/file_system_types.h"
#include "webkit/common/quota/quota_status_code.h"

namespace quota {
class QuotaManager;
}

namespace fileapi {

class FileSystemContext;
class FileSystemURL;

class AsyncFileTestHelper {
 public:
  typedef FileSystemOperation::FileEntryList FileEntryList;
  typedef FileSystemOperation::CopyProgressCallback CopyProgressCallback;

  static const int64 kDontCheckSize;

  
  static base::PlatformFileError Copy(FileSystemContext* context,
                                      const FileSystemURL& src,
                                      const FileSystemURL& dest);

  
  static base::PlatformFileError CopyWithProgress(
      FileSystemContext* context,
      const FileSystemURL& src,
      const FileSystemURL& dest,
      const CopyProgressCallback& progress_callback);

  
  static base::PlatformFileError Move(FileSystemContext* context,
                                      const FileSystemURL& src,
                                      const FileSystemURL& dest);

  
  static base::PlatformFileError Remove(FileSystemContext* context,
                                        const FileSystemURL& url,
                                        bool recursive);

  
  static base::PlatformFileError ReadDirectory(FileSystemContext* context,
                                               const FileSystemURL& url,
                                               FileEntryList* entries);

  
  static base::PlatformFileError CreateDirectory(FileSystemContext* context,
                                                 const FileSystemURL& url);

  
  static base::PlatformFileError CreateFile(FileSystemContext* context,
                                            const FileSystemURL& url);

  
  static base::PlatformFileError CreateFileWithData(
      FileSystemContext* context,
      const FileSystemURL& url,
      const char* buf,
      int buf_size);

  
  static base::PlatformFileError TruncateFile(FileSystemContext* context,
                                              const FileSystemURL& url,
                                              size_t size);

  
  static base::PlatformFileError GetMetadata(FileSystemContext* context,
                                             const FileSystemURL& url,
                                             base::PlatformFileInfo* file_info);

  
  static base::PlatformFileError GetPlatformPath(FileSystemContext* context,
                                                 const FileSystemURL& url,
                                                 base::FilePath* platform_path);

  
  
  
  static bool FileExists(FileSystemContext* context,
                         const FileSystemURL& url,
                         int64 size);

  
  static bool DirectoryExists(FileSystemContext* context,
                              const FileSystemURL& url);

  
  static quota::QuotaStatusCode GetUsageAndQuota(
      quota::QuotaManager* quota_manager,
      const GURL& origin,
      FileSystemType type,
      int64* usage,
      int64* quota);
};

}  

#endif  
