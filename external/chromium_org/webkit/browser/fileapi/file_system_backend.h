// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_FILE_SYSTEM_BACKEND_H_
#define WEBKIT_BROWSER_FILEAPI_FILE_SYSTEM_BACKEND_H_

#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/platform_file.h"
#include "webkit/browser/fileapi/file_permission_policy.h"
#include "webkit/browser/fileapi/open_file_system_mode.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/fileapi/file_system_types.h"

class GURL;

namespace webkit_blob {
class FileStreamReader;
}

namespace fileapi {

class AsyncFileUtil;
class CopyOrMoveFileValidatorFactory;
class FileSystemURL;
class FileStreamWriter;
class FileSystemContext;
class FileSystemFileUtil;
class FileSystemOperation;
class FileSystemQuotaUtil;

class WEBKIT_STORAGE_BROWSER_EXPORT FileSystemBackend {
 public:
  
  typedef base::Callback<void(const GURL& root_url,
                              const std::string& name,
                              base::PlatformFileError error)>
      OpenFileSystemCallback;
  virtual ~FileSystemBackend() {}

  
  
  virtual bool CanHandleType(FileSystemType type) const = 0;

  
  
  
  virtual void Initialize(FileSystemContext* context) = 0;

  
  
  
  
  
  
  virtual void OpenFileSystem(
      const GURL& origin_url,
      FileSystemType type,
      OpenFileSystemMode mode,
      const OpenFileSystemCallback& callback) = 0;

  
  virtual AsyncFileUtil* GetAsyncFileUtil(FileSystemType type) = 0;

  
  
  
  virtual CopyOrMoveFileValidatorFactory* GetCopyOrMoveFileValidatorFactory(
      FileSystemType type, base::PlatformFileError* error_code) = 0;

  
  
  
  
  
  
  virtual FileSystemOperation* CreateFileSystemOperation(
      const FileSystemURL& url,
      FileSystemContext* context,
      base::PlatformFileError* error_code) const = 0;

  
  
  
  
  
  
  
  
  virtual scoped_ptr<webkit_blob::FileStreamReader> CreateFileStreamReader(
      const FileSystemURL& url,
      int64 offset,
      const base::Time& expected_modification_time,
      FileSystemContext* context) const = 0;

  
  
  
  
  virtual scoped_ptr<FileStreamWriter> CreateFileStreamWriter(
      const FileSystemURL& url,
      int64 offset,
      FileSystemContext* context) const = 0;

  
  
  virtual FileSystemQuotaUtil* GetQuotaUtil() = 0;
};

class ExternalFileSystemBackend : public FileSystemBackend {
 public:
  
  
  
  virtual bool IsAccessAllowed(const fileapi::FileSystemURL& url) const = 0;
  
  
  
  virtual std::vector<base::FilePath> GetRootDirectories() const = 0;
  
  
  virtual void GrantFullAccessToExtension(const std::string& extension_id) = 0;
  
  virtual void GrantFileAccessToExtension(
      const std::string& extension_id,
      const base::FilePath& virtual_path) = 0;
  
  virtual void RevokeAccessForExtension(
        const std::string& extension_id) = 0;
  
  
  virtual bool GetVirtualPath(const base::FilePath& file_system_path,
                              base::FilePath* virtual_path) = 0;
};

}  

#endif  
