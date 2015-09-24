// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_HISTORY_SERVICE_H_
#define CHROME_BROWSER_HISTORY_HISTORY_SERVICE_H_

#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "base/threading/thread_checker.h"
#include "base/time/time.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/favicon/favicon_service.h"
#include "chrome/browser/history/delete_directive_handler.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/typed_url_syncable_service.h"
#include "chrome/browser/search_engines/template_url_id.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "chrome/common/ref_counted_util.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "components/visitedlink/browser/visitedlink_delegate.h"
#include "content/public/browser/download_manager_delegate.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/common/page_transition_types.h"
#include "sql/init_status.h"
#include "sync/api/syncable_service.h"
#include "ui/base/layout.h"

#if defined(OS_ANDROID)
#include "chrome/browser/history/android/android_history_provider_service.h"
#endif

class BookmarkService;
class GURL;
class HistoryURLProvider;
class PageUsageData;
class PageUsageRequest;
class Profile;
struct HistoryURLProviderParams;

namespace base {
class FilePath;
class Thread;
}

namespace visitedlink {
class VisitedLinkMaster;
}

namespace history {

class HistoryBackend;
class HistoryDatabase;
class HistoryDBTask;
class HistoryQueryTest;
class InMemoryHistoryBackend;
class InMemoryURLIndex;
class InMemoryURLIndexTest;
class URLDatabase;
class VisitDatabaseObserver;
class VisitFilter;
struct DownloadRow;
struct HistoryAddPageArgs;
struct HistoryDetails;

}  

class HistoryService : public CancelableRequestProvider,
                       public content::NotificationObserver,
                       public syncer::SyncableService,
                       public BrowserContextKeyedService,
                       public visitedlink::VisitedLinkDelegate {
 public:
  
  typedef std::vector<PageUsageData*> PageUsageDataList;

  
  explicit HistoryService(Profile* profile);
  
  HistoryService();

  virtual ~HistoryService();

  
  
  
  
  bool Init(const base::FilePath& history_dir, BookmarkService* bookmark_service) {
    return Init(history_dir, bookmark_service, false);
  }

  
  
  
  virtual bool BackendLoaded();

  
  bool backend_loaded() const { return backend_loaded_; }

  
  
  
  void UnloadBackend();

  
  
  
  
  
  
  
  
  
  void Cleanup();

  
  
  
  
  
  
  void NotifyRenderProcessHostDestruction(const void* host);

  
  
  
  
  
  
  history::URLDatabase* InMemoryDatabase();

  
  
  

  
  bool GetTypedCountForURL(const GURL& url, int* typed_count);

  
  bool GetLastVisitTimeForURL(const GURL& url, base::Time* last_visit);

  
  bool GetVisitCountForURL(const GURL& url, int* visit_count);

  
  
  
  history::TypedUrlSyncableService* GetTypedUrlSyncableService() const;

  
  history::InMemoryURLIndex* InMemoryIndex() const {
    return in_memory_url_index_.get();
  }

  
  virtual void Shutdown() OVERRIDE;

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void AddPage(const GURL& url,
               base::Time time,
               const void* id_scope,
               int32 page_id,
               const GURL& referrer,
               const history::RedirectList& redirects,
               content::PageTransition transition,
               history::VisitSource visit_source,
               bool did_replace_entry);

  
  void AddPage(const GURL& url,
               base::Time time,
               history::VisitSource visit_source);

  
  void AddPage(const history::HistoryAddPageArgs& add_page_args);

  
  
  
  void AddPageNoVisitForBookmark(const GURL& url, const base::string16& title);

  
  
  void SetPageTitle(const GURL& url, const base::string16& title);

  
  
  
  
  
  
  void UpdateWithPageEndTime(const void* host,
                             int32 page_id,
                             const GURL& url,
                             base::Time end_ts);

  

  
  
  
  
  
  
  
  typedef base::Callback<void(
      Handle,
      bool,  
      const history::URLRow*,
      history::VisitVector*)> QueryURLCallback;

  
  
  
  
  Handle QueryURL(const GURL& url,
                  bool want_visits,
                  CancelableRequestConsumerBase* consumer,
                  const QueryURLCallback& callback);

  
  
  
  typedef base::Callback<void(Handle, history::QueryResults*)>
      QueryHistoryCallback;

  
  
  
  Handle QueryHistory(const base::string16& text_query,
                      const history::QueryOptions& options,
                      CancelableRequestConsumerBase* consumer,
                      const QueryHistoryCallback& callback);

  
  
  
  
  
  
  
  
  
  typedef base::Callback<void(Handle,
                              GURL,  
                              bool,  
                              history::RedirectList*)> QueryRedirectsCallback;

  
  
  
  Handle QueryRedirectsFrom(const GURL& from_url,
                            CancelableRequestConsumerBase* consumer,
                            const QueryRedirectsCallback& callback);

  
  
  Handle QueryRedirectsTo(const GURL& to_url,
                          CancelableRequestConsumerBase* consumer,
                          const QueryRedirectsCallback& callback);

  typedef base::Callback<
      void(Handle,
           bool,        
           int,         
           base::Time)> 
                        
      GetVisibleVisitCountToHostCallback;

  
  
  
  Handle GetVisibleVisitCountToHost(
      const GURL& url,
      CancelableRequestConsumerBase* consumer,
      const GetVisibleVisitCountToHostCallback& callback);

  
  
  
  
  
  typedef base::Callback<
      void(Handle,
           bool,  
           std::vector<GURL>*,  
           history::RedirectMap*)>  
      QueryTopURLsAndRedirectsCallback;

  
  
  
  Handle QueryTopURLsAndRedirects(
      int result_count,
      CancelableRequestConsumerBase* consumer,
      const QueryTopURLsAndRedirectsCallback& callback);

  typedef base::Callback<void(Handle, history::MostVisitedURLList)>
      QueryMostVisitedURLsCallback;

  typedef base::Callback<void(Handle, const history::FilteredURLList&)>
      QueryFilteredURLsCallback;

  
  
  
  Handle QueryMostVisitedURLs(int result_count, int days_back,
                              CancelableRequestConsumerBase* consumer,
                              const QueryMostVisitedURLsCallback& callback);

  
  
  
  
  
  
  Handle QueryFilteredURLs(
      int result_count,
      const history::VisitFilter& filter,
      bool extended_info,
      CancelableRequestConsumerBase* consumer,
      const QueryFilteredURLsCallback& callback);

  

  
  void DeleteURL(const GURL& url);

  
  
  void DeleteURLsForTest(const std::vector<GURL>& urls);

  
  
  
  
  
  
  
  
  void ExpireHistoryBetween(const std::set<GURL>& restrict_urls,
                            base::Time begin_time,
                            base::Time end_time,
                            const base::Closure& callback,
                            CancelableTaskTracker* tracker);

  
  
  
  
  void ExpireHistory(const std::vector<history::ExpireHistoryArgs>& expire_list,
                     const base::Closure& callback,
                     CancelableTaskTracker* tracker);

  
  
  
  void ExpireLocalAndRemoteHistoryBetween(
      const std::set<GURL>& restrict_urls,
      base::Time begin_time,
      base::Time end_time,
      const base::Closure& callback,
      CancelableTaskTracker* tracker);

  
  
  
  syncer::SyncError ProcessLocalDeleteDirective(
      const sync_pb::HistoryDeleteDirectiveSpecifics& delete_directive);

  

  
  
  typedef base::Callback<void(bool)> DownloadCreateCallback;

  
  
  
  
  void CreateDownload(
      const history::DownloadRow& info,
      const DownloadCreateCallback& callback);

  
  
  void GetNextDownloadId(const content::DownloadIdCallback& callback);

  
  
  typedef base::Callback<void(
      scoped_ptr<std::vector<history::DownloadRow> >)>
          DownloadQueryCallback;

  
  
  
  
  void QueryDownloads(const DownloadQueryCallback& callback);

  
  
  
  void UpdateDownload(const history::DownloadRow& data);

  
  
  void RemoveDownloads(const std::set<uint32>& ids);

  

  typedef base::Callback<void(Handle, std::vector<PageUsageData*>*)>
      SegmentQueryCallback;

  
  
  
  
  
  
  
  
  
  
  
  
  
  Handle QuerySegmentUsageSince(CancelableRequestConsumerBase* consumer,
                                const base::Time from_time,
                                int max_result_count,
                                const SegmentQueryCallback& callback);

  
  void IncreaseSegmentDuration(const GURL& url,
                               base::Time time,
                               base::TimeDelta delta);

  
  Handle QuerySegmentDurationSince(CancelableRequestConsumerBase* consumer,
                                   base::Time from_time,
                                   int max_result_count,
                                   const SegmentQueryCallback& callback);

  

  
  
  void SetKeywordSearchTermsForURL(const GURL& url,
                                   TemplateURLID keyword_id,
                                   const base::string16& term);

  
  void DeleteAllSearchTermsForKeyword(TemplateURLID keyword_id);

  typedef base::Callback<
      void(Handle, std::vector<history::KeywordSearchTermVisit>*)>
          GetMostRecentKeywordSearchTermsCallback;

  
  
  
  
  Handle GetMostRecentKeywordSearchTerms(
      TemplateURLID keyword_id,
      const base::string16& prefix,
      int max_count,
      CancelableRequestConsumerBase* consumer,
      const GetMostRecentKeywordSearchTermsCallback& callback);

  
  void DeleteKeywordSearchTermForURL(const GURL& url);

  
  
  void DeleteMatchingURLsForKeyword(TemplateURLID keyword_id,
                                    const base::string16& term);

  

  
  void URLsNoLongerBookmarked(const std::set<GURL>& urls);

  

  
  
  virtual void ScheduleDBTask(history::HistoryDBTask* task,
                              CancelableRequestConsumerBase* consumer);

  
  void AddVisitDatabaseObserver(history::VisitDatabaseObserver* observer);
  void RemoveVisitDatabaseObserver(history::VisitDatabaseObserver* observer);

  void NotifyVisitDBObserversOnAddVisit(const history::BriefVisitInfo& info);

  

  
  void FlushForTest(const base::Closure& flushed);

  
  
  
  
  
  
  
  
  
  void SetOnBackendDestroyTask(const base::Closure& task);

  
  
  
  
  
  
  
  
  
  
  
  void AddPageWithDetails(const GURL& url,
                          const base::string16& title,
                          int visit_count,
                          int typed_count,
                          base::Time last_visit,
                          bool hidden,
                          history::VisitSource visit_source);

  
  void AddPagesWithDetails(const history::URLRows& info,
                           history::VisitSource visit_source);

  
  
  static bool CanAddURL(const GURL& url);

  base::WeakPtr<HistoryService> AsWeakPtr();

  
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

 protected:
  
  
  enum SchedulePriority {
    PRIORITY_UI,      
    PRIORITY_NORMAL,  
    PRIORITY_LOW,     
  };

 private:
  class BackendDelegate;
#if defined(OS_ANDROID)
  friend class AndroidHistoryProviderService;
#endif
  friend class base::RefCountedThreadSafe<HistoryService>;
  friend class BackendDelegate;
  friend class FaviconService;
  friend class history::HistoryBackend;
  friend class history::HistoryQueryTest;
  friend class HistoryOperation;
  friend class HistoryQuickProviderTest;
  friend class HistoryURLProvider;
  friend class HistoryURLProviderTest;
  friend class history::InMemoryURLIndexTest;
  template<typename Info, typename Callback> friend class DownloadRequest;
  friend class PageUsageRequest;
  friend class RedirectRequest;
  friend class TestingProfile;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void RebuildTable(
      const scoped_refptr<URLEnumerator>& enumerator) OVERRIDE;

  
  
  bool Init(const base::FilePath& history_dir,
            BookmarkService* bookmark_service,
            bool no_db);

  
  
  
  void ScheduleAutocomplete(HistoryURLProvider* provider,
                            HistoryURLProviderParams* params);

  
  
  
  
  
  void BroadcastNotificationsHelper(int type,
                                    history::HistoryDetails* details);

  
  void LoadBackendIfNecessary();

  
  
  void OnDBLoaded(int backend_id);

  
  
  
  bool GetRowForURL(const GURL& url, history::URLRow* url_row);

  

  
  
  

  
  
  
  
  
  
  
  
  
  
  CancelableTaskTracker::TaskId GetFavicons(
      const std::vector<GURL>& icon_urls,
      int icon_types,
      int desired_size_in_dip,
      const std::vector<ui::ScaleFactor>& desired_scale_factors,
      const FaviconService::FaviconResultsCallback& callback,
      CancelableTaskTracker* tracker);

  
  
  
  
  
  
  
  
  
  CancelableTaskTracker::TaskId GetFaviconsForURL(
      const GURL& page_url,
      int icon_types,
      int desired_size_in_dip,
      const std::vector<ui::ScaleFactor>& desired_scale_factors,
      const FaviconService::FaviconResultsCallback& callback,
      CancelableTaskTracker* tracker);

  
  
  
  
  
  
  
  
  
  
  
  CancelableTaskTracker::TaskId GetLargestFaviconForURL(
      const GURL& page_url,
      const std::vector<int>& icon_types,
      int minimum_size_in_pixels,
      const FaviconService::FaviconRawCallback& callback,
      CancelableTaskTracker* tracker);

  
  
  
  
  CancelableTaskTracker::TaskId GetFaviconForID(
      chrome::FaviconID favicon_id,
      int desired_size_in_dip,
      ui::ScaleFactor desired_scale_factor,
      const FaviconService::FaviconResultsCallback& callback,
      CancelableTaskTracker* tracker);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  CancelableTaskTracker::TaskId UpdateFaviconMappingsAndFetch(
      const GURL& page_url,
      const std::vector<GURL>& icon_urls,
      int icon_types,
      int desired_size_in_dip,
      const std::vector<ui::ScaleFactor>& desired_scale_factors,
      const FaviconService::FaviconResultsCallback& callback,
      CancelableTaskTracker* tracker);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
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

  
  
  // discarded. Existing favicons will not be overwritten.
  void SetImportedFavicons(
      const std::vector<ImportedFaviconUsage>& favicon_usage);

  
  
  void SetInMemoryBackend(
      int backend_id,
      scoped_ptr<history::InMemoryHistoryBackend> mem_backend);

  
  void NotifyProfileError(int backend_id, sql::InitStatus init_status);

  
  
  void ScheduleTask(SchedulePriority priority, const base::Closure& task);

  
  
  
  
  

  template<typename BackendFunc, class RequestType>
  Handle Schedule(SchedulePriority priority,
                  BackendFunc func,  
                  CancelableRequestConsumerBase* consumer,
                  RequestType* request) {
    DCHECK(thread_) << "History service being called after cleanup";
    DCHECK(thread_checker_.CalledOnValidThread());
    LoadBackendIfNecessary();
    if (consumer)
      AddRequest(request, consumer);
    ScheduleTask(priority,
                 base::Bind(func, history_backend_.get(),
                            scoped_refptr<RequestType>(request)));
    return request->handle();
  }

  template<typename BackendFunc, class RequestType, typename ArgA>
  Handle Schedule(SchedulePriority priority,
                  BackendFunc func,  
                  CancelableRequestConsumerBase* consumer,
                  RequestType* request,
                  const ArgA& a) {
    DCHECK(thread_) << "History service being called after cleanup";
    DCHECK(thread_checker_.CalledOnValidThread());
    LoadBackendIfNecessary();
    if (consumer)
      AddRequest(request, consumer);
    ScheduleTask(priority,
                 base::Bind(func, history_backend_.get(),
                            scoped_refptr<RequestType>(request), a));
    return request->handle();
  }

  template<typename BackendFunc,
           class RequestType,  
           typename ArgA,
           typename ArgB>
  Handle Schedule(SchedulePriority priority,
                  BackendFunc func,  
                  CancelableRequestConsumerBase* consumer,
                  RequestType* request,
                  const ArgA& a,
                  const ArgB& b) {
    DCHECK(thread_) << "History service being called after cleanup";
    DCHECK(thread_checker_.CalledOnValidThread());
    LoadBackendIfNecessary();
    if (consumer)
      AddRequest(request, consumer);
    ScheduleTask(priority,
                 base::Bind(func, history_backend_.get(),
                            scoped_refptr<RequestType>(request), a, b));
    return request->handle();
  }

  template<typename BackendFunc,
           class RequestType,  
           typename ArgA,
           typename ArgB,
           typename ArgC>
  Handle Schedule(SchedulePriority priority,
                  BackendFunc func,  
                  CancelableRequestConsumerBase* consumer,
                  RequestType* request,
                  const ArgA& a,
                  const ArgB& b,
                  const ArgC& c) {
    DCHECK(thread_) << "History service being called after cleanup";
    DCHECK(thread_checker_.CalledOnValidThread());
    LoadBackendIfNecessary();
    if (consumer)
      AddRequest(request, consumer);
    ScheduleTask(priority,
                 base::Bind(func, history_backend_.get(),
                            scoped_refptr<RequestType>(request), a, b, c));
    return request->handle();
  }

  template<typename BackendFunc,
           class RequestType,  
           typename ArgA,
           typename ArgB,
           typename ArgC,
           typename ArgD>
  Handle Schedule(SchedulePriority priority,
                  BackendFunc func,  
                  CancelableRequestConsumerBase* consumer,
                  RequestType* request,
                  const ArgA& a,
                  const ArgB& b,
                  const ArgC& c,
                  const ArgD& d) {
    DCHECK(thread_) << "History service being called after cleanup";
    DCHECK(thread_checker_.CalledOnValidThread());
    LoadBackendIfNecessary();
    if (consumer)
      AddRequest(request, consumer);
    ScheduleTask(priority,
                 base::Bind(func, history_backend_.get(),
                            scoped_refptr<RequestType>(request), a, b, c, d));
    return request->handle();
  }

  
  
  
  

  template<typename BackendFunc>
  void ScheduleAndForget(SchedulePriority priority,
                         BackendFunc func) {  
    DCHECK(thread_) << "History service being called after cleanup";
    DCHECK(thread_checker_.CalledOnValidThread());
    LoadBackendIfNecessary();
    ScheduleTask(priority, base::Bind(func, history_backend_.get()));
  }

  template<typename BackendFunc, typename ArgA>
  void ScheduleAndForget(SchedulePriority priority,
                         BackendFunc func,  
                         const ArgA& a) {
    DCHECK(thread_) << "History service being called after cleanup";
    DCHECK(thread_checker_.CalledOnValidThread());
    LoadBackendIfNecessary();
    ScheduleTask(priority, base::Bind(func, history_backend_.get(), a));
  }

  template<typename BackendFunc, typename ArgA, typename ArgB>
  void ScheduleAndForget(SchedulePriority priority,
                         BackendFunc func,  
                         const ArgA& a,
                         const ArgB& b) {
    DCHECK(thread_) << "History service being called after cleanup";
    DCHECK(thread_checker_.CalledOnValidThread());
    LoadBackendIfNecessary();
    ScheduleTask(priority, base::Bind(func, history_backend_.get(), a, b));
  }

  template<typename BackendFunc, typename ArgA, typename ArgB, typename ArgC>
  void ScheduleAndForget(SchedulePriority priority,
                         BackendFunc func,  
                         const ArgA& a,
                         const ArgB& b,
                         const ArgC& c) {
    DCHECK(thread_) << "History service being called after cleanup";
    DCHECK(thread_checker_.CalledOnValidThread());
    LoadBackendIfNecessary();
    ScheduleTask(priority, base::Bind(func, history_backend_.get(), a, b, c));
  }

  template<typename BackendFunc,
           typename ArgA,
           typename ArgB,
           typename ArgC,
           typename ArgD>
  void ScheduleAndForget(SchedulePriority priority,
                         BackendFunc func,  
                         const ArgA& a,
                         const ArgB& b,
                         const ArgC& c,
                         const ArgD& d) {
    DCHECK(thread_) << "History service being called after cleanup";
    DCHECK(thread_checker_.CalledOnValidThread());
    LoadBackendIfNecessary();
    ScheduleTask(priority, base::Bind(func, history_backend_.get(),
                                      a, b, c, d));
  }

  template<typename BackendFunc,
           typename ArgA,
           typename ArgB,
           typename ArgC,
           typename ArgD,
           typename ArgE>
  void ScheduleAndForget(SchedulePriority priority,
                         BackendFunc func,  
                         const ArgA& a,
                         const ArgB& b,
                         const ArgC& c,
                         const ArgD& d,
                         const ArgE& e) {
    DCHECK(thread_) << "History service being called after cleanup";
    DCHECK(thread_checker_.CalledOnValidThread());
    LoadBackendIfNecessary();
    ScheduleTask(priority, base::Bind(func, history_backend_.get(),
                                      a, b, c, d, e));
  }

  
  base::WeakPtrFactory<HistoryService> weak_ptr_factory_;

  base::ThreadChecker thread_checker_;

  content::NotificationRegistrar registrar_;

  
  
  CancelableRequestConsumer internal_consumer_;

  
  
  base::Thread* thread_;

  
  
  
  
  
  
  scoped_refptr<history::HistoryBackend> history_backend_;

  
  
  
  
  scoped_ptr<history::InMemoryHistoryBackend> in_memory_backend_;

  
  Profile* profile_;

  
  
  scoped_ptr<visitedlink::VisitedLinkMaster> visitedlink_master_;

  
  
  bool backend_loaded_;

  
  
  int current_backend_id_;

  
  base::FilePath history_dir_;
  BookmarkService* bookmark_service_;
  bool no_db_;

  
  
  
  scoped_ptr<history::InMemoryURLIndex> in_memory_url_index_;

  ObserverList<history::VisitDatabaseObserver> visit_database_observers_;

  history::DeleteDirectiveHandler delete_directive_handler_;

  DISALLOW_COPY_AND_ASSIGN(HistoryService);
};

#endif  
