// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_DATABASE_MANAGER_H_
#define CHROME_BROWSER_SAFE_BROWSING_DATABASE_MANAGER_H_

#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "chrome/browser/safe_browsing/protocol_manager.h"
#include "chrome/browser/safe_browsing/safe_browsing_util.h"
#include "url/gurl.h"

class SafeBrowsingService;
class SafeBrowsingDatabase;

namespace base {
class Thread;
}

namespace net {
class URLRequestContext;
class URLRequestContextGetter;
}

namespace safe_browsing {
class ClientSideDetectionService;
class DownloadProtectionService;
}

class SafeBrowsingDatabaseManager
    : public base::RefCountedThreadSafe<SafeBrowsingDatabaseManager>,
      public SafeBrowsingProtocolManagerDelegate {
 public:
  class Client;

  
  struct SafeBrowsingCheck {
    
    
    
    
    
    SafeBrowsingCheck(const std::vector<GURL>& urls,
                      const std::vector<SBFullHash>& full_hashes,
                      Client* client,
                      safe_browsing_util::ListType check_type,
                      const std::vector<SBThreatType>& expected_threats);
    ~SafeBrowsingCheck();

    
    
    
    std::vector<GURL> urls;
    std::vector<SBThreatType> url_results;
    std::vector<SBFullHash> full_hashes;
    std::vector<SBThreatType> full_hash_results;

    Client* client;
    bool need_get_hash;
    base::TimeTicks start;  
    safe_browsing_util::ListType check_type;  
    std::vector<SBThreatType> expected_threats;
    std::vector<SBPrefix> prefix_hits;
    std::vector<SBFullHashResult> full_hits;

    
    
    
    
    
    
    scoped_ptr<base::WeakPtrFactory<
        SafeBrowsingDatabaseManager> > timeout_factory_;

   private:
    DISALLOW_COPY_AND_ASSIGN(SafeBrowsingCheck);
  };

  class Client {
   public:
    void OnSafeBrowsingResult(const SafeBrowsingCheck& check);

   protected:
    virtual ~Client() {}

    
    virtual void OnCheckBrowseUrlResult(const GURL& url,
                                        SBThreatType threat_type) {}

    
    virtual void OnCheckDownloadUrlResult(const std::vector<GURL>& url_chain,
                                          SBThreatType threat_type) {}

    
    virtual void OnCheckDownloadHashResult(const std::string& hash,
                                           SBThreatType threat_type) {}

    
    virtual void OnCheckExtensionsResult(
        const std::set<std::string>& threats) {}
  };

  
  explicit SafeBrowsingDatabaseManager(
      const scoped_refptr<SafeBrowsingService>& service);

  
  bool CanCheckUrl(const GURL& url) const;

  
  bool download_protection_enabled() const {
    return enable_download_protection_;
  }

  
  
  
  
  virtual bool CheckBrowseUrl(const GURL& url, Client* client);

  
  
  virtual bool CheckDownloadUrl(const std::vector<GURL>& url_chain,
                                Client* client);

  
  
  virtual bool CheckDownloadHash(const std::string& full_hash, Client* client);

  
  
  
  virtual bool CheckExtensionIDs(const std::set<std::string>& extension_ids,
                                 Client* client);

  
  
  
  
  
  virtual bool CheckSideEffectFreeWhitelistUrl(const GURL& url);

  
  
  
  
  
  virtual bool MatchCsdWhitelistUrl(const GURL& url);

  
  
  virtual bool MatchMalwareIP(const std::string& ip_address);

  
  
  
  
  virtual bool MatchDownloadWhitelistUrl(const GURL& url);

  
  
  
  
  virtual bool MatchDownloadWhitelistString(const std::string& str);

  
  virtual bool IsMalwareKillSwitchOn();

  
  
  void CancelCheck(Client* client);

  
  
  void HandleGetHashResults(
      SafeBrowsingCheck* check,
      const std::vector<SBFullHashResult>& full_hashes,
      bool can_cache);

  
  void PurgeMemory();

  
  
  
  
  void LogPauseDelay(base::TimeDelta time);

  
  
  
  void StartOnIOThread();

  
  
  
  void StopOnIOThread(bool shutdown);

 protected:
  virtual ~SafeBrowsingDatabaseManager();

  
  void NotifyDatabaseUpdateFinished(bool update_succeeded);

 private:
  friend class base::RefCountedThreadSafe<SafeBrowsingDatabaseManager>;
  friend class SafeBrowsingServerTest;
  friend class SafeBrowsingServiceTest;
  friend class SafeBrowsingServiceTestHelper;
  friend class SafeBrowsingDatabaseManagerTest;

  typedef std::set<SafeBrowsingCheck*> CurrentChecks;
  typedef std::vector<SafeBrowsingCheck*> GetHashRequestors;
  typedef base::hash_map<SBPrefix, GetHashRequestors> GetHashRequests;

  
  struct QueuedCheck {
    QueuedCheck(const safe_browsing_util::ListType check_type,
                Client* client,
                const GURL& url,
                const std::vector<SBThreatType>& expected_threats,
                const base::TimeTicks& start);
    ~QueuedCheck();
    safe_browsing_util::ListType check_type;
    Client* client;
    GURL url;
    std::vector<SBThreatType> expected_threats;
    base::TimeTicks start;  
  };

  
  
  
  void DoStopOnIOThread();

  
  bool DatabaseAvailable() const;

  
  
  
  
  
  bool MakeDatabaseAvailable();

  
  
  
  
  
  
  void CloseDatabase();

  
  
  SafeBrowsingDatabase* GetDatabase();

  
  void OnCheckDone(SafeBrowsingCheck* info);

  
  void GetAllChunksFromDatabase(GetChunksCallback callback);

  
  void OnGetAllChunksFromDatabase(const std::vector<SBListChunkRanges>& lists,
                                  bool database_error,
                                  GetChunksCallback callback);

  
  void OnAddChunksComplete(AddChunksCallback callback);

  
  
  
  void DatabaseLoadComplete();

  
  
  void AddDatabaseChunks(const std::string& list, SBChunkList* chunks,
                         AddChunksCallback callback);

  void DeleteDatabaseChunks(std::vector<SBChunkDelete>* chunk_deletes);

  static SBThreatType GetThreatTypeFromListname(const std::string& list_name);

  void NotifyClientBlockingComplete(Client* client, bool proceed);

  void DatabaseUpdateFinished(bool update_succeeded);

  
  void OnCloseDatabase();

  
  
  void OnResetDatabase();

  
  void CacheHashResults(const std::vector<SBPrefix>& prefixes,
                        const std::vector<SBFullHashResult>& full_hashes);

  
  void OnHandleGetHashResults(SafeBrowsingCheck* check,
                              const std::vector<SBFullHashResult>& full_hashes);

  
  
  bool HandleOneCheck(SafeBrowsingCheck* check,
                      const std::vector<SBFullHashResult>& full_hashes);

  
  void CheckDownloadHashOnSBThread(SafeBrowsingCheck* check);

  
  
  void CheckDownloadUrlOnSBThread(SafeBrowsingCheck* check);

  
  
  void TimeoutCallback(SafeBrowsingCheck* check);

  
  void CheckDownloadUrlDone(SafeBrowsingCheck* check);

  
  void CheckDownloadHashDone(SafeBrowsingCheck* check);

  
  void CheckExtensionIDsOnSBThread(SafeBrowsingCheck* check);

  
  void SafeBrowsingCheckDone(SafeBrowsingCheck* check);

  
  
  
  void StartSafeBrowsingCheck(SafeBrowsingCheck* check,
                              const base::Closure& task);

  
  virtual void ResetDatabase() OVERRIDE;
  virtual void UpdateStarted() OVERRIDE;
  virtual void UpdateFinished(bool success) OVERRIDE;
  virtual void GetChunks(GetChunksCallback callback) OVERRIDE;
  virtual void AddChunks(const std::string& list, SBChunkList* chunks,
                         AddChunksCallback callback) OVERRIDE;
  virtual void DeleteChunks(
      std::vector<SBChunkDelete>* delete_chunks) OVERRIDE;

  scoped_refptr<SafeBrowsingService> sb_service_;

  CurrentChecks checks_;

  
  GetHashRequests gethash_requests_;

  
  
  SafeBrowsingDatabase* database_;

  
  mutable base::Lock database_lock_;

  
  
  bool enabled_;

  
  
  bool enable_download_protection_;

  
  
  bool enable_csd_whitelist_;

  
  bool enable_download_whitelist_;

  
  bool enable_extension_blacklist_;

  
  bool enable_side_effect_free_whitelist_;

  
  bool enable_ip_blacklist_;

  
  
  
  
  
  scoped_ptr<base::Thread> safe_browsing_thread_;

  
  bool update_in_progress_;

  
  
  bool database_update_in_progress_;

  
  
  bool closing_database_;

  std::deque<QueuedCheck> queued_checks_;

  
  base::TimeDelta check_timeout_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingDatabaseManager);
};

#endif  
