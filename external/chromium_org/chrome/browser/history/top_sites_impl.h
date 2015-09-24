// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_TOP_SITES_IMPL_H_
#define CHROME_BROWSER_HISTORY_TOP_SITES_IMPL_H_

#include <list>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/history_service.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/page_usage_data.h"
#include "chrome/browser/history/top_sites.h"
#include "chrome/browser/history/top_sites_backend.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "chrome/common/thumbnail_score.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/image/image.h"
#include "url/gurl.h"

class Profile;

namespace base {
class FilePath;
class RefCountedBytes;
class RefCountedMemory;
}

namespace history {

class TopSitesCache;
class TopSitesImplTest;

class TopSitesImpl : public TopSites {
 public:
  explicit TopSitesImpl(Profile* profile);

  
  void Init(const base::FilePath& db_name);

  virtual bool SetPageThumbnail(const GURL& url,
                                const gfx::Image& thumbnail,
                                const ThumbnailScore& score) OVERRIDE;
  virtual bool SetPageThumbnailToJPEGBytes(
      const GURL& url,
      const base::RefCountedMemory* memory,
      const ThumbnailScore& score) OVERRIDE;
  virtual void GetMostVisitedURLs(
      const GetMostVisitedURLsCallback& callback,
      bool include_forced_urls) OVERRIDE;
  virtual bool GetPageThumbnail(
      const GURL& url,
      bool prefix_match,
      scoped_refptr<base::RefCountedMemory>* bytes) OVERRIDE;
  virtual bool GetPageThumbnailScore(const GURL& url,
                                     ThumbnailScore* score) OVERRIDE;
  virtual bool GetTemporaryPageThumbnailScore(const GURL& url,
                                              ThumbnailScore* score) OVERRIDE;
  virtual void SyncWithHistory() OVERRIDE;
  virtual bool HasBlacklistedItems() const OVERRIDE;
  virtual void AddBlacklistedURL(const GURL& url) OVERRIDE;
  virtual void RemoveBlacklistedURL(const GURL& url) OVERRIDE;
  virtual bool IsBlacklisted(const GURL& url) OVERRIDE;
  virtual void ClearBlacklistedURLs() OVERRIDE;
  virtual void Shutdown() OVERRIDE;
  virtual CancelableRequestProvider::Handle StartQueryForMostVisited() OVERRIDE;
  virtual bool IsKnownURL(const GURL& url) OVERRIDE;
  virtual const std::string& GetCanonicalURLString(
      const GURL& url) const OVERRIDE;
  virtual bool IsNonForcedFull() OVERRIDE;
  virtual bool IsForcedFull() OVERRIDE;
  virtual MostVisitedURLList GetPrepopulatePages() OVERRIDE;
  virtual bool loaded() const OVERRIDE;
  virtual bool AddForcedURL(const GURL& url, const base::Time& time) OVERRIDE;

 protected:
  virtual ~TopSitesImpl();

 private:
  friend class TopSitesImplTest;
  FRIEND_TEST_ALL_PREFIXES(TopSitesImplTest, DiffMostVisited);
  FRIEND_TEST_ALL_PREFIXES(TopSitesImplTest, DiffMostVisitedWithForced);

  typedef base::Callback<void(const MostVisitedURLList&,
                              const MostVisitedURLList&)> PendingCallback;

  typedef std::pair<GURL, Images> TempImage;
  typedef std::list<TempImage> TempImages;
  typedef std::vector<PendingCallback> PendingCallbacks;

  
  
  
  
  
  
  
  
  
  
  static void DiffMostVisited(const MostVisitedURLList& old_list,
                              const MostVisitedURLList& new_list,
                              TopSitesDelta* delta);

  
  
  
  bool SetPageThumbnailNoDB(const GURL& url,
                            const base::RefCountedMemory* thumbnail_data,
                            const ThumbnailScore& score);

  
  
  bool SetPageThumbnailEncoded(const GURL& url,
                               const base::RefCountedMemory* thumbnail,
                               const ThumbnailScore& score);

  
  
  static bool EncodeBitmap(const gfx::Image& bitmap,
                           scoped_refptr<base::RefCountedBytes>* bytes);

  
  
  void RemoveTemporaryThumbnailByURL(const GURL& url);

  
  void AddTemporaryThumbnail(const GURL& url,
                             const base::RefCountedMemory* thumbnail,
                             const ThumbnailScore& score);

  
  void TimerFired();

  
  
  
  static int GetRedirectDistanceForURL(const MostVisitedURL& most_visited,
                                       const GURL& url);

  
  
  bool AddPrepopulatedPages(MostVisitedURLList* urls,
                            size_t num_forced_urls);

  
  
  
  
  
  
  size_t MergeCachedForcedURLs(MostVisitedURLList* new_list);

  
  
  
  void ApplyBlacklist(const MostVisitedURLList& urls, MostVisitedURLList* out);

  
  std::string GetURLHash(const GURL& url);

  
  
  base::TimeDelta GetUpdateDelay();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  
  
  
  void SetTopSites(const MostVisitedURLList& new_top_sites);

  
  
  
  int num_results_to_request_from_history() const;

  
  
  void MoveStateToLoaded();

  void ResetThreadSafeCache();

  void ResetThreadSafeImageCache();

  void NotifyTopSitesChanged();

  
  void RestartQueryForTopSitesTimer(base::TimeDelta delta);

  
  
  void OnGotMostVisitedThumbnails(
      const scoped_refptr<MostVisitedThumbnails>& thumbnails);

  
  void OnTopSitesAvailableFromHistory(CancelableRequestProvider::Handle handle,
                                      MostVisitedURLList data);

  scoped_refptr<TopSitesBackend> backend_;

  
  scoped_ptr<TopSitesCache> cache_;

  
  
  
  scoped_ptr<TopSitesCache> thread_safe_cache_;

  Profile* profile_;

  
  mutable base::Lock lock_;

  
  
  CancelableRequestConsumer history_consumer_;
  CancelableTaskTracker cancelable_task_tracker_;

  
  
  base::OneShotTimer<TopSitesImpl> timer_;

  
  base::TimeTicks timer_start_time_;

  content::NotificationRegistrar registrar_;

  
  size_t last_num_urls_changed_;

  
  
  
  PendingCallbacks pending_callbacks_;

  
  
  
  
  TempImages temp_images_;

  
  std::vector<GURL> prepopulated_page_urls_;

  
  bool loaded_;

  DISALLOW_COPY_AND_ASSIGN(TopSitesImpl);
};

}  

#endif  
