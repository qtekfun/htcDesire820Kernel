// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_EXPIRE_HISTORY_BACKEND_H_
#define CHROME_BROWSER_HISTORY_EXPIRE_HISTORY_BACKEND_H_

#include <queue>
#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/history/history_types.h"

class BookmarkService;
class GURL;
class TestingProfile;

namespace history {

class ArchivedDatabase;
class HistoryDatabase;
struct HistoryDetails;
class ThumbnailDatabase;

class BroadcastNotificationDelegate {
 public:
  
  
  virtual void BroadcastNotifications(int type,
                                      HistoryDetails* details_deleted) = 0;

  
  virtual void NotifySyncURLsDeleted(bool all_history,
                                     bool archived,
                                     URLRows* rows) = 0;

 protected:
  virtual ~BroadcastNotificationDelegate() {}
};

class ExpiringVisitsReader {
 public:
  virtual ~ExpiringVisitsReader() {}
  
  
  virtual bool Read(base::Time end_time, HistoryDatabase* db,
                    VisitVector* visits, int max_visits) const = 0;
};

typedef std::vector<const ExpiringVisitsReader*> ExpiringVisitsReaders;

class ExpireHistoryBackend {
 public:
  
  
  
  
  ExpireHistoryBackend(BroadcastNotificationDelegate* delegate,
                       BookmarkService* bookmark_service);
  ~ExpireHistoryBackend();

  
  void SetDatabases(HistoryDatabase* main_db,
                    ArchivedDatabase* archived_db,
                    ThumbnailDatabase* thumb_db);

  
  
  void StartArchivingOldStuff(base::TimeDelta expiration_threshold);

  
  void DeleteURL(const GURL& url);

  
  void DeleteURLs(const std::vector<GURL>& url);

  
  
  void ExpireHistoryBetween(const std::set<GURL>& restrict_urls,
                            base::Time begin_time, base::Time end_time);

  
  
  
  void ExpireHistoryForTimes(const std::vector<base::Time>& times);

  
  
  void ExpireVisits(const VisitVector& visits);

  
  
  
  
  void ArchiveHistoryBefore(base::Time end_time);

  
  
  
  base::Time GetCurrentArchiveTime() const {
    return base::Time::Now() - expiration_threshold_;
  }

 private:
  FRIEND_TEST_ALL_PREFIXES(ExpireHistoryTest, DeleteFaviconsIfPossible);
  FRIEND_TEST_ALL_PREFIXES(ExpireHistoryTest, ArchiveSomeOldHistory);
  FRIEND_TEST_ALL_PREFIXES(ExpireHistoryTest, ExpiringVisitsReader);
  FRIEND_TEST_ALL_PREFIXES(ExpireHistoryTest, ArchiveSomeOldHistoryWithSource);
  friend class ::TestingProfile;

  struct DeleteDependencies;

  
  
  
  void DeleteVisitRelatedInfo(const VisitVector& visits,
                              DeleteDependencies* dependencies);

  
  void ArchiveVisits(const VisitVector& visits);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void DeleteOneURL(const URLRow& url_row,
                    bool is_bookmarked,
                    DeleteDependencies* dependencies);

  
  
  
  
  
  URLID ArchiveOneURL(const URLRow& url_row);

  
  
  void DeleteURLs(const URLRows& urls,
                  DeleteDependencies* dependencies);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void ExpireURLsForVisits(const VisitVector& visits,
                           DeleteDependencies* dependencies);

  
  
  
  
  
  
  void ArchiveURLsAndVisits(const VisitVector& visits,
                            DeleteDependencies* dependencies);

  
  
  
  
  void DeleteFaviconsIfPossible(const std::set<chrome::FaviconID>& favicon_id,
                                std::set<GURL>* expired_favicons);

  
  enum DeletionType {
    
    DELETION_USER_INITIATED,
    
    
    DELETION_ARCHIVED
  };

  
  void BroadcastDeleteNotifications(DeleteDependencies* dependencies,
                                    DeletionType type);

  
  void ScheduleArchive();

  
  
  
  void DoArchiveIteration();

  
  
  
  
  bool ArchiveSomeOldHistory(base::Time end_time,
                             const ExpiringVisitsReader* reader,
                             int max_visits);

  
  
  void ParanoidExpireHistory();

  
  
  BookmarkService* GetBookmarkService();

  
  
  void InitWorkQueue();

  
  
  const ExpiringVisitsReader* GetAllVisitsReader();

  
  
  const ExpiringVisitsReader* GetAutoSubframeVisitsReader();

  
  BroadcastNotificationDelegate* delegate_;

  
  HistoryDatabase* main_db_;       
  ArchivedDatabase* archived_db_;  
  ThumbnailDatabase* thumb_db_;    

  
  
  base::WeakPtrFactory<ExpireHistoryBackend> weak_factory_;

  
  
  base::TimeDelta expiration_threshold_;

  
  
  ExpiringVisitsReaders readers_;

  
  
  
  std::queue<const ExpiringVisitsReader*> work_queue_;

  
  
  
  scoped_ptr<ExpiringVisitsReader> all_visits_reader_;
  scoped_ptr<ExpiringVisitsReader> auto_subframe_visits_reader_;

  
  
  
  
  BookmarkService* bookmark_service_;

  DISALLOW_COPY_AND_ASSIGN(ExpireHistoryBackend);
};

}  

#endif  
