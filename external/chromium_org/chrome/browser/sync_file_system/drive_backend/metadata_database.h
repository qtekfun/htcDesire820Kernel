// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_METADATA_DATABASE_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_METADATA_DATABASE_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/values.h"
#include "chrome/browser/sync_file_system/drive_backend/tracker_set.h"
#include "chrome/browser/sync_file_system/sync_callbacks.h"
#include "chrome/browser/sync_file_system/sync_status_code.h"

namespace base {
class FilePath;
class SequencedTaskRunner;
class SingleThreadTaskRunner;
}

namespace leveldb {
class DB;
class WriteBatch;
}

namespace google_apis {
class ChangeResource;
class FileResource;
class ResourceEntry;
}

namespace tracked_objects {
class Location;
}

namespace sync_file_system {
namespace drive_backend {

class FileDetails;
class FileMetadata;
class FileTracker;
class ServiceMetadata;
struct DatabaseContents;

class MetadataDatabase {
 public:
  typedef std::map<std::string, FileMetadata*> FileByID;
  typedef std::map<int64, FileTracker*> TrackerByID;
  typedef std::map<std::string, TrackerSet> TrackersByFileID;
  typedef std::map<std::string, TrackerSet> TrackersByTitle;
  typedef std::map<int64, TrackersByTitle> TrackersByParentAndTitle;
  typedef std::map<std::string, FileTracker*> TrackerByAppID;
  typedef std::vector<std::string> FileIDList;

  typedef base::Callback<
      void(SyncStatusCode status, scoped_ptr<MetadataDatabase> instance)>
      CreateCallback;

  
  static void Create(base::SequencedTaskRunner* task_runner,
                     const base::FilePath& database_path,
                     const CreateCallback& callback);
  ~MetadataDatabase();

  static void ClearDatabase(scoped_ptr<MetadataDatabase> metadata_database);

  int64 GetLargestFetchedChangeID() const;
  int64 GetSyncRootTrackerID() const;
  bool HasSyncRoot() const;

  
  scoped_ptr<base::ListValue> DumpFiles(const std::string& app_id);

  
  scoped_ptr<base::ListValue> DumpDatabase();

  
  
  
  
  
  
  
  
  
  
  
  // FileMetadata from overwritten by ChangeList.
  
  
  int64 GetLargestKnownChangeID() const;
  void UpdateLargestKnownChangeID(int64 change_id);

  
  
  
  
  
  
  
  void PopulateInitialData(
      int64 largest_change_id,
      const google_apis::FileResource& sync_root_folder,
      const ScopedVector<google_apis::FileResource>& app_root_folders,
      const SyncStatusCallback& callback);

  
  bool IsAppEnabled(const std::string& app_id) const;

  
  
  
  void RegisterApp(const std::string& app_id,
                   const std::string& folder_id,
                   const SyncStatusCallback& callback);

  
  
  void DisableApp(const std::string& app_id,
                  const SyncStatusCallback& callback);

  
  
  void EnableApp(const std::string& app_id,
                 const SyncStatusCallback& callback);

  
  
  
  
  void UnregisterApp(const std::string& app_id,
                     const SyncStatusCallback& callback);

  
  
  bool FindAppRootTracker(const std::string& app_id,
                          FileTracker* tracker) const;

  
  
  
  bool FindFileByFileID(const std::string& file_id, FileMetadata* file) const;

  
  
  
  
  bool FindTrackerByTrackerID(int64 tracker_id, FileTracker* tracker) const;

  
  
  bool FindTrackersByFileID(const std::string& file_id,
                            TrackerSet* trackers) const;

  
  
  
  
  bool FindTrackersByParentAndTitle(
      int64 parent_tracker_id,
      const std::string& title,
      TrackerSet* trackers) const;

  
  
  
  bool BuildPathForTracker(int64 tracker_id, base::FilePath* path) const;

  
  
  
  base::FilePath BuildDisplayPathForTracker(const FileTracker& tracker) const;

  
  
  
  
  bool FindNearestActiveAncestor(const std::string& app_id,
                                 const base::FilePath& full_path,
                                 FileTracker* tracker,
                                 base::FilePath* path) const;

  
  
  
  void UpdateByChangeList(int64 largest_change_id,
                          ScopedVector<google_apis::ChangeResource> changes,
                          const SyncStatusCallback& callback);

  
  
  
  void UpdateByFileResource(const google_apis::FileResource& resource,
                            const SyncStatusCallback& callback);
  void UpdateByFileResourceList(
      ScopedVector<google_apis::FileResource> resources,
      const SyncStatusCallback& callback);

  void UpdateByDeletedRemoteFile(const std::string& file_id,
                                 const SyncStatusCallback& callback);

  
  
  
  
  
  
  void ReplaceActiveTrackerWithNewResource(
      int64 parent_tracker_id,
      const google_apis::FileResource& resource,
      const SyncStatusCallback& callback);

  
  
  
  void PopulateFolderByChildList(const std::string& folder_id,
                                 const FileIDList& child_file_ids,
                                 const SyncStatusCallback& callback);

  
  void UpdateTracker(int64 tracker_id,
                     const FileDetails& updated_details,
                     const SyncStatusCallback& callback);

  
  
  
  
  bool TryNoSideEffectActivation(int64 parent_tracker_id,
                                 const std::string& file_id,
                                 const SyncStatusCallback& callback);


  
  void LowerTrackerPriority(int64 tracker_id);
  void PromoteLowerPriorityTrackersToNormal();

  
  
  bool GetNormalPriorityDirtyTracker(FileTracker* tracker) const;

  
  
  bool GetLowPriorityDirtyTracker(FileTracker* tracker) const;

  bool HasDirtyTracker() const {
    return !dirty_trackers_.empty() || !low_priority_dirty_trackers_.empty();
  }

  size_t GetDirtyTrackerCount() {
    return dirty_trackers_.size();
  }

  bool GetMultiParentFileTrackers(std::string* file_id,
                                  TrackerSet* trackers);
  bool GetConflictingTrackers(TrackerSet* trackers);

  
  void GetRegisteredAppIDs(std::vector<std::string>* app_ids);

 private:
  friend class ListChangesTaskTest;
  friend class MetadataDatabaseTest;
  friend class RegisterAppTaskTest;
  friend class SyncEngineInitializerTest;

  struct DirtyTrackerComparator {
    bool operator()(const FileTracker* left,
                    const FileTracker* right) const;
  };

  typedef std::set<FileTracker*, DirtyTrackerComparator> DirtyTrackers;

  MetadataDatabase(base::SequencedTaskRunner* task_runner,
                   const base::FilePath& database_path);
  static void CreateOnTaskRunner(base::SingleThreadTaskRunner* callback_runner,
                                 base::SequencedTaskRunner* task_runner,
                                 const base::FilePath& database_path,
                                 const CreateCallback& callback);
  static SyncStatusCode CreateForTesting(
      scoped_ptr<leveldb::DB> db,
      scoped_ptr<MetadataDatabase>* metadata_database_out);
  SyncStatusCode InitializeOnTaskRunner();
  void BuildIndexes(DatabaseContents* contents);

  
  void RegisterTrackerAsAppRoot(const std::string& app_id,
                                int64 tracker_id,
                                leveldb::WriteBatch* batch);
  void MakeTrackerActive(int64 tracker_id, leveldb::WriteBatch* batch);
  void MakeTrackerInactive(int64 tracker_id, leveldb::WriteBatch* batch);
  void MakeAppRootDisabled(int64 tracker_id, leveldb::WriteBatch* batch);
  void MakeAppRootEnabled(int64 tracker_id, leveldb::WriteBatch* batch);

  void UnregisterTrackerAsAppRoot(const std::string& app_id,
                                  leveldb::WriteBatch* batch);
  void RemoveAllDescendantTrackers(int64 root_tracker_id,
                                   leveldb::WriteBatch* batch);

  void CreateTrackerForParentAndFileID(const FileTracker& parent_tracker,
                                       const std::string& file_id,
                                       leveldb::WriteBatch* batch);
  void CreateTrackerForParentAndFileMetadata(const FileTracker& parent_tracker,
                                             const FileMetadata& file_metadata,
                                             leveldb::WriteBatch* batch);
  void CreateTrackerInternal(const FileTracker& parent_tracker,
                             const std::string& file_id,
                             const FileDetails* details,
                             leveldb::WriteBatch* batch);

  void RemoveTracker(int64 tracker_id, leveldb::WriteBatch* batch);
  void RemoveTrackerIgnoringSameTitle(int64 tracker_id,
                                      leveldb::WriteBatch* batch);
  void RemoveTrackerInternal(int64 tracker_id,
                             leveldb::WriteBatch* batch,
                             bool ignoring_same_title);
  void MaybeAddTrackersForNewFile(const FileMetadata& file,
                                  leveldb::WriteBatch* batch);

  void MarkSingleTrackerDirty(FileTracker* tracker,
                              leveldb::WriteBatch* batch);
  void MarkTrackerSetDirty(TrackerSet* trackers,
                           leveldb::WriteBatch* batch);
  void MarkTrackersDirtyByFileID(const std::string& file_id,
                                 leveldb::WriteBatch* batch);
  void MarkTrackersDirtyByPath(int64 parent_tracker_id,
                               const std::string& title,
                               leveldb::WriteBatch* batch);

  void EraseTrackerFromFileIDIndex(FileTracker* tracker,
                                   leveldb::WriteBatch* batch);
  void EraseTrackerFromPathIndex(FileTracker* tracker);
  void EraseFileFromDatabase(const std::string& file_id,
                             leveldb::WriteBatch* batch);

  int64 GetNextTrackerID(leveldb::WriteBatch* batch);

  void RecursiveMarkTrackerAsDirty(int64 root_tracker_id,
                                   leveldb::WriteBatch* batch);
  bool CanActivateTracker(const FileTracker& tracker);
  bool ShouldKeepDirty(const FileTracker& tracker) const;

  bool HasDisabledAppRoot(const FileTracker& tracker) const;
  bool HasActiveTrackerForFileID(const std::string& file_id) const;
  bool HasActiveTrackerForPath(int64 parent_tracker,
                               const std::string& title) const;

  void UpdateByFileMetadata(const tracked_objects::Location& from_where,
                            scoped_ptr<FileMetadata> file,
                            leveldb::WriteBatch* batch);

  void WriteToDatabase(scoped_ptr<leveldb::WriteBatch> batch,
                       const SyncStatusCallback& callback);

  bool HasNewerFileMetadata(const std::string& file_id, int64 change_id);

  scoped_ptr<base::ListValue> DumpTrackers();
  scoped_ptr<base::ListValue> DumpMetadata();

  scoped_refptr<base::SequencedTaskRunner> task_runner_;
  base::FilePath database_path_;
  scoped_ptr<leveldb::DB> db_;

  scoped_ptr<ServiceMetadata> service_metadata_;
  int64 largest_known_change_id_;

  FileByID file_by_id_;  
  TrackerByID tracker_by_id_;  

  
  
  TrackersByFileID trackers_by_file_id_;  

  
  
  
  TrackerByAppID app_root_by_app_id_;  

  
  
  
  
  
  
  TrackersByParentAndTitle trackers_by_parent_and_title_;

  
  
  DirtyTrackers dirty_trackers_;  
  DirtyTrackers low_priority_dirty_trackers_;  

  base::WeakPtrFactory<MetadataDatabase> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(MetadataDatabase);
};

}  
}  

#endif  
