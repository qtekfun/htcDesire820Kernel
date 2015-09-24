// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_SERVICE_H_
#define CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_SERVICE_H_
#pragma once

#include <deque>
#include <set>
#include <string>
#include <vector>

#include "base/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/task.h"
#include "base/time.h"
#include "chrome/browser/safe_browsing/safe_browsing_util.h"
#include "googleurl/src/gurl.h"
#include "webkit/glue/resource_type.h"

class MalwareDetails;
class PrefService;
class SafeBrowsingDatabase;
class SafeBrowsingProtocolManager;
class SafeBrowsingServiceFactory;

namespace base {
class Thread;
}

namespace net {
class URLRequestContextGetter;
}

class SafeBrowsingService
    : public base::RefCountedThreadSafe<SafeBrowsingService> {
 public:
  class Client;
  
  
  enum UrlCheckResult {
    SAFE,
    URL_PHISHING,
    URL_MALWARE,
    BINARY_MALWARE_URL,  
    BINARY_MALWARE_HASH,  
  };

  
  
  struct UnsafeResource {
    UnsafeResource();
    ~UnsafeResource();

    GURL url;
    GURL original_url;
    std::vector<GURL> redirect_urls;
    ResourceType::Type resource_type;
    UrlCheckResult threat_type;
    Client* client;
    int render_process_host_id;
    int render_view_id;
  };

  
  struct SafeBrowsingCheck {
    SafeBrowsingCheck();
    ~SafeBrowsingCheck();

    
    std::vector<GURL> urls;
    scoped_ptr<SBFullHash> full_hash;

    Client* client;
    bool need_get_hash;
    base::TimeTicks start;  
    UrlCheckResult result;
    bool is_download;  
    std::vector<SBPrefix> prefix_hits;
    std::vector<SBFullHashResult> full_hits;

    
    
    
    
    
    
    CancelableTask* timeout_task;

   private:
    DISALLOW_COPY_AND_ASSIGN(SafeBrowsingCheck);
  };

  class Client {
   public:
    virtual ~Client() {}

    void OnSafeBrowsingResult(const SafeBrowsingCheck& check);

    
    
    virtual void OnBlockingPageComplete(bool proceed) {}

   protected:
    
    virtual void OnBrowseUrlCheckResult(const GURL& url,
                                        UrlCheckResult result) {}

    
    virtual void OnDownloadUrlCheckResult(const std::vector<GURL>& url_chain,
                                          UrlCheckResult result) {}

    
    virtual void OnDownloadHashCheckResult(const std::string& hash,
                                           UrlCheckResult result) {}
  };


  
  
  static void RegisterFactory(SafeBrowsingServiceFactory* factory) {
    factory_ = factory;
  }

  
  static SafeBrowsingService* CreateSafeBrowsingService();

  
  void Initialize();

  
  void ShutDown();

  
  bool CanCheckUrl(const GURL& url) const;

  
  
  bool CanReportStats() const;

  
  
  bool DownloadBinHashNeeded() const;

  
  
  
  
  virtual bool CheckBrowseUrl(const GURL& url, Client* client);

  
  
  bool CheckDownloadUrl(const std::vector<GURL>& url_chain, Client* client);

  
  
  virtual bool CheckDownloadHash(const std::string& full_hash, Client* client);

  
  
  
  
  
  virtual bool MatchCsdWhitelistUrl(const GURL& url);

  
  
  void CancelCheck(Client* client);

  
  
  
  
  
  virtual void DisplayBlockingPage(const GURL& url,
                                   const GURL& original_url,
                                   const std::vector<GURL>& redirect_urls,
                                   ResourceType::Type resource_type,
                                   UrlCheckResult result,
                                   Client* client,
                                   int render_process_host_id,
                                   int render_view_id);

  
  
  void HandleGetHashResults(
      SafeBrowsingCheck* check,
      const std::vector<SBFullHashResult>& full_hashes,
      bool can_cache);

  
  void HandleChunk(const std::string& list, SBChunkList* chunks);
  void HandleChunkDelete(std::vector<SBChunkDelete>* chunk_deletes);

  
  void UpdateStarted();
  void UpdateFinished(bool update_succeeded);
  
  bool IsUpdateInProgress() const;

  
  void OnBlockingPageDone(const std::vector<UnsafeResource>& resources,
                          bool proceed);

  
  
  void OnNewMacKeys(const std::string& client_key,
                    const std::string& wrapped_key);

  
  void OnEnable(bool enabled);

  bool enabled() const { return enabled_; }

  bool download_protection_enabled() const {
    return enabled_ && enable_download_protection_;
  }

  
  static void RegisterPrefs(PrefService* prefs);

  
  
  
  
  
  
  void CloseDatabase();

  
  void ResetDatabase();

  
  
  
  
  void LogPauseDelay(base::TimeDelta time);

  
  
  virtual void SendSerializedMalwareDetails(const std::string& serialized);

  
  
  
  void ReportSafeBrowsingHit(const GURL& malicious_url,
                             const GURL& page_url,
                             const GURL& referrer_url,
                             bool is_subresource,
                             UrlCheckResult threat_type,
                             const std::string& post_data);

 protected:
  
  SafeBrowsingService();

  virtual ~SafeBrowsingService();

 private:
  friend class SafeBrowsingServiceFactoryImpl;

  typedef std::set<SafeBrowsingCheck*> CurrentChecks;
  typedef std::vector<SafeBrowsingCheck*> GetHashRequestors;
  typedef base::hash_map<SBPrefix, GetHashRequestors> GetHashRequests;

  
  struct WhiteListedEntry;

  
  struct QueuedCheck {
    Client* client;
    GURL url;
    base::TimeTicks start;  
  };

  friend class base::RefCountedThreadSafe<SafeBrowsingService>;
  friend class SafeBrowsingServiceTest;

  
  void OnIOInitialize(const std::string& client_key,
                      const std::string& wrapped_key,
                      net::URLRequestContextGetter* request_context_getter);

  
  void OnIOShutdown();

  
  bool DatabaseAvailable() const;

  
  
  
  
  
  bool MakeDatabaseAvailable();

  
  
  SafeBrowsingDatabase* GetDatabase();

  
  void OnCheckDone(SafeBrowsingCheck* info);

  
  void GetAllChunksFromDatabase();

  
  void OnGetAllChunksFromDatabase(const std::vector<SBListChunkRanges>& lists,
                                  bool database_error);

  
  void OnChunkInserted();

  
  
  
  void DatabaseLoadComplete();

  
  
  void HandleChunkForDatabase(const std::string& list,
                              SBChunkList* chunks);

  void DeleteChunks(std::vector<SBChunkDelete>* chunk_deletes);

  static UrlCheckResult GetResultFromListname(const std::string& list_name);

  void NotifyClientBlockingComplete(Client* client, bool proceed);

  void DatabaseUpdateFinished(bool update_succeeded);

  
  
  
  void Start();

  
  void OnCloseDatabase();

  
  
  void OnResetDatabase();

  
  void CacheHashResults(const std::vector<SBPrefix>& prefixes,
                        const std::vector<SBFullHashResult>& full_hashes);

  
  void OnHandleGetHashResults(SafeBrowsingCheck* check,
                              const std::vector<SBFullHashResult>& full_hashes);

  
  
  bool HandleOneCheck(SafeBrowsingCheck* check,
                      const std::vector<SBFullHashResult>& full_hashes);

  
  void DoDisplayBlockingPage(const UnsafeResource& resource);

  
  void ReportSafeBrowsingHitOnIOThread(const GURL& malicious_url,
                                       const GURL& page_url,
                                       const GURL& referrer_url,
                                       bool is_subresource,
                                       UrlCheckResult threat_type,
                                       const std::string& post_data);

  
  void CheckDownloadHashOnSBThread(SafeBrowsingCheck* check);

  
  
  void CheckDownloadUrlOnSBThread(SafeBrowsingCheck* check);

  
  
  void TimeoutCallback(SafeBrowsingCheck* check);

  
  void CheckDownloadUrlDone(SafeBrowsingCheck* check);

  
  void CheckDownloadHashDone(SafeBrowsingCheck* check);

  
  void SafeBrowsingCheckDone(SafeBrowsingCheck* check);

  
  
  
  void StartDownloadCheck(SafeBrowsingCheck* check,
                          Client* client,
                          CancelableTask* task,
                          int64 timeout_ms);

  
  
  
  static SafeBrowsingServiceFactory* factory_;

  CurrentChecks checks_;

  
  GetHashRequests gethash_requests_;

  
  
  SafeBrowsingDatabase* database_;

  
  mutable base::Lock database_lock_;

  
  SafeBrowsingProtocolManager* protocol_manager_;

  std::vector<WhiteListedEntry> white_listed_entries_;

  
  
  bool enabled_;

  
  
  bool enable_download_protection_;

  
  
  bool enable_csd_whitelist_;

  
  
  
  
  
  scoped_ptr<base::Thread> safe_browsing_thread_;

  
  bool update_in_progress_;

  
  
  bool database_update_in_progress_;

  
  
  bool closing_database_;

  std::deque<QueuedCheck> queued_checks_;

  
  
  int64 download_urlcheck_timeout_ms_;

  
  int64 download_hashcheck_timeout_ms_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingService);
};

class SafeBrowsingServiceFactory {
 public:
  SafeBrowsingServiceFactory() { }
  virtual ~SafeBrowsingServiceFactory() { }
  virtual SafeBrowsingService* CreateSafeBrowsingService() = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingServiceFactory);
};

#endif  
