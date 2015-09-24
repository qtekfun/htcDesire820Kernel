// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_CHANGE_LIST_LOADER_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_CHANGE_LIST_LOADER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "google_apis/drive/drive_common_callbacks.h"
#include "google_apis/drive/gdata_errorcode.h"

class GURL;

namespace base {
class SequencedTaskRunner;
}  

namespace google_apis {
class AboutResource;
class ResourceList;
}  

namespace drive {

class DriveServiceInterface;
class JobScheduler;
class ResourceEntry;

namespace internal {

class ChangeList;
class ChangeListLoaderObserver;
class ChangeListProcessor;
class DirectoryFetchInfo;
class ResourceMetadata;

typedef base::Callback<void(ScopedVector<ChangeList> change_lists,
                            FileError error)> LoadChangeListCallback;

class ChangeListLoader {
 public:
  
  class FeedFetcher;

  ChangeListLoader(base::SequencedTaskRunner* blocking_task_runner,
                   ResourceMetadata* resource_metadata,
                   JobScheduler* scheduler,
                   DriveServiceInterface* drive_service);
  ~ChangeListLoader();

  
  
  
  bool IsRefreshing() const;

  
  void AddObserver(ChangeListLoaderObserver* observer);
  void RemoveObserver(ChangeListLoaderObserver* observer);

  
  
  
  
  void CheckForUpdates(const FileOperationCallback& callback);

  
  
  
  
  
  
  
  
  
  
  
  void LoadIfNeeded(const DirectoryFetchInfo& directory_fetch_info,
                    const FileOperationCallback& callback);

  
  
  
  void GetAboutResource(const google_apis::AboutResourceCallback& callback);

 private:
  
  
  
  
  void Load(const DirectoryFetchInfo& directory_fetch_info,
            const FileOperationCallback& callback);

  
  
  
  
  
  
  
  void DoInitialLoad(const DirectoryFetchInfo& directory_fetch_info,
                     int64 local_changestamp);
  void DoUpdateLoad(const DirectoryFetchInfo& directory_fetch_info,
                    int64 local_changestamp);

  
  
  
  void OnChangeListLoadComplete(FileError error);

  
  
  
  void OnDirectoryLoadComplete(const DirectoryFetchInfo& directory_fetch_info,
                               FileError error);

  

  
  
  
  void LoadFromServerIfNeeded(const DirectoryFetchInfo& directory_fetch_info,
                              int64 local_changestamp);

  
  
  void LoadFromServerIfNeededAfterGetAbout(
      const DirectoryFetchInfo& directory_fetch_info,
      int64 local_changestamp,
      google_apis::GDataErrorCode status,
      scoped_ptr<google_apis::AboutResource> about_resource);

  
  
  
  
  
  void LoadFromServerIfNeededAfterLoadDirectory(
      const DirectoryFetchInfo& directory_fetch_info,
      scoped_ptr<google_apis::AboutResource> about_resource,
      int64 start_changestamp,
      FileError error);

  
  
  
  
  void LoadChangeListFromServer(
      scoped_ptr<google_apis::AboutResource> about_resource,
      int64 start_changestamp);

  
  
  void LoadChangeListFromServerAfterLoadChangeList(
      scoped_ptr<google_apis::AboutResource> about_resource,
      bool is_delta_update,
      FileError error,
      ScopedVector<ChangeList> change_lists);

  
  
  void LoadChangeListFromServerAfterUpdate();

  

  
  
  
  void CheckChangestampAndLoadDirectoryIfNeeded(
      const DirectoryFetchInfo& directory_fetch_info,
      int64 local_changestamp,
      const FileOperationCallback& callback);

  
  
  void DoLoadDirectoryFromServer(const DirectoryFetchInfo& directory_fetch_info,
                                 const FileOperationCallback& callback);

  
  void DoLoadGrandRootDirectoryFromServerAfterGetResourceEntryByPath(
      const DirectoryFetchInfo& directory_fetch_info,
      const FileOperationCallback& callback,
      FileError error,
      scoped_ptr<ResourceEntry> entry);

  
  void DoLoadGrandRootDirectoryFromServerAfterGetAboutResource(
      const DirectoryFetchInfo& directory_fetch_info,
      const FileOperationCallback& callback,
      google_apis::GDataErrorCode status,
      scoped_ptr<google_apis::AboutResource> about_resource);

  
  void DoLoadDirectoryFromServerAfterAddMyDrive(
      const DirectoryFetchInfo& directory_fetch_info,
      const FileOperationCallback& callback,
      std::string* local_id,
      FileError error);

  
  void DoLoadDirectoryFromServerAfterLoad(
      const DirectoryFetchInfo& directory_fetch_info,
      const FileOperationCallback& callback,
      FeedFetcher* fetcher,
      FileError error,
      ScopedVector<ChangeList> change_lists);

  
  void DoLoadDirectoryFromServerAfterRefresh(
      const DirectoryFetchInfo& directory_fetch_info,
      const FileOperationCallback& callback,
      const base::FilePath* directory_path,
      FileError error);

  

  
  
  
  
  
  
  void UpdateFromChangeList(
      scoped_ptr<google_apis::AboutResource> about_resource,
      ScopedVector<ChangeList> change_lists,
      bool is_delta_update,
      const base::Closure& callback);

  
  
  
  void UpdateFromChangeListAfterApply(
      ChangeListProcessor* change_list_processor,
      bool should_notify,
      base::Time start_time,
      const base::Closure& callback,
      FileError error);

  
  
  
  void UpdateAboutResource(
      const google_apis::AboutResourceCallback& callback);
  
  
  
  
  void UpdateAboutResourceAfterGetAbout(
      const google_apis::AboutResourceCallback& callback,
      google_apis::GDataErrorCode status,
      scoped_ptr<google_apis::AboutResource> about_resource);

  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;
  ResourceMetadata* resource_metadata_;  
  JobScheduler* scheduler_;  
  DriveServiceInterface* drive_service_;  
  ObserverList<ChangeListLoaderObserver> observers_;
  typedef std::map<std::string, std::vector<FileOperationCallback> >
      LoadCallbackMap;
  LoadCallbackMap pending_load_callback_;
  FileOperationCallback pending_update_check_callback_;

  
  scoped_ptr<FeedFetcher> change_feed_fetcher_;

  
  std::set<FeedFetcher*> fast_fetch_feed_fetcher_set_;

  
  scoped_ptr<google_apis::AboutResource> cached_about_resource_;

  
  
  bool loaded_;

  
  
  base::WeakPtrFactory<ChangeListLoader> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(ChangeListLoader);
};

}  
}  

#endif  
