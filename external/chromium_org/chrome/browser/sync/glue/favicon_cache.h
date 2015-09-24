// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_FAVICON_CACHE_H_
#define CHROME_BROWSER_SYNC_GLUE_FAVICON_CACHE_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/ref_counted_memory.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "sync/api/sync_change.h"
#include "sync/api/sync_error_factory.h"
#include "sync/api/syncable_service.h"
#include "url/gurl.h"

class Profile;

namespace chrome {
struct FaviconBitmapResult;
}

namespace browser_sync {

enum IconSize {
  SIZE_INVALID,
  SIZE_16,
  SIZE_32,
  SIZE_64,
  NUM_SIZES
};

struct SyncedFaviconInfo;

class FaviconCache : public syncer::SyncableService,
                     public content::NotificationObserver {
 public:
  FaviconCache(Profile* profile, int max_sync_favicon_limit);
  virtual ~FaviconCache();

  
  virtual syncer::SyncMergeResult MergeDataAndStartSyncing(
      syncer::ModelType type,
      const syncer::SyncDataList& initial_sync_data,
      scoped_ptr<syncer::SyncChangeProcessor> sync_processor,
      scoped_ptr<syncer::SyncErrorFactory> error_handler) OVERRIDE;
  virtual void StopSyncing(syncer::ModelType type) OVERRIDE;
  virtual syncer::SyncDataList GetAllSyncData(syncer::ModelType type)
      const OVERRIDE;
  virtual syncer::SyncError ProcessSyncChanges(
      const tracked_objects::Location& from_here,
      const syncer::SyncChangeList& change_list) OVERRIDE;

  
  
  
  bool GetSyncedFaviconForFaviconURL(
      const GURL& favicon_url,
      scoped_refptr<base::RefCountedMemory>* favicon_png) const;

  
  
  
  bool GetSyncedFaviconForPageURL(
      const GURL& page_url,
      scoped_refptr<base::RefCountedMemory>* favicon_png) const;

  
  
  
  
  void OnPageFaviconUpdated(const GURL& page_url);

  
  
  
  void OnFaviconVisited(const GURL& page_url, const GURL& favicon_url);

  
  
  
  void OnReceivedSyncFavicon(const GURL& page_url,
                             const GURL& icon_url,
                             const std::string& icon_bytes,
                             int64 visit_time_ms);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  friend class SyncFaviconCacheTest;

  
  struct FaviconRecencyFunctor {
    bool operator()(const linked_ptr<SyncedFaviconInfo>& lhs,
                    const linked_ptr<SyncedFaviconInfo>& rhs) const;
  };


  
  typedef std::map<GURL, linked_ptr<SyncedFaviconInfo> > FaviconMap;
  typedef std::set<linked_ptr<SyncedFaviconInfo>,
                   FaviconRecencyFunctor> RecencySet;
  
  typedef std::map<GURL, CancelableTaskTracker::TaskId> PageTaskMap;
  
  typedef std::map<GURL, GURL> PageFaviconMap;

  
  
  void OnReceivedSyncFaviconImpl(const GURL& icon_url,
                                 const std::string& icon_bytes,
                                 int64 visit_time_ms);

  
  
  void OnFaviconDataAvailable(
      const GURL& page_url,
      const std::vector<chrome::FaviconBitmapResult>& bitmap_result);

  
  
  
  
  
  void UpdateSyncState(const GURL& icon_url,
                       syncer::SyncChange::SyncChangeType image_change_type,
                       syncer::SyncChange::SyncChangeType tracking_change_type);

  
  
  
  SyncedFaviconInfo* GetFaviconInfo(const GURL& icon_url);

  
  
  void UpdateFaviconVisitTime(const GURL& icon_url, base::Time time);

  
  
  
  void ExpireFaviconsIfNecessary(syncer::SyncChangeList* image_changes,
                                 syncer::SyncChangeList* tracking_changes);

  
  
  GURL GetLocalFaviconFromSyncedData(
      const syncer::SyncData& sync_favicon) const;

  
  
  void MergeSyncFavicon(const syncer::SyncData& sync_favicon,
                        syncer::SyncChangeList* sync_changes);

  
  void AddLocalFaviconFromSyncedData(const syncer::SyncData& sync_favicon);

  
  
  syncer::SyncData CreateSyncDataFromLocalFavicon(
      syncer::ModelType type,
      const GURL& favicon_url) const;

  
  
  void DeleteSyncedFavicons(const std::set<GURL>& favicon_urls);

  
  
  void DeleteSyncedFavicon(FaviconMap::iterator favicon_iter,
                           syncer::SyncChangeList* image_changes,
                           syncer::SyncChangeList* tracking_changes);

  
  void DropSyncedFavicon(FaviconMap::iterator favicon_iter);

  
  size_t NumFaviconsForTest() const;
  size_t NumTasksForTest() const;

  
  CancelableTaskTracker cancelable_task_tracker_;

  
  FaviconMap synced_favicons_;

  
  
  RecencySet recent_favicons_;

  
  PageTaskMap page_task_map_;

  
  PageFaviconMap page_favicon_map_;

  Profile* profile_;

  
  

  scoped_ptr<syncer::SyncChangeProcessor> favicon_images_sync_processor_;
  scoped_ptr<syncer::SyncChangeProcessor> favicon_tracking_sync_processor_;

  
  content::NotificationRegistrar notification_registrar_;

  
  const size_t max_sync_favicon_limit_;

  
  base::WeakPtrFactory<FaviconCache> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(FaviconCache);
};

}  

#endif  
