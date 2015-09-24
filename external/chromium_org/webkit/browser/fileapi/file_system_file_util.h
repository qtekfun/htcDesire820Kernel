// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_FILE_SYSTEM_FILE_UTIL_H_
#define WEBKIT_BROWSER_FILEAPI_FILE_SYSTEM_FILE_UTIL_H_

#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/platform_file.h"
#include "webkit/browser/fileapi/file_system_operation.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/blob/scoped_file.h"

namespace base {
class Time;
}

namespace fileapi {

class FileSystemOperationContext;
class FileSystemURL;

class WEBKIT_STORAGE_BROWSER_EXPORT FileSystemFileUtil {
 public:
  typedef FileSystemOperation::CopyOrMoveOption CopyOrMoveOption;

  
  class WEBKIT_STORAGE_BROWSER_EXPORT AbstractFileEnumerator {
   public:
    virtual ~AbstractFileEnumerator() {}

    
    virtual base::FilePath Next() = 0;

    
    
    
    
    virtual int64 Size() = 0;
    virtual base::Time LastModifiedTime() = 0;
    virtual bool IsDirectory() = 0;
  };

  class WEBKIT_STORAGE_BROWSER_EXPORT EmptyFileEnumerator
      : public AbstractFileEnumerator {
    virtual base::FilePath Next() OVERRIDE;
    virtual int64 Size() OVERRIDE;
    virtual base::Time LastModifiedTime() OVERRIDE;
    virtual bool IsDirectory() OVERRIDE;
  };

  virtual ~FileSystemFileUtil() {}

  
  
  
  virtual base::PlatformFileError CreateOrOpen(
      FileSystemOperationContext* context,
      const FileSystemURL& url,
      int file_flags,
      base::PlatformFile* file_handle,
      bool* created) = 0;

  
  
  virtual base::PlatformFileError Close(
      FileSystemOperationContext* context,
      base::PlatformFile file) = 0;

  
  
  
  virtual base::PlatformFileError EnsureFileExists(
      FileSystemOperationContext* context,
      const FileSystemURL& url, bool* created) = 0;

  
  
  virtual base::PlatformFileError CreateDirectory(
      FileSystemOperationContext* context,
      const FileSystemURL& url,
      bool exclusive,
      bool recursive) = 0;

  
  
  virtual base::PlatformFileError GetFileInfo(
      FileSystemOperationContext* context,
      const FileSystemURL& url,
      base::PlatformFileInfo* file_info,
      base::FilePath* platform_path) = 0;

  
  
  
  
  
  
  
  virtual scoped_ptr<AbstractFileEnumerator> CreateFileEnumerator(
      FileSystemOperationContext* context,
      const FileSystemURL& root_url) = 0;

  
  
  
  virtual base::PlatformFileError GetLocalFilePath(
      FileSystemOperationContext* context,
      const FileSystemURL& file_system_url,
      base::FilePath* local_file_path) = 0;

  
  
  virtual base::PlatformFileError Touch(
      FileSystemOperationContext* context,
      const FileSystemURL& url,
      const base::Time& last_access_time,
      const base::Time& last_modified_time) = 0;

  
  
  virtual base::PlatformFileError Truncate(
      FileSystemOperationContext* context,
      const FileSystemURL& url,
      int64 length) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual base::PlatformFileError CopyOrMoveFile(
      FileSystemOperationContext* context,
      const FileSystemURL& src_url,
      const FileSystemURL& dest_url,
      CopyOrMoveOption option,
      bool copy) = 0;

  
  
  
  virtual base::PlatformFileError CopyInForeignFile(
        FileSystemOperationContext* context,
        const base::FilePath& src_file_path,
        const FileSystemURL& dest_url) = 0;

  
  
  virtual base::PlatformFileError DeleteFile(
      FileSystemOperationContext* context,
      const FileSystemURL& url) = 0;

  
  
  virtual base::PlatformFileError DeleteDirectory(
      FileSystemOperationContext* context,
      const FileSystemURL& url) = 0;

  
  
  
  
  
  virtual webkit_blob::ScopedFile CreateSnapshotFile(
      FileSystemOperationContext* context,
      const FileSystemURL& url,
      base::PlatformFileError* error,
      base::PlatformFileInfo* file_info,
      base::FilePath* platform_path) = 0;

 protected:
  FileSystemFileUtil() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(FileSystemFileUtil);
};

}  

#endif  
