// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_REMOTE_TO_LOCAL_SYNCER_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_REMOTE_TO_LOCAL_SYNCER_H_

#include <string>
#include <vector>

#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/sync_file_system/drive_backend/metadata_database.pb.h"
#include "chrome/browser/sync_file_system/remote_change_processor.h"
#include "chrome/browser/sync_file_system/sync_action.h"
#include "chrome/browser/sync_file_system/sync_callbacks.h"
#include "chrome/browser/sync_file_system/sync_file_metadata.h"
#include "chrome/browser/sync_file_system/sync_task.h"
#include "google_apis/drive/gdata_errorcode.h"
#include "webkit/browser/fileapi/file_system_url.h"

namespace drive {
class DriveServiceInterface;
}

namespace google_apis {
class FileResource;
class ResourceEntry;
class ResourceList;
}

namespace webkit_blob {
class ScopedFile;
}

namespace sync_file_system {
namespace drive_backend {

class MetadataDatabase;
class SyncEngineContext;

class RemoteToLocalSyncer : public SyncTask {
 public:
  
  
  explicit RemoteToLocalSyncer(SyncEngineContext* sync_context);
  virtual ~RemoteToLocalSyncer();

  virtual void Run(const SyncStatusCallback& callback) OVERRIDE;

  const fileapi::FileSystemURL& url() const { return url_; }
  SyncAction sync_action() const { return sync_action_; }

  bool is_sync_root_deletion() const { return sync_root_deletion_; }

 private:
  typedef std::vector<std::string> FileIDList;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void ResolveRemoteChange(const SyncStatusCallback& callback);

  
  
  
  
  void HandleMissingRemoteMetadata(const SyncStatusCallback& callback);
  void DidGetRemoteMetadata(const SyncStatusCallback& callback,
                            google_apis::GDataErrorCode error,
                            scoped_ptr<google_apis::ResourceEntry> entry);
  void DidUpdateDatabaseForRemoteMetadata(const SyncStatusCallback& callback,
                                          SyncStatusCode status);

  
  
  
  
  
  
  
  
  
  
  
  
  void DidPrepareForAddOrUpdateFile(const SyncStatusCallback& callback,
                                    SyncStatusCode status);

  
  
  void HandleFolderUpdate(const SyncStatusCallback& callback);
  void DidPrepareForFolderUpdate(const SyncStatusCallback& callback,
                                 SyncStatusCode status);

  void HandleSyncRootDeletion(const SyncStatusCallback& callback);

  
  
  
  
  
  
  
  
  
  
  
  
  void HandleDeletion(const SyncStatusCallback& callback);
  void DidPrepareForDeletion(const SyncStatusCallback& callback,
                             SyncStatusCode status);

  
  void HandleContentUpdate(const SyncStatusCallback& callback);

  void ListFolderContent(const SyncStatusCallback& callback);
  void DidListFolderContent(
      const SyncStatusCallback& callback,
      scoped_ptr<FileIDList> children,
      google_apis::GDataErrorCode error,
      scoped_ptr<google_apis::ResourceList> resource_list);

  void SyncCompleted(const SyncStatusCallback& callback, SyncStatusCode status);
  void FinalizeSync(const SyncStatusCallback& callback, SyncStatusCode status);


  void Prepare(const SyncStatusCallback& callback);
  void DidPrepare(const SyncStatusCallback& callback,
                  SyncStatusCode status,
                  const SyncFileMetadata& metadata,
                  const FileChangeList& changes);

  void DeleteLocalFile(const SyncStatusCallback& callback);
  void DownloadFile(const SyncStatusCallback& callback);
  void DidCreateTemporaryFileForDownload(const SyncStatusCallback& callback,
                                         webkit_blob::ScopedFile file);
  void DidDownloadFile(const SyncStatusCallback& callback,
                       webkit_blob::ScopedFile file,
                       google_apis::GDataErrorCode error,
                       const base::FilePath&);
  void DidCalculateMD5ForDownload(const SyncStatusCallback& callback,
                                  webkit_blob::ScopedFile file,
                                  const std::string& md5);
  void DidApplyDownload(const SyncStatusCallback& callback,
                        webkit_blob::ScopedFile,
                        SyncStatusCode status);

  void CreateFolder(const SyncStatusCallback& callback);

  drive::DriveServiceInterface* drive_service();
  MetadataDatabase* metadata_database();
  RemoteChangeProcessor* remote_change_processor();

  SyncEngineContext* sync_context_;  

  scoped_ptr<FileTracker> dirty_tracker_;
  scoped_ptr<FileMetadata> remote_metadata_;

  fileapi::FileSystemURL url_;
  SyncAction sync_action_;

  bool prepared_;
  bool sync_root_deletion_;

  scoped_ptr<SyncFileMetadata> local_metadata_;
  scoped_ptr<FileChangeList> local_changes_;

  base::WeakPtrFactory<RemoteToLocalSyncer> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(RemoteToLocalSyncer);
};

}  
}  

#endif  
