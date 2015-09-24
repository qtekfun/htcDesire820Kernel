// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_HISTORY_H_
#define CHROME_BROWSER_HISTORY_HISTORY_H_
#pragma once

#include <set>
#include <vector>

#include "app/sql/init_status.h"
#include "base/basictypes.h"
#include "base/callback.h"
#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "base/task.h"
#include "chrome/browser/favicon_service.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/search_engines/template_url_id.h"
#include "chrome/common/ref_counted_util.h"
#include "content/browser/cancelable_request.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/page_transition_types.h"

class BookmarkService;
struct DownloadCreateInfo;
class FilePath;
class GURL;
class HistoryURLProvider;
struct HistoryURLProviderParams;
class InMemoryURLDatabase;
class MainPagesRequest;
class PageUsageData;
class PageUsageRequest;
class Profile;
class SkBitmap;
struct ThumbnailScore;

namespace base {
class Thread;
class Time;
}

namespace browser_sync {
class HistoryModelWorker;
class TypedUrlDataTypeController;
}

namespace history {
class InMemoryHistoryBackend;
class InMemoryURLIndex;
class HistoryAddPageArgs;
class HistoryBackend;
class HistoryDatabase;
struct HistoryDetails;
class HistoryQueryTest;
class URLDatabase;
}  


class HistoryDBTask : public base::RefCountedThreadSafe<HistoryDBTask> {
 public:
  
  
  
  
  
  virtual bool RunOnDBThread(history::HistoryBackend* backend,
                             history::HistoryDatabase* db) = 0;

  
  
  
  virtual void DoneRunOnMainThread() = 0;

 protected:
  friend class base::RefCountedThreadSafe<HistoryDBTask>;

  virtual ~HistoryDBTask() {}
};

class HistoryService : public CancelableRequestProvider,
                       public NotificationObserver,
                       public base::RefCountedThreadSafe<HistoryService> {
 public:
  
  typedef std::vector<PageUsageData*> PageUsageDataList;

  
  
  static const history::StarID kBookmarkBarID;

  
  explicit HistoryService(Profile* profile);
  
  HistoryService();

  
  
  
  
  bool Init(const FilePath& history_dir, BookmarkService* bookmark_service) {
    return Init(history_dir, bookmark_service, false);
  }

  
  
  bool BackendLoaded();

  
  bool backend_loaded() const { return backend_loaded_; }

  
  
  
  void UnloadBackend();

  
  
  
  
  
  
  
  
  
  void Cleanup();

  
  
  
  
  
  
  void NotifyRenderProcessHostDestruction(const void* host);

  
  
  
  
  
  
  history::URLDatabase* InMemoryDatabase();

  
  history::InMemoryURLIndex* InMemoryIndex();

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void AddPage(const GURL& url,
               const void* id_scope,
               int32 page_id,
               const GURL& referrer,
               PageTransition::Type transition,
               const history::RedirectList& redirects,
               history::VisitSource visit_source,
               bool did_replace_entry);

  
  
  void AddPage(const GURL& url,
               base::Time time,
               const void* id_scope,
               int32 page_id,
               const GURL& referrer,
               PageTransition::Type transition,
               const history::RedirectList& redirects,
               history::VisitSource visit_source,
               bool did_replace_entry);

  
  void AddPage(const GURL& url, history::VisitSource visit_source) {
    AddPage(url, NULL, 0, GURL(), PageTransition::LINK,
            history::RedirectList(), visit_source, false);
  }

  
  void AddPage(const history::HistoryAddPageArgs& add_page_args);

  
  
  
  void AddPageNoVisitForBookmark(const GURL& url);

  
  
  
  
  void SetPageTitle(const GURL& url, const string16& title);

  

  
  
  
  void SetPageContents(const GURL& url, const string16& contents);

  

  
  
  
  class URLEnumerator {
   public:
    virtual ~URLEnumerator() {}

    
    
    virtual void OnURL(const GURL& url) = 0;

    
    
    
    virtual void OnComplete(bool success) = 0;
  };

  
  
  
  
  
  void IterateURLs(URLEnumerator* iterator);

  
  
  
  
  
  
  
  typedef Callback4<Handle,
                    bool,  
                    const history::URLRow*,
                    history::VisitVector*>::Type
      QueryURLCallback;

  
  
  
  
  Handle QueryURL(const GURL& url,
                  bool want_visits,
                  CancelableRequestConsumerBase* consumer,
                  QueryURLCallback* callback);

  
  
  
  typedef Callback2<Handle, history::QueryResults*>::Type
      QueryHistoryCallback;

  
  
  
  
  
  
  
  Handle QueryHistory(const string16& text_query,
                      const history::QueryOptions& options,
                      CancelableRequestConsumerBase* consumer,
                      QueryHistoryCallback* callback);

  
  
  
  
  
  
  
  
  
  typedef Callback4<Handle,
                    GURL,  
                    bool,  
                    history::RedirectList*>::Type
      QueryRedirectsCallback;

  
  
  
  Handle QueryRedirectsFrom(const GURL& from_url,
                            CancelableRequestConsumerBase* consumer,
                            QueryRedirectsCallback* callback);

  
  
  Handle QueryRedirectsTo(const GURL& to_url,
                          CancelableRequestConsumerBase* consumer,
                          QueryRedirectsCallback* callback);

  typedef Callback4<Handle,
                    bool,        
                    int,         
                    base::Time>::Type  
                                       
      GetVisitCountToHostCallback;

  
  
  Handle GetVisitCountToHost(const GURL& url,
                             CancelableRequestConsumerBase* consumer,
                             GetVisitCountToHostCallback* callback);

  
  
  
  
  
  typedef Callback4<Handle,
                    bool,  
                    std::vector<GURL>*,  
                    history::RedirectMap*>::Type  
      QueryTopURLsAndRedirectsCallback;

  
  
  
  Handle QueryTopURLsAndRedirects(int result_count,
                                  CancelableRequestConsumerBase* consumer,
                                  QueryTopURLsAndRedirectsCallback* callback);

  typedef Callback2<Handle, history::MostVisitedURLList>::Type
                    QueryMostVisitedURLsCallback;

  
  
  
  Handle QueryMostVisitedURLs(int result_count, int days_back,
                              CancelableRequestConsumerBase* consumer,
                              QueryMostVisitedURLsCallback* callback);

  

  
  
  
  
  
  
  typedef Callback2<Handle, scoped_refptr<RefCountedBytes> >::Type
      ThumbnailDataCallback;

  
  
  void SetPageThumbnail(const GURL& url,
                        const SkBitmap& thumbnail,
                        const ThumbnailScore& score);

  
  Handle GetPageThumbnail(const GURL& page_url,
                          CancelableRequestConsumerBase* consumer,
                          ThumbnailDataCallback* callback);

  

  
  void DeleteURL(const GURL& url);

  
  
  typedef Callback0::Type ExpireHistoryCallback;

  
  
  
  
  
  
  
  
  void ExpireHistoryBetween(const std::set<GURL>& restrict_urls,
                            base::Time begin_time, base::Time end_time,
                            CancelableRequestConsumerBase* consumer,
                            ExpireHistoryCallback* callback);

  

  
  
  typedef Callback2<DownloadCreateInfo, int64>::Type
      DownloadCreateCallback;

  
  
  
  Handle CreateDownload(const DownloadCreateInfo& info,
                        CancelableRequestConsumerBase* consumer,
                        DownloadCreateCallback* callback);

  
  
  typedef Callback1<std::vector<DownloadCreateInfo>*>::Type
      DownloadQueryCallback;

  
  
  
  
  Handle QueryDownloads(CancelableRequestConsumerBase* consumer,
                        DownloadQueryCallback* callback);

  
  
  void CleanUpInProgressEntries();

  
  
  
  void UpdateDownload(int64 received_bytes, int32 state, int64 db_handle);

  
  
  void UpdateDownloadPath(const FilePath& path, int64 db_handle);

  
  
  void RemoveDownload(int64 db_handle);

  
  
  
  
  
  void RemoveDownloadsBetween(base::Time remove_begin, base::Time remove_end);

  

  typedef Callback2<Handle, std::vector<PageUsageData*>*>::Type
      SegmentQueryCallback;

  
  
  
  
  
  
  
  
  
  
  
  
  
  Handle QuerySegmentUsageSince(CancelableRequestConsumerBase* consumer,
                                const base::Time from_time,
                                int max_result_count,
                                SegmentQueryCallback* callback);

  
  void SetSegmentPresentationIndex(int64 segment_id, int index);

  

  
  
  void SetKeywordSearchTermsForURL(const GURL& url,
                                   TemplateURLID keyword_id,
                                   const string16& term);

  
  void DeleteAllSearchTermsForKeyword(TemplateURLID keyword_id);

  typedef Callback2<Handle, std::vector<history::KeywordSearchTermVisit>*>::Type
      GetMostRecentKeywordSearchTermsCallback;

  
  
  
  
  Handle GetMostRecentKeywordSearchTerms(
      TemplateURLID keyword_id,
      const string16& prefix,
      int max_count,
      CancelableRequestConsumerBase* consumer,
      GetMostRecentKeywordSearchTermsCallback* callback);

  

  
  void URLsNoLongerBookmarked(const std::set<GURL>& urls);

  

  typedef Callback0::Type HistoryDBTaskCallback;

  
  
  virtual Handle ScheduleDBTask(HistoryDBTask* task,
                                CancelableRequestConsumerBase* consumer);

  
  
  bool needs_top_sites_migration() const { return needs_top_sites_migration_; }

  

  
  
  
  
  
  
  
  
  
  void SetOnBackendDestroyTask(Task* task);

  
  
  
  
  
  
  
  void AddPageWithDetails(const GURL& url,
                          const string16& title,
                          int visit_count,
                          int typed_count,
                          base::Time last_visit,
                          bool hidden,
                          history::VisitSource visit_source);

  
  void AddPagesWithDetails(const std::vector<history::URLRow>& info,
                           history::VisitSource visit_source);

  
  
  void StartTopSitesMigration();

  
  
  void OnTopSitesReady();

  
  
  static bool CanAddURL(const GURL& url);

 protected:
  ~HistoryService();

  
  
  enum SchedulePriority {
    PRIORITY_UI,      
    PRIORITY_NORMAL,  
    PRIORITY_LOW,     
  };

 private:
  class BackendDelegate;
  friend class base::RefCountedThreadSafe<HistoryService>;
  friend class BackendDelegate;
  friend class FaviconService;
  friend class history::HistoryBackend;
  friend class history::HistoryQueryTest;
  friend class HistoryOperation;
  friend class HistoryURLProvider;
  friend class HistoryURLProviderTest;
  template<typename Info, typename Callback> friend class DownloadRequest;
  friend class PageUsageRequest;
  friend class RedirectRequest;
  friend class TestingProfile;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  bool Init(const FilePath& history_dir,
            BookmarkService* bookmark_service,
            bool no_db);

  
  
  
  void ScheduleAutocomplete(HistoryURLProvider* provider,
                            HistoryURLProviderParams* params);

  
  
  
  
  
  
  
  void BroadcastNotifications(NotificationType type,
                              history::HistoryDetails* details_deleted);

  
  void LoadBackendIfNecessary();

  
  
  void OnDBLoaded();

  

  
  
  

  
  void GetFavicon(FaviconService::GetFaviconRequest* request,
                  const GURL& icon_url,
                  history::IconType icon_type);

  
  
  void UpdateFaviconMappingAndFetch(FaviconService::GetFaviconRequest* request,
                                    const GURL& page_url,
                                    const GURL& icon_url,
                                    history::IconType icon_type);

  
  void GetFaviconForURL(FaviconService::GetFaviconRequest* request,
                        const GURL& page_url,
                        int icon_types);

  
  
  void SetFaviconOutOfDateForPage(const GURL& page_url);

  
  
  // discarded. Existing favicons will not be overwritten.
  void SetImportedFavicons(
      const std::vector<history::ImportedFaviconUsage>& favicon_usage);

  
  
  void SetFavicon(const GURL& page_url,
                  const GURL& icon_url,
                  const std::vector<unsigned char>& image_data,
                  history::IconType icon_type);


  
  
  void SetInMemoryBackend(history::InMemoryHistoryBackend* mem_backend);

  
  void NotifyProfileError(sql::InitStatus init_status);

  
  
  void ScheduleTask(SchedulePriority priority, Task* task);

  
  
  
  
  

  template<typename BackendFunc, class RequestType>
  Handle Schedule(SchedulePriority priority,
                  BackendFunc func,  
                  CancelableRequestConsumerBase* consumer,
                  RequestType* request) {
    DCHECK(thread_) << "History service being called after cleanup";
    LoadBackendIfNecessary();
    if (consumer)
      AddRequest(request, consumer);
    ScheduleTask(priority,
                 NewRunnableMethod(history_backend_.get(), func,
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
    LoadBackendIfNecessary();
    if (consumer)
      AddRequest(request, consumer);
    ScheduleTask(priority,
                 NewRunnableMethod(history_backend_.get(), func,
                                   scoped_refptr<RequestType>(request),
                                   a));
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
    LoadBackendIfNecessary();
    if (consumer)
      AddRequest(request, consumer);
    ScheduleTask(priority,
                 NewRunnableMethod(history_backend_.get(), func,
                                   scoped_refptr<RequestType>(request),
                                   a, b));
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
    LoadBackendIfNecessary();
    if (consumer)
      AddRequest(request, consumer);
    ScheduleTask(priority,
                 NewRunnableMethod(history_backend_.get(), func,
                                   scoped_refptr<RequestType>(request),
                                   a, b, c));
    return request->handle();
  }

  
  
  
  

  template<typename BackendFunc>
  void ScheduleAndForget(SchedulePriority priority,
                         BackendFunc func) {  
    DCHECK(thread_) << "History service being called after cleanup";
    LoadBackendIfNecessary();
    ScheduleTask(priority, NewRunnableMethod(history_backend_.get(), func));
  }

  template<typename BackendFunc, typename ArgA>
  void ScheduleAndForget(SchedulePriority priority,
                         BackendFunc func,  
                         const ArgA& a) {
    DCHECK(thread_) << "History service being called after cleanup";
    LoadBackendIfNecessary();
    ScheduleTask(priority, NewRunnableMethod(history_backend_.get(), func, a));
  }

  template<typename BackendFunc, typename ArgA, typename ArgB>
  void ScheduleAndForget(SchedulePriority priority,
                         BackendFunc func,  
                         const ArgA& a,
                         const ArgB& b) {
    DCHECK(thread_) << "History service being called after cleanup";
    LoadBackendIfNecessary();
    ScheduleTask(priority, NewRunnableMethod(history_backend_.get(), func,
                                             a, b));
  }

  template<typename BackendFunc, typename ArgA, typename ArgB, typename ArgC>
  void ScheduleAndForget(SchedulePriority priority,
                         BackendFunc func,  
                         const ArgA& a,
                         const ArgB& b,
                         const ArgC& c) {
    DCHECK(thread_) << "History service being called after cleanup";
    LoadBackendIfNecessary();
    ScheduleTask(priority, NewRunnableMethod(history_backend_.get(), func,
                                             a, b, c));
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
    LoadBackendIfNecessary();
    ScheduleTask(priority, NewRunnableMethod(history_backend_.get(), func,
                                             a, b, c, d));
  }

  NotificationRegistrar registrar_;

  
  
  CancelableRequestConsumer internal_consumer_;

  
  
  base::Thread* thread_;

  
  
  
  
  
  
  scoped_refptr<history::HistoryBackend> history_backend_;

  
  
  
  scoped_ptr<history::InMemoryHistoryBackend> in_memory_backend_;

  
  Profile* profile_;

  
  
  bool backend_loaded_;

  
  FilePath history_dir_;
  BookmarkService* bookmark_service_;
  bool no_db_;

  
  bool needs_top_sites_migration_;

  DISALLOW_COPY_AND_ASSIGN(HistoryService);
};

#endif  
