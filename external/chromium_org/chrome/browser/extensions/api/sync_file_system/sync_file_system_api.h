// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_SYNC_FILE_SYSTEM_SYNC_FILE_SYSTEM_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_SYNC_FILE_SYSTEM_SYNC_FILE_SYSTEM_API_H_

#include <map>

#include "base/platform_file.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/browser/sync_file_system/conflict_resolution_policy.h"
#include "chrome/browser/sync_file_system/sync_file_status.h"
#include "chrome/browser/sync_file_system/sync_status_code.h"
#include "chrome/common/extensions/api/sync_file_system.h"
#include "webkit/browser/fileapi/file_system_url.h"
#include "webkit/common/quota/quota_types.h"

namespace fileapi {
class FileSystemContext;
}

namespace extensions {

class SyncFileSystemDeleteFileSystemFunction
    : public ChromeAsyncExtensionFunction {
 public:
  
  
  
  

 protected:
  virtual ~SyncFileSystemDeleteFileSystemFunction() {}
  virtual bool RunImpl() OVERRIDE;

 private:
  void DidDeleteFileSystem(base::PlatformFileError error);
};

class SyncFileSystemGetFileStatusFunction
    : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("syncFileSystem.getFileStatus",
                             SYNCFILESYSTEM_GETFILESYNCSTATUS)

 protected:
  virtual ~SyncFileSystemGetFileStatusFunction() {}
  virtual bool RunImpl() OVERRIDE;

 private:
  void DidGetFileStatus(
      const sync_file_system::SyncStatusCode sync_service_status,
      const sync_file_system::SyncFileStatus sync_file_status);
};

class SyncFileSystemGetFileStatusesFunction
    : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("syncFileSystem.getFileStatuses",
                             SYNCFILESYSTEM_GETFILESYNCSTATUSES)
  SyncFileSystemGetFileStatusesFunction();

 protected:
  virtual ~SyncFileSystemGetFileStatusesFunction();
  virtual bool RunImpl() OVERRIDE;

 private:
  typedef std::pair<sync_file_system::SyncStatusCode,
                    sync_file_system::SyncFileStatus> FileStatusPair;
  typedef std::map<fileapi::FileSystemURL, FileStatusPair,
                   fileapi::FileSystemURL::Comparator> URLToStatusMap;

  void DidGetFileStatus(
      const fileapi::FileSystemURL& file_system_url,
      sync_file_system::SyncStatusCode sync_status_code,
      sync_file_system::SyncFileStatus sync_file_statuses);

  unsigned int num_expected_results_;
  unsigned int num_results_received_;
  URLToStatusMap file_sync_statuses_;
};

class SyncFileSystemGetUsageAndQuotaFunction
    : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("syncFileSystem.getUsageAndQuota",
                             SYNCFILESYSTEM_GETUSAGEANDQUOTA)

 protected:
  virtual ~SyncFileSystemGetUsageAndQuotaFunction() {}
  virtual bool RunImpl() OVERRIDE;

 private:
  void DidGetUsageAndQuota(quota::QuotaStatusCode status,
                           int64 usage,
                           int64 quota);
};

class SyncFileSystemRequestFileSystemFunction
    : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("syncFileSystem.requestFileSystem",
                             SYNCFILESYSTEM_REQUESTFILESYSTEM)

 protected:
  virtual ~SyncFileSystemRequestFileSystemFunction() {}
  virtual bool RunImpl() OVERRIDE;

 private:
  typedef SyncFileSystemRequestFileSystemFunction self;

  
  fileapi::FileSystemContext* GetFileSystemContext();

  void DidOpenFileSystem(const GURL& root_url,
                         const std::string& file_system_name,
                         base::PlatformFileError error);
};

class SyncFileSystemSetConflictResolutionPolicyFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("syncFileSystem.setConflictResolutionPolicy",
                             SYNCFILESYSTEM_SETCONFLICTRESOLUTIONPOLICY)

 protected:
  virtual ~SyncFileSystemSetConflictResolutionPolicyFunction() {}
  virtual bool RunImpl() OVERRIDE;
};

class SyncFileSystemGetConflictResolutionPolicyFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("syncFileSystem.getConflictResolutionPolicy",
                             SYNCFILESYSTEM_GETCONFLICTRESOLUTIONPOLICY)

 protected:
  virtual ~SyncFileSystemGetConflictResolutionPolicyFunction() {}
  virtual bool RunImpl() OVERRIDE;
};

class SyncFileSystemGetServiceStatusFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("syncFileSystem.getServiceStatus",
                             SYNCFILESYSTEM_GETSERVICESTATUS)

 protected:
  virtual ~SyncFileSystemGetServiceStatusFunction() {}
  virtual bool RunImpl() OVERRIDE;
};

}  

#endif  