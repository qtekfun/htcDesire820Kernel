// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_FILE_SYSTEM_OPERATION_RUNNER_H_
#define WEBKIT_BROWSER_FILEAPI_FILE_SYSTEM_OPERATION_RUNNER_H_

#include <map>
#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/id_map.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "webkit/browser/blob/blob_data_handle.h"
#include "webkit/browser/fileapi/file_system_operation.h"
#include "webkit/browser/fileapi/file_system_url.h"
#include "webkit/browser/webkit_storage_browser_export.h"

namespace net {
class URLRequestContext;
}

namespace fileapi {

class FileSystemURL;
class FileSystemContext;

class WEBKIT_STORAGE_BROWSER_EXPORT FileSystemOperationRunner
    : public base::SupportsWeakPtr<FileSystemOperationRunner> {
 public:
  typedef FileSystemOperation::GetMetadataCallback GetMetadataCallback;
  typedef FileSystemOperation::ReadDirectoryCallback ReadDirectoryCallback;
  typedef FileSystemOperation::SnapshotFileCallback SnapshotFileCallback;
  typedef FileSystemOperation::StatusCallback StatusCallback;
  typedef FileSystemOperation::WriteCallback WriteCallback;
  typedef FileSystemOperation::OpenFileCallback OpenFileCallback;
  typedef FileSystemOperation::CopyProgressCallback CopyProgressCallback;
  typedef FileSystemOperation::CopyFileProgressCallback
      CopyFileProgressCallback;
  typedef FileSystemOperation::CopyOrMoveOption CopyOrMoveOption;

  typedef int OperationID;

  virtual ~FileSystemOperationRunner();

  
  void Shutdown();

  
  
  OperationID CreateFile(const FileSystemURL& url,
                         bool exclusive,
                         const StatusCallback& callback);

  OperationID CreateDirectory(const FileSystemURL& url,
                              bool exclusive,
                              bool recursive,
                              const StatusCallback& callback);

  
  
  
  
  
  
  OperationID Copy(const FileSystemURL& src_url,
                   const FileSystemURL& dest_url,
                   CopyOrMoveOption option,
                   const CopyProgressCallback& progress_callback,
                   const StatusCallback& callback);

  
  
  
  OperationID Move(const FileSystemURL& src_url,
                   const FileSystemURL& dest_url,
                   CopyOrMoveOption option,
                   const StatusCallback& callback);

  
  OperationID DirectoryExists(const FileSystemURL& url,
                              const StatusCallback& callback);

  
  OperationID FileExists(const FileSystemURL& url,
                         const StatusCallback& callback);

  
  OperationID GetMetadata(const FileSystemURL& url,
                          const GetMetadataCallback& callback);

  
  OperationID ReadDirectory(const FileSystemURL& url,
                            const ReadDirectoryCallback& callback);

  
  
  OperationID Remove(const FileSystemURL& url, bool recursive,
                     const StatusCallback& callback);

  
  
  OperationID Write(const net::URLRequestContext* url_request_context,
                    const FileSystemURL& url,
                    scoped_ptr<webkit_blob::BlobDataHandle> blob,
                    int64 offset,
                    const WriteCallback& callback);

  
  
  
  OperationID Truncate(const FileSystemURL& url, int64 length,
                       const StatusCallback& callback);

  
  
  
  void Cancel(OperationID id, const StatusCallback& callback);

  
  
  
  
  
  OperationID TouchFile(const FileSystemURL& url,
                        const base::Time& last_access_time,
                        const base::Time& last_modified_time,
                        const StatusCallback& callback);

  
  
  
  
  
  
  
  OperationID OpenFile(const FileSystemURL& url,
                       int file_flags,
                       const OpenFileCallback& callback);

  
  
  
  
  
  
  
  
  OperationID CreateSnapshotFile(const FileSystemURL& url,
                                 const SnapshotFileCallback& callback);

  
  
  
  
  
  
  
  
  
  
  OperationID CopyInForeignFile(const base::FilePath& src_local_disk_path,
                                const FileSystemURL& dest_url,
                                const StatusCallback& callback);

  
  
  
  
  
  
  OperationID RemoveFile(const FileSystemURL& url,
                         const StatusCallback& callback);

  
  
  
  
  
  
  
  OperationID RemoveDirectory(const FileSystemURL& url,
                              const StatusCallback& callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  OperationID CopyFileLocal(const FileSystemURL& src_url,
                            const FileSystemURL& dest_url,
                            CopyOrMoveOption option,
                            const CopyFileProgressCallback& progress_callback,
                            const StatusCallback& callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  OperationID MoveFileLocal(const FileSystemURL& src_url,
                            const FileSystemURL& dest_url,
                            CopyOrMoveOption option,
                            const StatusCallback& callback);

  
  
  
  base::PlatformFileError SyncGetPlatformPath(const FileSystemURL& url,
                                              base::FilePath* platform_path);

 private:
  class BeginOperationScoper;

  struct OperationHandle {
    OperationID id;
    base::WeakPtr<BeginOperationScoper> scope;

    OperationHandle();
    ~OperationHandle();
  };

  friend class FileSystemContext;
  explicit FileSystemOperationRunner(FileSystemContext* file_system_context);

  void DidFinish(const OperationHandle& handle,
                 const StatusCallback& callback,
                 base::PlatformFileError rv);
  void DidGetMetadata(const OperationHandle& handle,
                      const GetMetadataCallback& callback,
                      base::PlatformFileError rv,
                      const base::PlatformFileInfo& file_info);
  void DidReadDirectory(const OperationHandle& handle,
                        const ReadDirectoryCallback& callback,
                        base::PlatformFileError rv,
                        const std::vector<DirectoryEntry>& entries,
                        bool has_more);
  void DidWrite(const OperationHandle& handle,
                const WriteCallback& callback,
                base::PlatformFileError rv,
                int64 bytes,
                bool complete);
  void DidOpenFile(
      const OperationHandle& handle,
      const OpenFileCallback& callback,
      base::PlatformFileError rv,
      base::PlatformFile file,
      const base::Closure& on_close_callback);
  void DidCreateSnapshot(
      const OperationHandle& handle,
      const SnapshotFileCallback& callback,
      base::PlatformFileError rv,
      const base::PlatformFileInfo& file_info,
      const base::FilePath& platform_path,
      const scoped_refptr<webkit_blob::ShareableFileReference>& file_ref);

  void OnCopyProgress(
      const OperationHandle& handle,
      const CopyProgressCallback& callback,
      FileSystemOperation::CopyProgressType type,
      const FileSystemURL& source_url,
      const FileSystemURL& dest_url,
      int64 size);

  void PrepareForWrite(OperationID id, const FileSystemURL& url);
  void PrepareForRead(OperationID id, const FileSystemURL& url);

  
  OperationHandle BeginOperation(FileSystemOperation* operation,
                                 base::WeakPtr<BeginOperationScoper> scope);
  void FinishOperation(OperationID id);

  
  FileSystemContext* file_system_context_;

  
  IDMap<FileSystemOperation, IDMapOwnPointer> operations_;

  
  
  typedef std::map<OperationID, FileSystemURLSet> OperationToURLSet;
  OperationToURLSet write_target_urls_;

  
  std::set<OperationID> finished_operations_;

  
  std::map<OperationID, StatusCallback> stray_cancel_callbacks_;

  DISALLOW_COPY_AND_ASSIGN(FileSystemOperationRunner);
};

}  

#endif  
