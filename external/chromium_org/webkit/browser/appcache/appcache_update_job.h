// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_APPCACHE_APPCACHE_UPDATE_JOB_H_
#define WEBKIT_BROWSER_APPCACHE_APPCACHE_UPDATE_JOB_H_

#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "net/base/completion_callback.h"
#include "net/http/http_response_headers.h"
#include "net/url_request/url_request.h"
#include "url/gurl.h"
#include "webkit/browser/appcache/appcache.h"
#include "webkit/browser/appcache/appcache_host.h"
#include "webkit/browser/appcache/appcache_response.h"
#include "webkit/browser/appcache/appcache_service.h"
#include "webkit/browser/appcache/appcache_storage.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/appcache/appcache_interfaces.h"

namespace appcache {

class HostNotifier;

class WEBKIT_STORAGE_BROWSER_EXPORT AppCacheUpdateJob
    : public AppCacheStorage::Delegate,
      public AppCacheHost::Observer,
      public AppCacheService::Observer {
 public:
  AppCacheUpdateJob(AppCacheService* service, AppCacheGroup* group);
  virtual ~AppCacheUpdateJob();

  
  
  void StartUpdate(AppCacheHost* host, const GURL& new_master_resource);

 private:
  friend class AppCacheUpdateJobTest;
  class URLFetcher;

  
  
  typedef std::vector<AppCacheHost*> PendingHosts;
  typedef std::map<GURL, PendingHosts> PendingMasters;
  typedef std::map<GURL, URLFetcher*> PendingUrlFetches;
  typedef std::map<int64, GURL> LoadingResponses;

  static const int kRerunDelayMs = 1000;

  
  
  

  enum UpdateType {
    UNKNOWN_TYPE,
    UPGRADE_ATTEMPT,
    CACHE_ATTEMPT,
  };

  enum InternalUpdateState {
    FETCH_MANIFEST,
    NO_UPDATE,
    DOWNLOADING,

    
    REFETCH_MANIFEST,
    CACHE_FAILURE,
    CANCELLED,
    COMPLETED,
  };

  enum StoredState {
    UNSTORED,
    STORING,
    STORED,
  };

  struct UrlToFetch {
    UrlToFetch(const GURL& url, bool checked, AppCacheResponseInfo* info);
    ~UrlToFetch();

    GURL url;
    bool storage_checked;
    scoped_refptr<AppCacheResponseInfo> existing_response_info;
  };

  class URLFetcher : public net::URLRequest::Delegate {
   public:
    enum FetchType {
      MANIFEST_FETCH,
      URL_FETCH,
      MASTER_ENTRY_FETCH,
      MANIFEST_REFETCH,
    };
    URLFetcher(const GURL& url,
               FetchType fetch_type,
               AppCacheUpdateJob* job);
    virtual ~URLFetcher();
    void Start();
    FetchType fetch_type() const { return fetch_type_; }
    net::URLRequest* request() const { return request_.get(); }
    const AppCacheEntry& existing_entry() const { return existing_entry_; }
    const std::string& manifest_data() const { return manifest_data_; }
    AppCacheResponseWriter* response_writer() const {
      return response_writer_.get();
    }
    void set_existing_response_headers(net::HttpResponseHeaders* headers) {
      existing_response_headers_ = headers;
    }
    void set_existing_entry(const AppCacheEntry& entry) {
      existing_entry_ = entry;
    }

   private:
    
    virtual void OnReceivedRedirect(net::URLRequest* request,
                                    const GURL& new_url,
                                    bool* defer_redirect) OVERRIDE;
    virtual void OnResponseStarted(net::URLRequest* request) OVERRIDE;
    virtual void OnReadCompleted(net::URLRequest* request,
                                 int bytes_read) OVERRIDE;

    void AddConditionalHeaders(const net::HttpResponseHeaders* headers);
    void OnWriteComplete(int result);
    void ReadResponseData();
    bool ConsumeResponseData(int bytes_read);
    void OnResponseCompleted();
    bool MaybeRetryRequest();

    GURL url_;
    AppCacheUpdateJob* job_;
    FetchType fetch_type_;
    int retry_503_attempts_;
    scoped_refptr<net::IOBuffer> buffer_;
    scoped_ptr<net::URLRequest> request_;
    AppCacheEntry existing_entry_;
    scoped_refptr<net::HttpResponseHeaders> existing_response_headers_;
    std::string manifest_data_;
    scoped_ptr<AppCacheResponseWriter> response_writer_;
  };  

  AppCacheResponseWriter* CreateResponseWriter();

  
  virtual void OnResponseInfoLoaded(AppCacheResponseInfo* response_info,
                                    int64 response_id) OVERRIDE;
  virtual void OnGroupAndNewestCacheStored(AppCacheGroup* group,
                                           AppCache* newest_cache,
                                           bool success,
                                           bool would_exceed_quota) OVERRIDE;
  virtual void OnGroupMadeObsolete(AppCacheGroup* group, bool success) OVERRIDE;

  
  virtual void OnCacheSelectionComplete(AppCacheHost* host) OVERRIDE {}  
  virtual void OnDestructionImminent(AppCacheHost* host) OVERRIDE;

  
  virtual void OnServiceReinitialized(
      AppCacheStorageReference* old_storage) OVERRIDE;

  void HandleCacheFailure(const std::string& error_message);

  void FetchManifest(bool is_first_fetch);
  void HandleManifestFetchCompleted(URLFetcher* fetcher);
  void ContinueHandleManifestFetchCompleted(bool changed);

  void HandleUrlFetchCompleted(URLFetcher* fetcher);
  void HandleMasterEntryFetchCompleted(URLFetcher* fetcher);

  void HandleManifestRefetchCompleted(URLFetcher* fetcher);
  void OnManifestInfoWriteComplete(int result);
  void OnManifestDataWriteComplete(int result);

  void StoreGroupAndCache();

  void NotifySingleHost(AppCacheHost* host, EventID event_id);
  void NotifyAllAssociatedHosts(EventID event_id);
  void NotifyAllProgress(const GURL& url);
  void NotifyAllFinalProgress();
  void NotifyAllError(const std::string& error_message);
  void AddAllAssociatedHostsToNotifier(HostNotifier* notifier);

  
  
  void CheckIfManifestChanged();
  void OnManifestDataReadComplete(int result);

  
  
  void BuildUrlFileList(const Manifest& manifest);
  void AddUrlToFileList(const GURL& url, int type);
  void FetchUrls();
  void CancelAllUrlFetches();
  bool ShouldSkipUrlFetch(const AppCacheEntry& entry);

  
  
  
  bool AlreadyFetchedEntry(const GURL& url, int entry_type);

  
  
  
  void AddMasterEntryToFetchList(AppCacheHost* host, const GURL& url,
                                 bool is_new);
  void FetchMasterEntries();
  void CancelAllMasterEntryFetches(const std::string& error_message);

  
  
  
  
  bool MaybeLoadFromNewestCache(const GURL& url, AppCacheEntry& entry);
  void LoadFromNewestCacheFailed(const GURL& url,
                                 AppCacheResponseInfo* newest_response_info);

  
  
  
  void MaybeCompleteUpdate();

  
  
  void ScheduleUpdateRetry(int delay_ms);

  void Cancel();
  void ClearPendingMasterEntries();
  void DiscardInprogressCache();
  void DiscardDuplicateResponses();

  
  void DeleteSoon();

  bool IsTerminating() { return internal_state_ >= REFETCH_MANIFEST ||
                                stored_state_ != UNSTORED; }

  AppCacheService* service_;
  const GURL manifest_url_;  

  
  
  
  scoped_refptr<AppCacheStorageReference> disabled_storage_reference_;

  scoped_refptr<AppCache> inprogress_cache_;

  AppCacheGroup* group_;

  UpdateType update_type_;
  InternalUpdateState internal_state_;

  PendingMasters pending_master_entries_;
  size_t master_entries_completed_;

  
  
  
  
  
  std::set<GURL> master_entries_to_fetch_;
  PendingUrlFetches master_entry_fetches_;

  
  AppCache::EntryMap url_file_list_;
  size_t url_fetches_completed_;

  
  
  
  std::deque<UrlToFetch> urls_to_fetch_;

  
  
  LoadingResponses loading_responses_;

  
  URLFetcher* manifest_fetcher_;
  PendingUrlFetches pending_url_fetches_;

  
  std::string manifest_data_;
  scoped_ptr<net::HttpResponseInfo> manifest_response_info_;
  scoped_ptr<AppCacheResponseWriter> manifest_response_writer_;
  scoped_refptr<net::IOBuffer> read_manifest_buffer_;
  std::string loaded_manifest_data_;
  scoped_ptr<AppCacheResponseReader> manifest_response_reader_;

  
  
  std::vector<GURL> added_master_entries_;

  
  
  std::vector<int64> stored_response_ids_;

  
  
  
  
  
  std::vector<int64> duplicate_response_ids_;

  
  StoredState stored_state_;

  AppCacheStorage* storage_;

  FRIEND_TEST_ALL_PREFIXES(AppCacheGroupTest, QueueUpdate);

  DISALLOW_COPY_AND_ASSIGN(AppCacheUpdateJob);
};

}  

#endif  
