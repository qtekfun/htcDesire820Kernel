// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_TOP_SITES_H_
#define CHROME_BROWSER_HISTORY_TOP_SITES_H_
#pragma once

#include <list>
#include <set>
#include <string>
#include <utility>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/ref_counted_memory.h"
#include "base/synchronization/lock.h"
#include "base/time.h"
#include "base/timer.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/history.h"
#include "chrome/browser/history/page_usage_data.h"
#include "chrome/common/thumbnail_score.h"
#include "content/browser/cancelable_request.h"
#include "googleurl/src/gurl.h"

class DictionaryValue;
class FilePath;
class SkBitmap;
class Profile;

namespace history {

class TopSitesCache;
class TopSitesBackend;
class TopSitesTest;

class TopSites
    : public base::RefCountedThreadSafe<TopSites>,
      public NotificationObserver,
      public CancelableRequestProvider {
 public:
  explicit TopSites(Profile* profile);

  
  void Init(const FilePath& db_name);

  
  
  
  bool SetPageThumbnail(const GURL& url,
                        const SkBitmap& thumbnail,
                        const ThumbnailScore& score);

  
  typedef Callback1<const MostVisitedURLList&>::Type GetTopSitesCallback;
  typedef std::set<scoped_refptr<CancelableRequest<GetTopSitesCallback> > >
      PendingCallbackSet;

  
  
  
  void GetMostVisitedURLs(CancelableRequestConsumer* consumer,
                          GetTopSitesCallback* callback);

  
  
  
  
  bool GetPageThumbnail(const GURL& url,
                        scoped_refptr<RefCountedBytes>* bytes);

  
  
  
  virtual bool GetPageThumbnailScore(const GURL& url, ThumbnailScore* score);

  
  
  
  bool GetTemporaryPageThumbnailScore(const GURL& url, ThumbnailScore* score);

  
  
  void MigrateFromHistory();

  
  void FinishHistoryMigration(const ThumbnailMigration& data);

  
  
  void HistoryLoaded();

  

  
  bool HasBlacklistedItems() const;

  
  void AddBlacklistedURL(const GURL& url);

  
  void RemoveBlacklistedURL(const GURL& url);

  
  bool IsBlacklisted(const GURL& url);

  
  void ClearBlacklistedURLs();

  

  
  void AddPinnedURL(const GURL& url, size_t index);

  
  bool IsURLPinned(const GURL& url);

  
  void RemovePinnedURL(const GURL& url);

  
  
  bool GetPinnedURLAtIndex(size_t index, GURL* out);

  
  void Shutdown();

  
  
  
  
  
  
  
  
  static void DiffMostVisited(const MostVisitedURLList& old_list,
                              const MostVisitedURLList& new_list,
                              TopSitesDelta* delta);

  
  
  
  CancelableRequestProvider::Handle StartQueryForMostVisited();

  bool loaded() const { return loaded_; }

  
  
  virtual bool IsKnownURL(const GURL& url);

  
  
  
  virtual bool IsFull();

 protected:
  
  virtual ~TopSites();

 private:
  friend class base::RefCountedThreadSafe<TopSites>;
  friend class TopSitesTest;

  typedef std::pair<GURL, Images> TempImage;
  typedef std::list<TempImage> TempImages;

  
  enum HistoryLoadState {
    
    HISTORY_LOADING,

    
    HISTORY_MIGRATING,

    
    HISTORY_LOADED
  };

  
  enum TopSitesLoadState {
    
    TOP_SITES_LOADING,

    
    
    
    TOP_SITES_LOADED_WAITING_FOR_HISTORY,

    
    TOP_SITES_LOADED
  };

  
  
  
  bool SetPageThumbnailNoDB(const GURL& url,
                            const RefCountedBytes* thumbnail_data,
                            const ThumbnailScore& score);

  
  
  bool SetPageThumbnailEncoded(const GURL& url,
                               const RefCountedBytes* thumbnail,
                               const ThumbnailScore& score);

  
  
  static bool EncodeBitmap(const SkBitmap& bitmap,
                           scoped_refptr<RefCountedBytes>* bytes);

  
  
  void RemoveTemporaryThumbnailByURL(const GURL& url);

  
  void AddTemporaryThumbnail(const GURL& url,
                             const RefCountedBytes* thumbnail,
                             const ThumbnailScore& score);

  
  void TimerFired();

  
  
  
  static int GetRedirectDistanceForURL(const MostVisitedURL& most_visited,
                                       const GURL& url);

  
  static MostVisitedURLList GetPrepopulatePages();

  
  
  static bool AddPrepopulatedPages(MostVisitedURLList* urls);

  
  
  void MigratePinnedURLs();

  
  
  void ApplyBlacklistAndPinnedURLs(const MostVisitedURLList& urls,
                                   MostVisitedURLList* out);

  
  std::string GetURLString(const GURL& url);

  
  std::string GetURLHash(const GURL& url);

  
  
  base::TimeDelta GetUpdateDelay();

  
  
  static void ProcessPendingCallbacks(
      const PendingCallbackSet& pending_callbacks,
      const MostVisitedURLList& urls);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  void SetTopSites(const MostVisitedURLList& new_top_sites);

  
  
  int num_results_to_request_from_history() const;

  
  void MoveStateToLoaded();

  void ResetThreadSafeCache();

  void ResetThreadSafeImageCache();

  
  void RestartQueryForTopSitesTimer(base::TimeDelta delta);

  
  
  void OnHistoryMigrationWrittenToDisk(
      CancelableRequestProvider::Handle handle);

  
  void OnGotMostVisitedThumbnails(CancelableRequestProvider::Handle handle,
                                  scoped_refptr<MostVisitedThumbnails> data,
                                  bool may_need_history_migration);

  
  void OnTopSitesAvailableFromHistory(CancelableRequestProvider::Handle handle,
                                      MostVisitedURLList data);

  scoped_refptr<TopSitesBackend> backend_;

  
  scoped_ptr<TopSitesCache> cache_;

  
  
  
  scoped_ptr<TopSitesCache> thread_safe_cache_;

  Profile* profile_;

  
  mutable base::Lock lock_;

  CancelableRequestConsumer cancelable_consumer_;

  
  
  base::OneShotTimer<TopSites> timer_;

  
  base::TimeTicks timer_start_time_;

  NotificationRegistrar registrar_;

  
  size_t last_num_urls_changed_;

  
  
  
  PendingCallbackSet pending_callbacks_;

  
  
  
  
  TempImages temp_images_;

  

  
  
  
  
  
  const DictionaryValue* blacklist_;

  
  
  
  const DictionaryValue* pinned_urls_;

  
  HistoryLoadState history_state_;

  
  TopSitesLoadState top_sites_state_;

  
  bool loaded_;

  DISALLOW_COPY_AND_ASSIGN(TopSites);
};

}  

#endif  
