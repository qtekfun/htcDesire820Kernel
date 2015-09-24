// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_HISTORY_BACKEND_H_
#define CHROME_BROWSER_HISTORY_HISTORY_BACKEND_H_
#pragma once

#include <string>
#include <utility>

#include "app/sql/init_status.h"
#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/history/archived_database.h"
#include "chrome/browser/history/expire_history_backend.h"
#include "chrome/browser/history/history_database.h"
#include "chrome/browser/history/history_marshaling.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/text_database_manager.h"
#include "chrome/browser/history/thumbnail_database.h"
#include "chrome/browser/history/visit_tracker.h"
#include "chrome/browser/search_engines/template_url_id.h"
#include "content/common/mru_cache.h"

class BookmarkService;
struct DownloadCreateInfo;
class TestingProfile;
struct ThumbnailScore;

namespace history {

class CommitLaterTask;
class HistoryPublisher;

class HistoryBackend : public base::RefCountedThreadSafe<HistoryBackend>,
                       public BroadcastNotificationDelegate {
 public:
  
  
  
  
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void NotifyProfileError(sql::InitStatus init_status) = 0;

    
    
    
    
    
    
    
    
    
    virtual void SetInMemoryBackend(InMemoryHistoryBackend* backend) = 0;

    
    
    
    
    
    virtual void BroadcastNotifications(NotificationType type,
                                        HistoryDetails* details) = 0;

    
    virtual void DBLoaded() = 0;

    
    virtual void StartTopSitesMigration() = 0;
  };

  
  
  
  
  
  
  
  
  
  
  
  
  HistoryBackend(const FilePath& history_dir,
                 Delegate* delegate,
                 BookmarkService* bookmark_service);

  
  
  
  
  
  
  
  void Init(const std::string& languages, bool force_fail);

  
  
  
  void Closing();

  
  void NotifyRenderProcessHostDestruction(const void* host);

  

  void AddPage(scoped_refptr<HistoryAddPageArgs> request);
  virtual void SetPageTitle(const GURL& url, const string16& title);
  void AddPageNoVisitForBookmark(const GURL& url);

  

  void SetPageContents(const GURL& url, const string16& contents);

  

  
  
  
  void ScheduleAutocomplete(HistoryURLProvider* provider,
                            HistoryURLProviderParams* params);

  void IterateURLs(HistoryService::URLEnumerator* enumerator);
  void QueryURL(scoped_refptr<QueryURLRequest> request,
                const GURL& url,
                bool want_visits);
  void QueryHistory(scoped_refptr<QueryHistoryRequest> request,
                    const string16& text_query,
                    const QueryOptions& options);
  void QueryRedirectsFrom(scoped_refptr<QueryRedirectsRequest> request,
                          const GURL& url);
  void QueryRedirectsTo(scoped_refptr<QueryRedirectsRequest> request,
                        const GURL& url);

  void GetVisitCountToHost(scoped_refptr<GetVisitCountToHostRequest> request,
                           const GURL& url);

  
  void QueryTopURLsAndRedirects(
      scoped_refptr<QueryTopURLsAndRedirectsRequest> request,
      int result_count);

  
  
  
  void QueryMostVisitedURLs(
      scoped_refptr<QueryMostVisitedURLsRequest> request,
      int result_count,
      int days_back);

  
  void QueryMostVisitedURLsImpl(int result_count,
                                int days_back,
                                MostVisitedURLList* result);

  
  
  
  
  
  
  
  bool GetMostRecentRedirectsFrom(const GURL& url,
                                  history::RedirectList* redirects);

  
  
  
  
  bool GetMostRecentRedirectsTo(const GURL& url,
                                      history::RedirectList* redirects);

  

  void SetPageThumbnail(const GURL& url,
                        const SkBitmap& thumbnail,
                        const ThumbnailScore& score);

  
  
  void GetPageThumbnail(scoped_refptr<GetPageThumbnailRequest> request,
                        const GURL& page_url);

  
  
  
  
  
  void GetPageThumbnailDirectly(
      const GURL& page_url,
      scoped_refptr<RefCountedBytes>* data);

  void MigrateThumbnailsDatabase();

  

  void GetFavicon(scoped_refptr<GetFaviconRequest> request,
                  const GURL& icon_url,
                  int icon_types);

  void GetFaviconForURL(scoped_refptr<GetFaviconRequest> request,
                        const GURL& page_url,
                        int icon_types);

  void SetFavicon(const GURL& page_url,
                  const GURL& icon_url,
                  scoped_refptr<RefCountedMemory> data,
                  IconType icon_type);

  void UpdateFaviconMappingAndFetch(scoped_refptr<GetFaviconRequest> request,
                                    const GURL& page_url,
                                    const GURL& icon_url,
                                    IconType icon_type);

  void SetFaviconOutOfDateForPage(const GURL& page_url);

  void SetImportedFavicons(
      const std::vector<ImportedFaviconUsage>& favicon_usage);

  

  void QueryDownloads(scoped_refptr<DownloadQueryRequest> request);
  void CleanUpInProgressEntries();
  void UpdateDownload(int64 received_bytes, int32 state, int64 db_handle);
  void UpdateDownloadPath(const FilePath& path, int64 db_handle);
  void CreateDownload(scoped_refptr<DownloadCreateRequest> request,
                      const DownloadCreateInfo& info);
  void RemoveDownload(int64 db_handle);
  void RemoveDownloadsBetween(const base::Time remove_begin,
                              const base::Time remove_end);
  void RemoveDownloads(const base::Time remove_end);

  

  void QuerySegmentUsage(scoped_refptr<QuerySegmentUsageRequest> request,
                         const base::Time from_time,
                         int max_result_count);
  void DeleteOldSegmentData();
  void SetSegmentPresentationIndex(SegmentID segment_id, int index);

  

  void SetKeywordSearchTermsForURL(const GURL& url,
                                   TemplateURLID keyword_id,
                                   const string16& term);

  void DeleteAllSearchTermsForKeyword(TemplateURLID keyword_id);

  void GetMostRecentKeywordSearchTerms(
      scoped_refptr<GetMostRecentKeywordSearchTermsRequest> request,
      TemplateURLID keyword_id,
      const string16& prefix,
      int max_count);

  

  void ProcessDBTask(scoped_refptr<HistoryDBTaskRequest> request);

  virtual bool GetAllTypedURLs(std::vector<history::URLRow>* urls);

  virtual bool GetVisitsForURL(URLID id, VisitVector* visits);

  virtual bool UpdateURL(URLID id, const history::URLRow& url);

  
  virtual bool AddVisits(const GURL& url,
                         const std::vector<base::Time>& visits,
                         VisitSource visit_source);

  virtual bool RemoveVisits(const VisitVector& visits);

  virtual bool GetURL(const GURL& url, history::URLRow* url_row);

  

  virtual void DeleteURLs(const std::vector<GURL>& urls);

  virtual void DeleteURL(const GURL& url);

  
  void ExpireHistoryBetween(scoped_refptr<ExpireHistoryRequest> request,
                            const std::set<GURL>& restrict_urls,
                            base::Time begin_time,
                            base::Time end_time);

  

  
  
  void URLsNoLongerBookmarked(const std::set<GURL>& urls);

  

  
  
  
  void SetOnBackendDestroyTask(MessageLoop* message_loop, Task* task);

  
  
  
  void AddPagesWithDetails(const std::vector<URLRow>& info,
                           VisitSource visit_source);

#if defined(UNIT_TEST)
  HistoryDatabase* db() const { return db_.get(); }

  ExpireHistoryBackend* expire_backend() { return &expirer_; }
#endif

 protected:
  virtual ~HistoryBackend();

 private:
  friend class base::RefCountedThreadSafe<HistoryBackend>;
  friend class CommitLaterTask;  
  friend class HistoryBackendTest;
  friend class HistoryTest;  
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, DeleteAll);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, ImportedFaviconsTest);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, URLsNoLongerBookmarked);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, StripUsernamePasswordTest);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, DeleteThumbnailsDatabaseTest);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, AddPageVisitSource);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, AddPageArgsSource);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, AddVisitsSource);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, RemoveVisitsSource);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, MigrationVisitSource);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, MigrationIconMapping);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, SetFaviconMapping);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, AddOrUpdateIconMapping);

  friend class ::TestingProfile;

  
  FilePath GetThumbnailFileName() const;

  
  
  
  FilePath GetFaviconsFileName() const;
  FilePath GetArchivedFileName() const;

  class URLQuerier;
  friend class URLQuerier;

  
  void InitImpl(const std::string& languages);

  
  
  
  
  
  
  
  std::pair<URLID, VisitID> AddPageVisit(const GURL& url,
                                         base::Time time,
                                         VisitID referring_visit,
                                         PageTransition::Type transition,
                                         VisitSource visit_source);

  
  
  
  void GetRedirectsFromSpecificVisit(
      VisitID cur_visit, history::RedirectList* redirects);

  
  
  void GetRedirectsToSpecificVisit(
      VisitID cur_visit, history::RedirectList* redirects);

  

  
  
  
  
  
  bool GetThumbnailFromOlderRedirect(
      const GURL& page_url, std::vector<unsigned char>* data);

  

  
  
  
  
  void QueryHistoryBasic(URLDatabase* url_db, VisitDatabase* visit_db,
                         const QueryOptions& options, QueryResults* result);
  void QueryHistoryFTS(const string16& text_query,
                       const QueryOptions& options,
                       QueryResults* result);

  

  
  
  
  
  
  
  void Commit();

  
  
  
  void ScheduleCommit();

  
  
  void CancelScheduledCommit();

  

  
  
  SegmentID GetLastSegmentID(VisitID from_visit);

  
  
  SegmentID UpdateSegments(const GURL& url,
                           VisitID from_visit,
                           VisitID visit_id,
                           PageTransition::Type transition_type,
                           const base::Time ts);

  

  
  
  
  
  
  void UpdateFaviconMappingAndFetchImpl(
      const GURL* page_url,
      const GURL& icon_url,
      scoped_refptr<GetFaviconRequest> request,
      int icon_type);

  
  
  void SetFaviconMapping(const GURL& page_url,
                         FaviconID id,
                         IconType icon_type);

  
  
  
  
  
  bool AddOrUpdateIconMapping(const GURL& page_url,
                              FaviconID id,
                              IconType icon_type,
                              FaviconID* replaced_icon_id);

  

  
  
  void ProcessDBTaskImpl();

  
  void ReleaseDBTasks();

  
  
  
  virtual void BroadcastNotifications(NotificationType type,
                                      HistoryDetails* details_deleted);

  

  
  
  
  
  
  
  void DeleteAllHistory();

  
  
  
  
  bool ClearAllThumbnailHistory(std::vector<URLRow>* kept_urls);

  
  
  
  
  
  bool ClearAllMainHistory(const std::vector<URLRow>& kept_urls);

  
  
  BookmarkService* GetBookmarkService();

  

  
  
  
  scoped_ptr<Delegate> delegate_;

  
  FilePath history_dir_;

  
  
  
  
  scoped_ptr<HistoryDatabase> db_;
  scoped_ptr<ThumbnailDatabase> thumbnail_db_;

  
  scoped_ptr<ArchivedDatabase> archived_db_;

  
  
  scoped_ptr<TextDatabaseManager> text_database_;

  
  ExpireHistoryBackend expirer_;

  
  
  
  
  scoped_refptr<CommitLaterTask> scheduled_commit_;

  
  
  
  
  
  
  
  
  
  typedef MRUCache<GURL, history::RedirectList> RedirectCache;
  RedirectCache recent_redirects_;

  
  
  
  
  
  base::Time last_requested_time_;

  
  
  
  
  base::Time last_recorded_time_;

  
  base::Time first_recorded_time_;

  
  MessageLoop* backend_destroy_message_loop_;
  Task* backend_destroy_task_;

  
  VisitTracker tracker_;

  
  
  bool segment_queried_;

  
  
  std::list<HistoryDBTaskRequest*> db_task_requests_;

  
  
  
  
  
  BookmarkService* bookmark_service_;

  
  
  scoped_ptr<HistoryPublisher> history_publisher_;

  DISALLOW_COPY_AND_ASSIGN(HistoryBackend);
};

}  

#endif  
