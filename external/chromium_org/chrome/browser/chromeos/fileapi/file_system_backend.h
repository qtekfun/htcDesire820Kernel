// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FILEAPI_FILE_SYSTEM_BACKEND_H_
#define CHROME_BROWSER_CHROMEOS_FILEAPI_FILE_SYSTEM_BACKEND_H_

#include <map>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "webkit/browser/fileapi/file_system_backend.h"
#include "webkit/browser/quota/special_storage_policy.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/fileapi/file_system_types.h"

namespace fileapi {
class CopyOrMoveFileValidatorFactory;
class ExternalMountPoints;
class FileSystemFileUtil;
class FileSystemURL;
class IsolatedContext;
}

namespace chromeos {

class FileSystemBackendDelegate;
class FileAccessPermissions;

class FileSystemBackend : public fileapi::ExternalFileSystemBackend {
 public:
  using fileapi::FileSystemBackend::OpenFileSystemCallback;

  
  
  
  
  FileSystemBackend(
      FileSystemBackendDelegate* drive_delegate,
      scoped_refptr<quota::SpecialStoragePolicy> special_storage_policy,
      scoped_refptr<fileapi::ExternalMountPoints> mount_points,
      fileapi::ExternalMountPoints* system_mount_points);
  virtual ~FileSystemBackend();

  
  
  void AddSystemMountPoints();

  
  
  
  static bool CanHandleURL(const fileapi::FileSystemURL& url);

  
  virtual bool CanHandleType(fileapi::FileSystemType type) const OVERRIDE;
  virtual void Initialize(fileapi::FileSystemContext* context) OVERRIDE;
  virtual void OpenFileSystem(
      const GURL& origin_url,
      fileapi::FileSystemType type,
      fileapi::OpenFileSystemMode mode,
      const OpenFileSystemCallback& callback) OVERRIDE;
  virtual fileapi::AsyncFileUtil* GetAsyncFileUtil(
      fileapi::FileSystemType type) OVERRIDE;
  virtual fileapi::CopyOrMoveFileValidatorFactory*
      GetCopyOrMoveFileValidatorFactory(
          fileapi::FileSystemType type,
          base::PlatformFileError* error_code) OVERRIDE;
  virtual fileapi::FileSystemOperation* CreateFileSystemOperation(
      const fileapi::FileSystemURL& url,
      fileapi::FileSystemContext* context,
      base::PlatformFileError* error_code) const OVERRIDE;
  virtual scoped_ptr<webkit_blob::FileStreamReader> CreateFileStreamReader(
      const fileapi::FileSystemURL& path,
      int64 offset,
      const base::Time& expected_modification_time,
      fileapi::FileSystemContext* context) const OVERRIDE;
  virtual scoped_ptr<fileapi::FileStreamWriter> CreateFileStreamWriter(
      const fileapi::FileSystemURL& url,
      int64 offset,
      fileapi::FileSystemContext* context) const OVERRIDE;
  virtual fileapi::FileSystemQuotaUtil* GetQuotaUtil() OVERRIDE;

  
  virtual bool IsAccessAllowed(const fileapi::FileSystemURL& url)
      const OVERRIDE;
  virtual std::vector<base::FilePath> GetRootDirectories() const OVERRIDE;
  virtual void GrantFullAccessToExtension(
      const std::string& extension_id) OVERRIDE;
  virtual void GrantFileAccessToExtension(
      const std::string& extension_id,
      const base::FilePath& virtual_path) OVERRIDE;
  virtual void RevokeAccessForExtension(
      const std::string& extension_id) OVERRIDE;
  virtual bool GetVirtualPath(const base::FilePath& filesystem_path,
                              base::FilePath* virtual_path) OVERRIDE;

 private:
  scoped_refptr<quota::SpecialStoragePolicy> special_storage_policy_;
  scoped_ptr<FileAccessPermissions> file_access_permissions_;
  scoped_ptr<fileapi::AsyncFileUtil> local_file_util_;

  
  scoped_ptr<FileSystemBackendDelegate> drive_delegate_;

  
  
  
  
  
  
  
  
  
  
  
  scoped_refptr<fileapi::ExternalMountPoints> mount_points_;

  
  
  fileapi::ExternalMountPoints* system_mount_points_;

  DISALLOW_COPY_AND_ASSIGN(FileSystemBackend);
};

}  

#endif  
