// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_V1_DRIVE_METADATA_STORE_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_V1_DRIVE_METADATA_STORE_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/sync_file_system/sync_callbacks.h"
#include "chrome/browser/sync_file_system/sync_status_code.h"
#include "webkit/browser/fileapi/file_system_url.h"

namespace base {
class ListValue;
class SequencedTaskRunner;
}

namespace leveldb {
class DB;
class WriteBatch;
}

class GURL;

namespace sync_file_system {

class DriveMetadata;
struct DBContents;
struct FileMetadata;

class DriveMetadataStore
    : public base::NonThreadSafe,
      public base::SupportsWeakPtr<DriveMetadataStore> {
 public:
  typedef std::map<GURL, std::string> ResourceIdByOrigin;
  typedef std::map<std::string, GURL> OriginByResourceId;
  typedef std::map<base::FilePath, DriveMetadata> PathToMetadata;
  typedef std::map<GURL, PathToMetadata> MetadataMap;
  typedef std::vector<std::pair<fileapi::FileSystemURL, DriveMetadata> >
      URLAndDriveMetadataList;
  typedef base::Callback<void(SyncStatusCode status, bool created)>
      InitializationCallback;

  static const base::FilePath::CharType kDatabaseName[];

  DriveMetadataStore(const base::FilePath& base_dir,
                     base::SequencedTaskRunner* file_task_runner);
  ~DriveMetadataStore();

  
  
  void Initialize(const InitializationCallback& callback);

  void SetLargestChangeStamp(int64 largest_changestamp,
                             const SyncStatusCallback& callback);
  int64 GetLargestChangeStamp() const;

  
  void UpdateEntry(const fileapi::FileSystemURL& url,
                   const DriveMetadata& metadata,
                   const SyncStatusCallback& callback);

  
  void DeleteEntry(const fileapi::FileSystemURL& url,
                   const SyncStatusCallback& callback);

  
  SyncStatusCode ReadEntry(const fileapi::FileSystemURL& url,
                           DriveMetadata* metadata) const;

  
  
  
  void AddIncrementalSyncOrigin(const GURL& origin,
                                const std::string& resource_id);

  
  bool IsKnownOrigin(const GURL& origin) const;

  
  
  
  bool IsIncrementalSyncOrigin(const GURL& origin) const;

  
  bool IsOriginDisabled(const GURL& origin) const;

  void EnableOrigin(const GURL& origin,
                    const SyncStatusCallback& callback);

  void DisableOrigin(const GURL& origin,
                     const SyncStatusCallback& callback);

  void RemoveOrigin(const GURL& origin,
                    const SyncStatusCallback& callback);

  
  
  void SetSyncRootDirectory(const std::string& resource_id);
  void SetOriginRootDirectory(const GURL& origin,
                              const std::string& resource_id);

  
  SyncStatusCode GetConflictURLs(
      fileapi::FileSystemURLSet* urls) const;

  
  SyncStatusCode GetToBeFetchedFiles(URLAndDriveMetadataList* list) const;

  
  
  
  std::string GetResourceIdForOrigin(const GURL& origin) const;

  const std::string& sync_root_directory() const {
    DCHECK(CalledOnValidThread());
    return sync_root_directory_resource_id_;
  }

  const ResourceIdByOrigin& incremental_sync_origins() const {
    DCHECK(CalledOnValidThread());
    return incremental_sync_origins_;
  }

  const ResourceIdByOrigin& disabled_origins() const {
    DCHECK(CalledOnValidThread());
    return disabled_origins_;
  }

  
  
  void GetAllOrigins(std::vector<GURL>* origins);

  
  
  
  bool GetOriginByOriginRootDirectoryId(const std::string& resource_id,
                                        GURL* origin);

  
  scoped_ptr<base::ListValue> DumpFiles(const GURL& origin);

 private:
  friend class DriveMetadataStoreTest;

  void WriteToDB(scoped_ptr<leveldb::WriteBatch> batch,
                 const SyncStatusCallback& callback);

  void UpdateDBStatus(SyncStatusCode status);
  void UpdateDBStatusAndInvokeCallback(const SyncStatusCallback& callback,
                                       const leveldb::Status& status);
  void DidInitialize(const InitializationCallback& callback,
                     scoped_ptr<DBContents> contents);

  scoped_refptr<base::SequencedTaskRunner> file_task_runner_;
  base::FilePath base_dir_;
  scoped_ptr<leveldb::DB> db_;
  SyncStatusCode db_status_;

  int64 largest_changestamp_;
  MetadataMap metadata_map_;

  std::string sync_root_directory_resource_id_;
  ResourceIdByOrigin incremental_sync_origins_;
  ResourceIdByOrigin disabled_origins_;

  OriginByResourceId origin_by_resource_id_;

  DISALLOW_COPY_AND_ASSIGN(DriveMetadataStore);
};

}  

#endif  
