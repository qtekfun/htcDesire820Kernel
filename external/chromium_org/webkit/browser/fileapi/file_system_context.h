// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_FILE_SYSTEM_CONTEXT_H_
#define WEBKIT_BROWSER_FILEAPI_FILE_SYSTEM_CONTEXT_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/platform_file.h"
#include "base/sequenced_task_runner_helpers.h"
#include "webkit/browser/fileapi/file_system_url.h"
#include "webkit/browser/fileapi/open_file_system_mode.h"
#include "webkit/browser/fileapi/plugin_private_file_system_backend.h"
#include "webkit/browser/fileapi/sandbox_file_system_backend_delegate.h"
#include "webkit/browser/fileapi/task_runner_bound_observer_list.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/fileapi/file_system_types.h"

namespace base {
class FilePath;
class SequencedTaskRunner;
class SingleThreadTaskRunner;
}

namespace chrome {
class NativeMediaFileUtilTest;
}

namespace quota {
class QuotaManagerProxy;
class SpecialStoragePolicy;
}

namespace webkit_blob {
class BlobURLRequestJobTest;
class FileStreamReader;
}

namespace fileapi {

class AsyncFileUtil;
class CopyOrMoveFileValidatorFactory;
class ExternalFileSystemBackend;
class ExternalMountPoints;
class FileStreamWriter;
class FileSystemBackend;
class FileSystemFileUtil;
class FileSystemOperation;
class FileSystemOperationRunner;
class FileSystemOptions;
class FileSystemQuotaUtil;
class FileSystemURL;
class IsolatedFileSystemBackend;
class MountPoints;
class QuotaReservation;
class SandboxFileSystemBackend;

struct DefaultContextDeleter;
struct FileSystemInfo;

class WEBKIT_STORAGE_BROWSER_EXPORT FileSystemContext
    : public base::RefCountedThreadSafe<FileSystemContext,
                                        DefaultContextDeleter> {
 public:
  
  
  
  
  
  
  static int GetPermissionPolicy(FileSystemType type);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  FileSystemContext(
      base::SingleThreadTaskRunner* io_task_runner,
      base::SequencedTaskRunner* file_task_runner,
      ExternalMountPoints* external_mount_points,
      quota::SpecialStoragePolicy* special_storage_policy,
      quota::QuotaManagerProxy* quota_manager_proxy,
      ScopedVector<FileSystemBackend> additional_backends,
      const base::FilePath& partition_path,
      const FileSystemOptions& options);

  
  
  bool DeleteDataForOriginOnFileThread(const GURL& origin_url);

  
  
  
  
  scoped_refptr<QuotaReservation> CreateQuotaReservationOnFileTaskRunner(
      const GURL& origin_url,
      FileSystemType type);

  quota::QuotaManagerProxy* quota_manager_proxy() const {
    return quota_manager_proxy_.get();
  }

  
  void Shutdown();

  
  
  
  FileSystemQuotaUtil* GetQuotaUtil(FileSystemType type) const;

  
  AsyncFileUtil* GetAsyncFileUtil(FileSystemType type) const;

  
  
  
  CopyOrMoveFileValidatorFactory* GetCopyOrMoveFileValidatorFactory(
      FileSystemType type, base::PlatformFileError* error_code) const;

  
  
  
  FileSystemBackend* GetFileSystemBackend(
      FileSystemType type) const;

  
  
  
  
  bool IsSandboxFileSystem(FileSystemType type) const;

  
  const UpdateObserverList* GetUpdateObservers(FileSystemType type) const;
  const AccessObserverList* GetAccessObservers(FileSystemType type) const;

  
  void GetFileSystemTypes(std::vector<FileSystemType>* types) const;

  
  
  
  ExternalFileSystemBackend* external_backend() const;

  
  typedef base::Callback<void(const GURL& root,
                              const std::string& name,
                              base::PlatformFileError result)>
      OpenFileSystemCallback;

  
  typedef base::Callback<void(base::PlatformFileError result,
                              const FileSystemInfo& info,
                              const base::FilePath& file_path,
                              bool is_directory)> ResolveURLCallback;

  
  typedef base::Callback<void(base::PlatformFileError result)> StatusCallback;

  
  
  
  
  
  void OpenFileSystem(
      const GURL& origin_url,
      FileSystemType type,
      OpenFileSystemMode mode,
      const OpenFileSystemCallback& callback);

  
  
  
  void ResolveURL(
      const FileSystemURL& url,
      const ResolveURLCallback& callback);

  
  
  void DeleteFileSystem(
      const GURL& origin_url,
      FileSystemType type,
      const StatusCallback& callback);

  
  
  
  
  
  
  
  
  
  
  scoped_ptr<webkit_blob::FileStreamReader> CreateFileStreamReader(
      const FileSystemURL& url,
      int64 offset,
      const base::Time& expected_modification_time);

  
  
  scoped_ptr<FileStreamWriter> CreateFileStreamWriter(
      const FileSystemURL& url,
      int64 offset);

  
  scoped_ptr<FileSystemOperationRunner> CreateFileSystemOperationRunner();

  base::SequencedTaskRunner* default_file_task_runner() {
    return default_file_task_runner_.get();
  }

  FileSystemOperationRunner* operation_runner() {
    return operation_runner_.get();
  }

  const base::FilePath& partition_path() const { return partition_path_; }

  
  FileSystemURL CrackURL(const GURL& url) const;
  
  
  FileSystemURL CreateCrackedFileSystemURL(const GURL& origin,
                                           FileSystemType type,
                                           const base::FilePath& path) const;

#if defined(OS_CHROMEOS)
  
  void EnableTemporaryFileSystemInIncognito();
#endif

  SandboxFileSystemBackendDelegate* sandbox_delegate() {
    return sandbox_delegate_.get();
  }

  
  
  bool CanServeURLRequest(const FileSystemURL& url) const;

  
  
  void OpenPluginPrivateFileSystem(
      const GURL& origin_url,
      FileSystemType type,
      const std::string& filesystem_id,
      const std::string& plugin_id,
      OpenFileSystemMode mode,
      const StatusCallback& callback);

 private:
  typedef std::map<FileSystemType, FileSystemBackend*>
      FileSystemBackendMap;

  
  friend class FileSystemOperationRunner;

  
  friend class SandboxFileSystemTestHelper;

  
  friend class PluginPrivateFileSystemBackendTest;

  
  friend struct DefaultContextDeleter;
  friend class base::DeleteHelper<FileSystemContext>;
  friend class base::RefCountedThreadSafe<FileSystemContext,
                                          DefaultContextDeleter>;
  ~FileSystemContext();

  void DeleteOnCorrectThread() const;

  
  
  
  
  
  
  
  FileSystemOperation* CreateFileSystemOperation(
      const FileSystemURL& url,
      base::PlatformFileError* error_code);

  
  
  
  
  
  
  
  FileSystemURL CrackFileSystemURL(const FileSystemURL& url) const;

  
  
  void RegisterBackend(FileSystemBackend* backend);

  void DidOpenFileSystemForResolveURL(
      const FileSystemURL& url,
      const ResolveURLCallback& callback,
      const GURL& filesystem_root,
      const std::string& filesystem_name,
      base::PlatformFileError error);

  
  SandboxFileSystemBackend* sandbox_backend() const {
    return sandbox_backend_.get();
  }

  
  PluginPrivateFileSystemBackend* plugin_private_backend() const {
    return plugin_private_backend_.get();
  }

  scoped_refptr<base::SingleThreadTaskRunner> io_task_runner_;
  scoped_refptr<base::SequencedTaskRunner> default_file_task_runner_;

  scoped_refptr<quota::QuotaManagerProxy> quota_manager_proxy_;

  scoped_ptr<SandboxFileSystemBackendDelegate> sandbox_delegate_;

  
  scoped_ptr<SandboxFileSystemBackend> sandbox_backend_;
  scoped_ptr<IsolatedFileSystemBackend> isolated_backend_;

  
  scoped_ptr<PluginPrivateFileSystemBackend> plugin_private_backend_;
  ScopedVector<FileSystemBackend> additional_backends_;

  
  
  
  
  
  
  FileSystemBackendMap backend_map_;

  
  
  scoped_refptr<ExternalMountPoints> external_mount_points_;

  
  
  std::vector<MountPoints*> url_crackers_;

  
  const base::FilePath partition_path_;

  bool is_incognito_;

  scoped_ptr<FileSystemOperationRunner> operation_runner_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(FileSystemContext);
};

struct DefaultContextDeleter {
  static void Destruct(const FileSystemContext* context) {
    context->DeleteOnCorrectThread();
  }
};

}  

#endif  
