// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_SYNC_ENGINE_INITIALIZER_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_SYNC_ENGINE_INITIALIZER_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner.h"
#include "chrome/browser/sync_file_system/sync_callbacks.h"
#include "chrome/browser/sync_file_system/sync_task.h"
#include "google_apis/drive/drive_common_callbacks.h"
#include "google_apis/drive/gdata_errorcode.h"

namespace drive {
class DriveServiceInterface;
}

namespace google_apis {
class AboutResource;
class ResourceEntry;
class ResourceList;
}

namespace sync_file_system {
namespace drive_backend {

class MetadataDatabase;
class SyncEngineContext;

class SyncEngineInitializer : public SyncTask {
 public:
  SyncEngineInitializer(SyncEngineContext* sync_context,
                        base::SequencedTaskRunner* task_runner,
                        drive::DriveServiceInterface* drive_service,
                        const base::FilePath& database_path);
  virtual ~SyncEngineInitializer();
  virtual void Run(const SyncStatusCallback& callback) OVERRIDE;

  scoped_ptr<MetadataDatabase> PassMetadataDatabase();

 private:
  typedef base::Callback<void(const SyncStatusCallback& callback)> Task;

  void DidCreateMetadataDatabase(const SyncStatusCallback& callback,
                                 SyncStatusCode status,
                                 scoped_ptr<MetadataDatabase> instance);

  void GetAboutResource(const SyncStatusCallback& callback);
  void DidGetAboutResource(
      const SyncStatusCallback& callback,
      google_apis::GDataErrorCode error,
      scoped_ptr<google_apis::AboutResource> about_resource);
  void FindSyncRoot(const SyncStatusCallback& callback);
  void DidFindSyncRoot(const SyncStatusCallback& callback,
                       google_apis::GDataErrorCode error,
                       scoped_ptr<google_apis::ResourceList> resource_list);
  void CreateSyncRoot(const SyncStatusCallback& callback);
  void DidCreateSyncRoot(const SyncStatusCallback& callback,
                         google_apis::GDataErrorCode error,
                         scoped_ptr<google_apis::ResourceEntry> entry);
  void DetachSyncRoot(const SyncStatusCallback& callback);
  void DidDetachSyncRoot(const SyncStatusCallback& callback,
                         google_apis::GDataErrorCode error);
  void ListAppRootFolders(const SyncStatusCallback& callback);
  void DidListAppRootFolders(
      const SyncStatusCallback& callback,
      google_apis::GDataErrorCode error,
      scoped_ptr<google_apis::ResourceList> resource_list);
  void PopulateDatabase(const SyncStatusCallback& callback);
  void DidPopulateDatabase(const SyncStatusCallback& callback,
                           SyncStatusCode status);

  SyncEngineContext* sync_context_;  

  scoped_refptr<base::SequencedTaskRunner> task_runner_;
  drive::DriveServiceInterface* drive_service_;
  google_apis::CancelCallback cancel_callback_;
  base::FilePath database_path_;

  int find_sync_root_retry_count_;

  scoped_ptr<MetadataDatabase> metadata_database_;
  ScopedVector<google_apis::ResourceEntry> app_root_folders_;

  int64 largest_change_id_;
  std::string root_folder_id_;

  scoped_ptr<google_apis::ResourceEntry> sync_root_folder_;

  base::WeakPtrFactory<SyncEngineInitializer> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(SyncEngineInitializer);
};

}  
}  

#endif  
