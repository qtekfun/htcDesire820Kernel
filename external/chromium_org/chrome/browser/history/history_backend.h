// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_HISTORY_BACKEND_H_
#define CHROME_BROWSER_HISTORY_HISTORY_BACKEND_H_

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/containers/mru_cache.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/memory_pressure_listener.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/history/archived_database.h"
#include "chrome/browser/history/expire_history_backend.h"
#include "chrome/browser/history/history_database.h"
#include "chrome/browser/history/history_marshaling.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/thumbnail_database.h"
#include "chrome/browser/history/visit_tracker.h"
#include "chrome/browser/search_engines/template_url_id.h"
#include "sql/init_status.h"
#include "ui/base/layout.h"

class BookmarkService;
class TestingProfile;
class TypedUrlSyncableService;
struct ThumbnailScore;

namespace history {
#if defined(OS_ANDROID)
class AndroidProviderBackend;
#endif

class CommitLaterTask;
class VisitFilter;
struct DownloadRow;

static const size_t kMaxFaviconsPerPage = 8;

static const size_t kMaxFaviconBitmapsPerIconURL = 8;

class HistoryBackend : public base::RefCountedThreadSafe<HistoryBackend>,
                       public BroadcastNotificationDelegate {
 public:
  
  
  
  
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void NotifyProfileError(int backend_id,
                                    sql::InitStatus init_status) = 0;

    
    
    
    
    
    
    
    
    
    virtual void SetInMemoryBackend(int backend_id,
                                    InMemoryHistoryBackend* backend) = 0;

    
    
    
    
    
    
    virtual void BroadcastNotifications(int type,
                                        HistoryDetails* details) = 0;

    
    virtual void DBLoaded(int backend_id) = 0;

    virtual void NotifyVisitDBObserversOnAddVisit(
        const history::BriefVisitInfo& info) = 0;
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  HistoryBackend(const base::FilePath& history_dir,
                 int id,
                 Delegate* delegate,
                 BookmarkService* bookmark_service);

  
  
  
  
  
  
  
  void Init(const std::string& languages, bool force_fail);

  
  
  
  void Closing();

  
  void NotifyRenderProcessHostDestruction(const void* host);

  

  
  void AddPage(const HistoryAddPageArgs& request);
  virtual void SetPageTitle(const GURL& url, const base::string16& title);
  void AddPageNoVisitForBookmark(const GURL& url, const base::string16& title);

  
  
  
  
  
  
  void UpdateWithPageEndTime(const void* host,
                             int32 page_id,
                             const GURL& url,
                             base::Time end_ts);

  

  
  
  
  void ScheduleAutocomplete(HistoryURLProvider* provider,
                            HistoryURLProviderParams* params);

  void IterateURLs(
      const scoped_refptr<visitedlink::VisitedLinkDelegate::URLEnumerator>&
          enumerator);
  void QueryURL(scoped_refptr<QueryURLRequest> request,
                const GURL& url,
                bool want_visits);
  void QueryHistory(scoped_refptr<QueryHistoryRequest> request,
                    const base::string16& text_query,
                    const QueryOptions& options);
  void QueryRedirectsFrom(scoped_refptr<QueryRedirectsRequest> request,
                          const GURL& url);
  void QueryRedirectsTo(scoped_refptr<QueryRedirectsRequest> request,
                        const GURL& url);

  void GetVisibleVisitCountToHost(
      scoped_refptr<GetVisibleVisitCountToHostRequest> request,
      const GURL& url);

  
  void QueryTopURLsAndRedirects(
      scoped_refptr<QueryTopURLsAndRedirectsRequest> request,
      int result_count);

  
  
  
  void QueryMostVisitedURLs(
      scoped_refptr<QueryMostVisitedURLsRequest> request,
      int result_count,
      int days_back);

  
  
  
  void QueryFilteredURLs(
      scoped_refptr<QueryFilteredURLsRequest> request,
      int result_count,
      const history::VisitFilter& filter,
      bool debug);

  
  void QueryMostVisitedURLsImpl(int result_count,
                                int days_back,
                                MostVisitedURLList* result);

  
  
  
  
  
  
  
  bool GetMostRecentRedirectsFrom(const GURL& url,
                                  history::RedirectList* redirects);

  
  
  
  
  bool GetMostRecentRedirectsTo(const GURL& url,
                                history::RedirectList* redirects);

  

  void GetFavicons(const std::vector<GURL>& icon_urls,
                    int icon_types,
                    int desired_size_in_dip,
                    const std::vector<ui::ScaleFactor>& desired_scale_factors,
                    std::vector<chrome::FaviconBitmapResult>* bitmap_results);

  void GetLargestFaviconForURL(
      const GURL& page_url,
      const std::vector<int>& icon_types,
      int minimum_size_in_pixels,
      chrome::FaviconBitmapResult* bitmap_result);

  void GetFaviconsForURL(
      const GURL& page_url,
      int icon_types,
      int desired_size_in_dip,
      const std::vector<ui::ScaleFactor>& desired_scale_factors,
      std::vector<chrome::FaviconBitmapResult>* bitmap_results);

  void GetFaviconForID(
      chrome::FaviconID favicon_id,
      int desired_size_in_dip,
      ui::ScaleFactor desired_scale_factor,
      std::vector<chrome::FaviconBitmapResult>* bitmap_results);

  void UpdateFaviconMappingsAndFetch(
      const GURL& page_url,
      const std::vector<GURL>& icon_urls,
      int icon_types,
      int desired_size_in_dip,
      const std::vector<ui::ScaleFactor>& desired_scale_factors,
      std::vector<chrome::FaviconBitmapResult>* bitmap_results);

  void MergeFavicon(const GURL& page_url,
                    const GURL& icon_url,
                    chrome::IconType icon_type,
                    scoped_refptr<base::RefCountedMemory> bitmap_data,
                    const gfx::Size& pixel_size);

  void SetFavicons(
      const GURL& page_url,
      chrome::IconType icon_type,
      const std::vector<chrome::FaviconBitmapData>& favicon_bitmap_data);

  void SetFaviconsOutOfDateForPage(const GURL& page_url);

  void CloneFavicons(const GURL& old_page_url, const GURL& new_page_url);

  void SetImportedFavicons(
      const std::vector<ImportedFaviconUsage>& favicon_usage);

  

  void GetNextDownloadId(uint32* next_id);
  void QueryDownloads(std::vector<DownloadRow>* rows);
  void UpdateDownload(const DownloadRow& data);
  void CreateDownload(const history::DownloadRow& history_info,
                      bool* success);
  void RemoveDownloads(const std::set<uint32>& ids);

  

  void QuerySegmentUsage(scoped_refptr<QuerySegmentUsageRequest> request,
                         const base::Time from_time,
                         int max_result_count);
  void DeleteOldSegmentData();

  void IncreaseSegmentDuration(const GURL& url,
                               base::Time time,
                               base::TimeDelta delta);

  void QuerySegmentDuration(scoped_refptr<QuerySegmentUsageRequest> request,
                            const base::Time from_time,
                            int max_result_count);

  

  void SetKeywordSearchTermsForURL(const GURL& url,
                                   TemplateURLID keyword_id,
                                   const base::string16& term);

  void DeleteAllSearchTermsForKeyword(TemplateURLID keyword_id);

  void GetMostRecentKeywordSearchTerms(
      scoped_refptr<GetMostRecentKeywordSearchTermsRequest> request,
      TemplateURLID keyword_id,
      const base::string16& prefix,
      int max_count);

  void DeleteKeywordSearchTermForURL(const GURL& url);

  void DeleteMatchingURLsForKeyword(TemplateURLID keyword_id,
                                    const base::string16& term);

#if defined(OS_ANDROID)
  

  
  void InsertHistoryAndBookmark(scoped_refptr<InsertRequest> request,
                                const HistoryAndBookmarkRow& row);

  void QueryHistoryAndBookmarks(
      scoped_refptr<QueryRequest> request,
      const std::vector<HistoryAndBookmarkRow::ColumnID>& projections,
      const std::string& selection,
      const std::vector<base::string16>& selection_args,
      const std::string& sort_order);

  void UpdateHistoryAndBookmarks(
      scoped_refptr<UpdateRequest> request,
      const HistoryAndBookmarkRow& row,
      const std::string& selection,
      const std::vector<base::string16>& selection_args);

  void DeleteHistoryAndBookmarks(
      scoped_refptr<DeleteRequest> request,
      const std::string& selection,
      const std::vector<base::string16>& selection_args);

  void DeleteHistory(scoped_refptr<DeleteRequest> request,
                     const std::string& selection,
                     const std::vector<base::string16>& selection_args);

  
  
  void MoveStatement(scoped_refptr<MoveStatementRequest> request,
                     history::AndroidStatement* statement,
                     int current_pos,
                     int destination);

  
  void CloseStatement(AndroidStatement* statement);

  
  void InsertSearchTerm(scoped_refptr<InsertRequest> request,
                        const SearchRow& row);

  void UpdateSearchTerms(scoped_refptr<UpdateRequest> request,
                         const SearchRow& row,
                         const std::string& selection,
                         const std::vector<base::string16> selection_args);

  void DeleteSearchTerms(scoped_refptr<DeleteRequest> request,
                         const std::string& selection,
                         const std::vector<base::string16> selection_args);

  void QuerySearchTerms(scoped_refptr<QueryRequest> request,
                        const std::vector<SearchRow::ColumnID>& projections,
                        const std::string& selection,
                        const std::vector<base::string16>& selection_args,
                        const std::string& sort_order);

#endif  

  

  void ProcessDBTask(scoped_refptr<HistoryDBTaskRequest> request);

  virtual bool GetAllTypedURLs(URLRows* urls);

  virtual bool GetVisitsForURL(URLID id, VisitVector* visits);

  
  virtual bool GetMostRecentVisitsForURL(URLID id,
                                         int max_visits,
                                         VisitVector* visits);

  virtual bool UpdateURL(URLID id, const history::URLRow& url);

  
  virtual bool AddVisits(const GURL& url,
                         const std::vector<history::VisitInfo>& visits,
                         VisitSource visit_source);

  virtual bool RemoveVisits(const VisitVector& visits);

  
  
  
  bool GetVisitsSource(const VisitVector& visits, VisitSourceMap* sources);

  virtual bool GetURL(const GURL& url, history::URLRow* url_row);

  
  
  virtual TypedUrlSyncableService* GetTypedUrlSyncableService() const;

  

  virtual void DeleteURLs(const std::vector<GURL>& urls);

  virtual void DeleteURL(const GURL& url);

  
  void ExpireHistoryBetween(
      const std::set<GURL>& restrict_urls,
      base::Time begin_time,
      base::Time end_time);

  
  
  
  
  
  void ExpireHistoryForTimes(const std::set<base::Time>& times,
                             base::Time begin_time, base::Time end_time);

  
  
  
  void ExpireHistory(const std::vector<ExpireHistoryArgs>& expire_list);

  

  
  
  void URLsNoLongerBookmarked(const std::set<GURL>& urls);

  

  
  
  void DatabaseErrorCallback(int error, sql::Statement* stmt);

  
  
  
  
  
  void KillHistoryDatabase();

  

  
  
  
  void SetOnBackendDestroyTask(base::MessageLoop* message_loop,
                               const base::Closure& task);

  
  
  
  
  void AddPagesWithDetails(const URLRows& info, VisitSource visit_source);

#if defined(UNIT_TEST)
  HistoryDatabase* db() const { return db_.get(); }

  ExpireHistoryBackend* expire_backend() { return &expirer_; }
#endif

  
  
  virtual bool IsExpiredVisitTime(const base::Time& time);

  base::Time GetFirstRecordedTimeForTest() {
    return first_recorded_time_;
  }

 protected:
  virtual ~HistoryBackend();

 private:
  friend class base::RefCountedThreadSafe<HistoryBackend>;
  friend class CommitLaterTask;  
  friend class HistoryBackendTest;
  friend class HistoryBackendDBTest;  
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, DeleteAll);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, DeleteAllThenAddData);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, ImportedFaviconsTest);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, URLsNoLongerBookmarked);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, StripUsernamePasswordTest);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, DeleteThumbnailsDatabaseTest);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, AddPageVisitSource);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, AddPageVisitNotLastVisit);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, AddPageArgsSource);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, AddVisitsSource);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, GetMostRecentVisits);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, RemoveVisitsSource);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, RemoveVisitsTransitions);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, MigrationVisitSource);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest,
                           SetFaviconMappingsForPageAndRedirects);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest,
                           SetFaviconMappingsForPageDuplicates);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, SetFaviconsDeleteBitmaps);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, SetFaviconsReplaceBitmapData);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest,
                           SetFaviconsSameFaviconURLForTwoPages);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest,
                           UpdateFaviconMappingsAndFetchNoChange);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, MergeFaviconPageURLNotInDB);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, MergeFaviconPageURLInDB);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, MergeFaviconMaxFaviconsPerPage);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest,
                           MergeFaviconIconURLMappedToDifferentPageURL);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest,
                           MergeFaviconMaxFaviconBitmapsPerIconURL);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest,
                           UpdateFaviconMappingsAndFetchMultipleIconTypes);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, GetFaviconsFromDBEmpty);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest,
                           GetFaviconsFromDBNoFaviconBitmaps);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest,
                           GetFaviconsFromDBSelectClosestMatch);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, GetFaviconsFromDBSingleIconURL);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, GetFaviconsFromDBIconType);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, GetFaviconsFromDBExpired);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest,
                           UpdateFaviconMappingsAndFetchNoDB);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest,
                           CloneFaviconIsRestrictedToSameDomain);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, QueryFilteredURLs);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, UpdateVisitDuration);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, ExpireHistoryForTimes);
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, DeleteFTSIndexDatabases);

  friend class ::TestingProfile;

  
  base::FilePath GetArchivedFileName() const;
  base::FilePath GetThumbnailFileName() const;

  
  
  base::FilePath GetFaviconsFileName() const;

#if defined(OS_ANDROID)
  
  base::FilePath GetAndroidCacheFileName() const;

  
  
  
  

  void PopulateMostVisitedURLMap();
  
  
  
  
  void RecordTopPageVisitStats(const GURL& url);
#endif

  class URLQuerier;
  friend class URLQuerier;

  
  void InitImpl(const std::string& languages);

  
  void OnMemoryPressure(
      base::MemoryPressureListener::MemoryPressureLevel memory_pressure_level);

  
  
  void CloseAllDatabases();

  
  
  
  
  
  
  
  std::pair<URLID, VisitID> AddPageVisit(const GURL& url,
                                         base::Time time,
                                         VisitID referring_visit,
                                         content::PageTransition transition,
                                         VisitSource visit_source);

  
  
  
  void GetRedirectsFromSpecificVisit(
      VisitID cur_visit, history::RedirectList* redirects);

  
  
  void GetRedirectsToSpecificVisit(
      VisitID cur_visit, history::RedirectList* redirects);

  
  void UpdateVisitDuration(VisitID visit_id, const base::Time end_ts);

  

  
  
  
  
  
  void QueryHistoryBasic(URLDatabase* url_db, VisitDatabase* visit_db,
                         const QueryOptions& options, QueryResults* result);
  void QueryHistoryText(URLDatabase* url_db,
                        VisitDatabase* visit_db,
                        const base::string16& text_query,
                        const QueryOptions& options,
                        QueryResults* result);

  

  
  
  
  
  
  
  void Commit();

  
  
  
  void ScheduleCommit();

  
  
  void CancelScheduledCommit();

  

  
  
  SegmentID GetLastSegmentID(VisitID from_visit);

  
  
  SegmentID UpdateSegments(const GURL& url,
                           VisitID from_visit,
                           VisitID visit_id,
                           content::PageTransition transition_type,
                           const base::Time ts);

  

  
  
  
  
  
  
  
  
  void UpdateFaviconMappingsAndFetchImpl(
      const GURL* page_url,
      const std::vector<GURL>& icon_urls,
      int icon_types,
      int desired_size_in_dip,
      const std::vector<ui::ScaleFactor>& desired_scale_factors,
      std::vector<chrome::FaviconBitmapResult>* results);

  
  
  
  
  
  
  
  
  
  
  void SetFaviconBitmaps(
      chrome::FaviconID icon_id,
      const std::vector<chrome::FaviconBitmapData>& favicon_bitmap_data,
      bool* favicon_bitmaps_changed);

  
  
  
  
  
  
  bool ValidateSetFaviconsParams(
      const std::vector<chrome::FaviconBitmapData>& favicon_bitmap_data) const;

  
  bool IsFaviconBitmapDataEqual(
      FaviconBitmapID bitmap_id,
      const scoped_refptr<base::RefCountedMemory>& new_bitmap_data);

  
  
  
  
  
  
  
  
  
  
  
  bool GetFaviconsFromDB(
      const GURL& page_url,
      int icon_types,
      const int desired_size_in_dip,
      const std::vector<ui::ScaleFactor>& desired_scale_factors,
      std::vector<chrome::FaviconBitmapResult>* favicon_bitmap_results);

  
  
  
  
  
  
  
  
  
  bool GetFaviconBitmapResultsForBestMatch(
      const std::vector<chrome::FaviconID>& candidate_favicon_ids,
      int desired_size_in_dip,
      const std::vector<ui::ScaleFactor>& desired_scale_factors,
      std::vector<chrome::FaviconBitmapResult>* favicon_bitmap_results);

  
  
  
  
  bool SetFaviconMappingsForPageAndRedirects(
      const GURL& page_url,
      chrome::IconType icon_type,
      const std::vector<chrome::FaviconID>& icon_ids);

  
  
  bool SetFaviconMappingsForPage(
      const GURL& page_url,
      chrome::IconType icon_type,
      const std::vector<chrome::FaviconID>& icon_ids);

  
  
  void GetCachedRecentRedirects(const GURL& page_url,
                                history::RedirectList* redirect_list);

  
  
  void SendFaviconChangedNotificationForPageAndRedirects(
      const GURL& page_url);

  

  
  
  void ProcessDBTaskImpl();

  
  void ReleaseDBTasks();

  
  
  
  virtual void BroadcastNotifications(int type,
                                      HistoryDetails* details_deleted) OVERRIDE;

  virtual void NotifySyncURLsDeleted(bool all_history,
                                     bool archived,
                                     URLRows* rows) OVERRIDE;

  

  
  
  
  
  
  
  void DeleteAllHistory();

  
  
  
  bool ClearAllThumbnailHistory(const URLRows& kept_urls);

  
  
  
  
  
  bool ClearAllMainHistory(const URLRows& kept_urls);

  
  void DeleteFTSIndexDatabases();

  
  
  BookmarkService* GetBookmarkService();

  
  void NotifyVisitObservers(const VisitRow& visit);

  

  
  
  
  scoped_ptr<Delegate> delegate_;

  
  
  int id_;

  
  base::FilePath history_dir_;

  
  
  
  
  scoped_ptr<HistoryDatabase> db_;
  bool scheduled_kill_db_;  
  scoped_ptr<ThumbnailDatabase> thumbnail_db_;

  
  scoped_ptr<ArchivedDatabase> archived_db_;

  
  ExpireHistoryBackend expirer_;

  
  
  
  
  scoped_refptr<CommitLaterTask> scheduled_commit_;

  
  
  
  
  
  
  
  
  
  typedef base::MRUCache<GURL, history::RedirectList> RedirectCache;
  RedirectCache recent_redirects_;

  
  base::Time first_recorded_time_;

  
  base::MessageLoop* backend_destroy_message_loop_;
  base::Closure backend_destroy_task_;

  
  VisitTracker tracker_;

  
  
  bool segment_queried_;

  
  
  std::list<HistoryDBTaskRequest*> db_task_requests_;

  
  
  
  
  
  BookmarkService* bookmark_service_;

#if defined(OS_ANDROID)
  
  scoped_ptr<AndroidProviderBackend> android_provider_backend_;

  
  
  
  
  std::map<GURL, int> most_visited_urls_map_;
#endif

  
  
  scoped_ptr<TypedUrlSyncableService> typed_url_syncable_service_;

  
  scoped_ptr<base::MemoryPressureListener> memory_pressure_listener_;

  DISALLOW_COPY_AND_ASSIGN(HistoryBackend);
};

}  

#endif  
