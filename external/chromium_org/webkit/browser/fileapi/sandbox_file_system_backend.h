// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_SANDBOX_FILE_SYSTEM_BACKEND_H_
#define WEBKIT_BROWSER_FILEAPI_SANDBOX_FILE_SYSTEM_BACKEND_H_

#include <set>
#include <string>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "webkit/browser/fileapi/file_system_backend.h"
#include "webkit/browser/fileapi/file_system_quota_util.h"
#include "webkit/browser/fileapi/sandbox_file_system_backend_delegate.h"
#include "webkit/browser/fileapi/task_runner_bound_observer_list.h"
#include "webkit/browser/quota/special_storage_policy.h"
#include "webkit/browser/webkit_storage_browser_export.h"

namespace fileapi {

class WEBKIT_STORAGE_BROWSER_EXPORT SandboxFileSystemBackend
    : public FileSystemBackend {
 public:
  explicit SandboxFileSystemBackend(SandboxFileSystemBackendDelegate* delegate);
  virtual ~SandboxFileSystemBackend();

  
  virtual bool CanHandleType(FileSystemType type) const OVERRIDE;
  virtual void Initialize(FileSystemContext* context) OVERRIDE;
  virtual void OpenFileSystem(
      const GURL& origin_url,
      FileSystemType type,
      OpenFileSystemMode mode,
      const OpenFileSystemCallback& callback) OVERRIDE;
  virtual AsyncFileUtil* GetAsyncFileUtil(FileSystemType type) OVERRIDE;
  virtual CopyOrMoveFileValidatorFactory* GetCopyOrMoveFileValidatorFactory(
      FileSystemType type,
      base::PlatformFileError* error_code) OVERRIDE;
  virtual FileSystemOperation* CreateFileSystemOperation(
      const FileSystemURL& url,
      FileSystemContext* context,
      base::PlatformFileError* error_code) const OVERRIDE;
  virtual scoped_ptr<webkit_blob::FileStreamReader> CreateFileStreamReader(
      const FileSystemURL& url,
      int64 offset,
      const base::Time& expected_modification_time,
      FileSystemContext* context) const OVERRIDE;
  virtual scoped_ptr<FileStreamWriter> CreateFileStreamWriter(
      const FileSystemURL& url,
      int64 offset,
      FileSystemContext* context) const OVERRIDE;
  virtual FileSystemQuotaUtil* GetQuotaUtil() OVERRIDE;

  
  
  SandboxFileSystemBackendDelegate::OriginEnumerator* CreateOriginEnumerator();

  void set_enable_temporary_file_system_in_incognito(bool enable) {
    enable_temporary_file_system_in_incognito_ = enable;
  }
  bool enable_temporary_file_system_in_incognito() const {
    return enable_temporary_file_system_in_incognito_;
  }


 private:
  SandboxFileSystemBackendDelegate* delegate_;  

  bool enable_temporary_file_system_in_incognito_;

  DISALLOW_COPY_AND_ASSIGN(SandboxFileSystemBackend);
};

}  

#endif  
