// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_PROTOCOL_MANAGER_H_
#define CHROME_BROWSER_SAFE_BROWSING_PROTOCOL_MANAGER_H_


#include <deque>
#include <set>
#include <string>
#include <vector>

#include "base/containers/hash_tables.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/safe_browsing/chunk_range.h"
#include "chrome/browser/safe_browsing/protocol_manager_helper.h"
#include "chrome/browser/safe_browsing/protocol_parser.h"
#include "chrome/browser/safe_browsing/safe_browsing_util.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "url/gurl.h"

namespace net {
class URLFetcher;
class URLRequestContextGetter;
}  

#if defined(COMPILER_GCC)
namespace BASE_HASH_NAMESPACE {
template<>
struct hash<const net::URLFetcher*> {
  size_t operator()(const net::URLFetcher* fetcher) const {
    return reinterpret_cast<size_t>(fetcher);
  }
};
}
#endif

class SBProtocolManagerFactory;
class SafeBrowsingProtocolManagerDelegate;

class SafeBrowsingProtocolManager : public net::URLFetcherDelegate,
                                    public base::NonThreadSafe {
 public:
  
  
  
  
  
  
  typedef base::Callback<void(const std::vector<SBFullHashResult>&,
                              bool)> FullHashCallback;

  virtual ~SafeBrowsingProtocolManager();

  
  
  static void RegisterFactory(SBProtocolManagerFactory* factory) {
    factory_ = factory;
  }

  
  static SafeBrowsingProtocolManager* Create(
      SafeBrowsingProtocolManagerDelegate* delegate,
      net::URLRequestContextGetter* request_context_getter,
      const SafeBrowsingProtocolConfig& config);

  
  
  virtual void Initialize();

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  
  
  virtual void GetFullHash(const std::vector<SBPrefix>& prefixes,
                           FullHashCallback callback,
                           bool is_download);

  
  void ForceScheduleNextUpdate(base::TimeDelta interval);

  
  void GetNextUpdate();

  
  
  // manager shouldn't fetch updates since they can't be written to disk.  It
  
  void OnGetChunksComplete(const std::vector<SBListChunkRanges>& list,
                           bool database_error);

  
  base::Time last_update() const { return last_update_; }

  
  
  
  
  void set_additional_query(const std::string& query) {
    additional_query_ = query;
  }
  const std::string& additional_query() const {
    return additional_query_;
  }

  
  
  enum ResultType {
    
    
    
    GET_HASH_STATUS_200,
    GET_HASH_STATUS_204,

    
    
    
    GET_HASH_FULL_HASH_EMPTY,

    
    
    GET_HASH_FULL_HASH_HIT,

    
    
    
    GET_HASH_FULL_HASH_MISS,

    
    
    GET_HASH_RESULT_MAX
  };

  
  
  static void RecordGetHashResult(bool is_download,
                                  ResultType result_type);

  
  bool IsUpdateScheduled() const;

 protected:
  
  
  SafeBrowsingProtocolManager(
      SafeBrowsingProtocolManagerDelegate* delegate,
      net::URLRequestContextGetter* request_context_getter,
      const SafeBrowsingProtocolConfig& config);

 private:
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingProtocolManagerTest, TestBackOffTimes);
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingProtocolManagerTest, TestChunkStrings);
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingProtocolManagerTest, TestGetHashUrl);
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingProtocolManagerTest,
                           TestGetHashBackOffTimes);
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingProtocolManagerTest, TestNextChunkUrl);
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingProtocolManagerTest, TestUpdateUrl);
  friend class SafeBrowsingServerTest;
  friend class SBProtocolManagerFactoryImpl;

  
  
  
  enum SafeBrowsingRequestType {
    NO_REQUEST = 0,     
    UPDATE_REQUEST,     
    BACKUP_UPDATE_REQUEST, 
    CHUNK_REQUEST,      
  };

  
  enum BackupUpdateReason {
    BACKUP_UPDATE_REASON_CONNECT,
    BACKUP_UPDATE_REASON_HTTP,
    BACKUP_UPDATE_REASON_NETWORK,
    BACKUP_UPDATE_REASON_MAX,
  };

  
  GURL UpdateUrl() const;

  
  
  GURL BackupUpdateUrl(BackupUpdateReason reason) const;

  
  GURL GetHashUrl() const;
  

  
  GURL NextChunkUrl(const std::string& input) const;

  
  
  
  base::TimeDelta GetNextUpdateInterval(bool back_off);

  
  
  
  base::TimeDelta GetNextBackOffInterval(int* error_count,
                                         int* multiplier) const;

  
  
  
  
  
  void ScheduleNextUpdate(bool back_off);

  
  
  
  
  
  void IssueUpdateRequest();

  
  
  
  
  
  
  bool IssueBackupUpdateRequest(BackupUpdateReason reason);

  
  void IssueChunkRequest();

  
  
  static std::string FormatList(const SBListChunkRanges& list);

  
  
  
  bool HandleServiceResponse(const GURL& url, const char* data, int length);

  
  
  void HandleGetHashError(const base::Time& now);

  
  void UpdateFinished(bool success);
  void UpdateFinished(bool success, bool back_off);

  
  
  void UpdateResponseTimeout();

  
  void OnAddChunksComplete();

 private:
  
  struct FullHashDetails {
    FullHashDetails();
    FullHashDetails(FullHashCallback callback, bool is_download);
    ~FullHashDetails();

    FullHashCallback callback;
    bool is_download;
  };
  typedef base::hash_map<const net::URLFetcher*, FullHashDetails> HashRequests;

  
  
  static SBProtocolManagerFactory* factory_;

  
  SafeBrowsingProtocolManagerDelegate* delegate_;

  
  
  
  scoped_ptr<net::URLFetcher> request_;

  
  SafeBrowsingRequestType request_type_;

  
  int update_error_count_;
  int gethash_error_count_;

  
  int update_back_off_mult_;
  int gethash_back_off_mult_;

  
  float back_off_fuzz_;

  
  std::string list_name_;

  
  
  base::TimeDelta next_update_interval_;
  base::OneShotTimer<SafeBrowsingProtocolManager> update_timer_;

  
  
  base::OneShotTimer<SafeBrowsingProtocolManager> timeout_timer_;

  
  std::deque<ChunkUrl> chunk_request_urls_;

  HashRequests hash_requests_;

  
  enum UpdateRequestState {
    FIRST_REQUEST = 0,
    SECOND_REQUEST,
    NORMAL_REQUEST
  };
  UpdateRequestState update_state_;

  
  
  bool chunk_pending_to_write_;

  
  base::Time last_update_;

  
  base::Time next_gethash_time_;

  
  std::string version_;

  
  base::Time chunk_request_start_;

  
  int update_size_;

  
  std::string client_name_;

  
  
  std::string additional_query_;

  
  scoped_refptr<net::URLRequestContextGetter> request_context_getter_;

  
  std::string url_prefix_;

  
  std::string backup_url_prefixes_[BACKUP_UPDATE_REASON_MAX];

  
  BackupUpdateReason backup_update_reason_;

  
  std::string update_list_data_;

  
  
  bool disable_auto_update_;

  
  int url_fetcher_id_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingProtocolManager);
};

class SBProtocolManagerFactory {
 public:
  SBProtocolManagerFactory() {}
  virtual ~SBProtocolManagerFactory() {}
  virtual SafeBrowsingProtocolManager* CreateProtocolManager(
      SafeBrowsingProtocolManagerDelegate* delegate,
      net::URLRequestContextGetter* request_context_getter,
      const SafeBrowsingProtocolConfig& config) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(SBProtocolManagerFactory);
};

class SafeBrowsingProtocolManagerDelegate {
 public:
  typedef base::Callback<void(const std::vector<SBListChunkRanges>&, bool)>
      GetChunksCallback;
  typedef base::Callback<void(void)> AddChunksCallback;

  virtual ~SafeBrowsingProtocolManagerDelegate();

  
  
  virtual void UpdateStarted() = 0;

  
  
  virtual void UpdateFinished(bool success) = 0;

  
  virtual void ResetDatabase() = 0;

  
  
  
  
  virtual void GetChunks(GetChunksCallback callback) = 0;

  
  
  virtual void AddChunks(const std::string& list, SBChunkList* chunks,
                         AddChunksCallback callback) = 0;

  
  virtual void DeleteChunks(
      std::vector<SBChunkDelete>* delete_chunks) = 0;
};

#endif  
