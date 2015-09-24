// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_TYPED_URL_SYNCABLE_SERVICE_H_
#define CHROME_BROWSER_HISTORY_TYPED_URL_SYNCABLE_SERVICE_H_

#include <set>
#include <vector>

#include "chrome/browser/history/history_notifications.h"
#include "chrome/browser/history/history_types.h"
#include "content/public/browser/notification_types.h"
#include "sync/api/sync_change.h"
#include "sync/api/sync_data.h"
#include "sync/api/sync_error.h"
#include "sync/api/sync_error_factory.h"
#include "sync/api/syncable_service.h"

class GURL;
class TypedUrlSyncableServiceTest;

namespace base {
class MessageLoop;
};

namespace sync_pb {
class TypedUrlSpecifics;
};

namespace history {

class HistoryBackend;
class URLRow;

extern const char kTypedUrlTag[];

class TypedUrlSyncableService : public syncer::SyncableService {
 public:
  explicit TypedUrlSyncableService(HistoryBackend* history_backend);
  virtual ~TypedUrlSyncableService();

  static syncer::ModelType model_type() { return syncer::TYPED_URLS; }

  
  virtual syncer::SyncMergeResult MergeDataAndStartSyncing(
      syncer::ModelType type,
      const syncer::SyncDataList& initial_sync_data,
      scoped_ptr<syncer::SyncChangeProcessor> sync_processor,
      scoped_ptr<syncer::SyncErrorFactory> error_handler) OVERRIDE;
  virtual void StopSyncing(syncer::ModelType type) OVERRIDE;
  virtual syncer::SyncDataList GetAllSyncData(
      syncer::ModelType type) const OVERRIDE;
  virtual syncer::SyncError ProcessSyncChanges(
      const tracked_objects::Location& from_here,
      const syncer::SyncChangeList& change_list) OVERRIDE;

  
  void OnUrlsModified(URLRows* changed_urls);
  void OnUrlVisited(content::PageTransition transition, URLRow* row);
  void OnUrlsDeleted(bool all_history, bool archived, URLRows* rows);

 protected:
  void GetSyncedUrls(std::set<GURL>* urls) {
    urls->insert(synced_typed_urls_.begin(), synced_typed_urls_.end());
  }

 private:
  typedef std::vector<std::pair<URLID, URLRow> > TypedUrlUpdateVector;
  typedef std::vector<std::pair<GURL, std::vector<VisitInfo> > >
      TypedUrlVisitVector;

  
  
  typedef std::map<GURL, std::pair<syncer::SyncChange::SyncChangeType,
                                   URLRows::iterator> > TypedUrlMap;
  
  
  typedef std::map<GURL, VisitVector> UrlVisitVectorMap;

  
  
  bool ShouldIgnoreUrl(const GURL& url);

  
  
  
  
  bool ShouldSyncVisit(content::PageTransition transition, URLRow* row);

  
  
  
  bool CreateOrUpdateSyncNode(URLRow typed_url,
                              syncer::SyncChangeList* changes);

  void AddTypedUrlToChangeList(
    syncer::SyncChange::SyncChangeType change_type,
    const URLRow& row,
    const VisitVector& visits,
    std::string title,
    syncer::SyncChangeList* change_list);

  
  
  static void WriteToTypedUrlSpecifics(const URLRow& url,
                                       const VisitVector& visits,
                                       sync_pb::TypedUrlSpecifics* specifics);

  
  
  
  
  
  
  
  virtual bool FixupURLAndGetVisits(URLRow* url,
                                    VisitVector* visits);

  
  
  std::set<GURL> synced_typed_urls_;

  HistoryBackend* const history_backend_;

  
  
  bool processing_syncer_changes_;

  
  
  scoped_ptr<syncer::SyncChangeProcessor> sync_processor_;
  scoped_ptr<syncer::SyncErrorFactory> sync_error_handler_;

  
  
  int num_db_accesses_;
  int num_db_errors_;

  base::MessageLoop* expected_loop_;

  FRIEND_TEST_ALL_PREFIXES(TypedUrlSyncableServiceTest,
                           AddLocalTypedUrlAndSync);
  FRIEND_TEST_ALL_PREFIXES(TypedUrlSyncableServiceTest,
                           UpdateLocalTypedUrlAndSync);
  FRIEND_TEST_ALL_PREFIXES(TypedUrlSyncableServiceTest,
                           LinkVisitLocalTypedUrlAndSync);
  FRIEND_TEST_ALL_PREFIXES(TypedUrlSyncableServiceTest,
                           TypedVisitLocalTypedUrlAndSync);
  FRIEND_TEST_ALL_PREFIXES(TypedUrlSyncableServiceTest,
                           DeleteLocalTypedUrlAndSync);
  FRIEND_TEST_ALL_PREFIXES(TypedUrlSyncableServiceTest,
                           DeleteAllLocalTypedUrlAndSync);
  FRIEND_TEST_ALL_PREFIXES(TypedUrlSyncableServiceTest,
                           MaxVisitLocalTypedUrlAndSync);
  FRIEND_TEST_ALL_PREFIXES(TypedUrlSyncableServiceTest,
                           ThrottleVisitLocalTypedUrlSync);

  DISALLOW_COPY_AND_ASSIGN(TypedUrlSyncableService);
};

}  

#endif  
