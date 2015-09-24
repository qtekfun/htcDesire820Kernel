// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_PROTOCOL_MANAGER_H_
#define CHROME_BROWSER_SAFE_BROWSING_PROTOCOL_MANAGER_H_
#pragma once


#include <deque>
#include <set>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "base/time.h"
#include "base/timer.h"
#include "chrome/browser/safe_browsing/chunk_range.h"
#include "chrome/browser/safe_browsing/protocol_parser.h"
#include "chrome/browser/safe_browsing/safe_browsing_service.h"
#include "chrome/browser/safe_browsing/safe_browsing_util.h"
#include "chrome/common/net/url_fetcher.h"

namespace net {
class URLRequestStatus;
}  

#if defined(COMPILER_GCC)
namespace __gnu_cxx {
template<>
struct hash<const URLFetcher*> {
  size_t operator()(const URLFetcher* fetcher) const {
    return reinterpret_cast<size_t>(fetcher);
  }
};
}
#endif

class SafeBrowsingProtocolManager;
class SBProtocolManagerFactory {
 public:
  SBProtocolManagerFactory() {}
  virtual ~SBProtocolManagerFactory() {}
  virtual SafeBrowsingProtocolManager* CreateProtocolManager(
      SafeBrowsingService* sb_service,
      const std::string& client_name,
      const std::string& client_key,
      const std::string& wrapped_key,
      net::URLRequestContextGetter* request_context_getter,
      const std::string& info_url_prefix,
      const std::string& mackey_url_prefix,
      bool disable_auto_update) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(SBProtocolManagerFactory);
};

class SafeBrowsingProtocolManager : public URLFetcher::Delegate {
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingProtocolManagerTest, TestBackOffTimes);
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingProtocolManagerTest, TestChunkStrings);
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingProtocolManagerTest, TestGetHashUrl);
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingProtocolManagerTest,
                           TestGetHashBackOffTimes);
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingProtocolManagerTest, TestMacKeyUrl);
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingProtocolManagerTest,
                           TestSafeBrowsingHitUrl);
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingProtocolManagerTest,
                           TestMalwareDetailsUrl);
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingProtocolManagerTest, TestNextChunkUrl);
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingProtocolManagerTest, TestUpdateUrl);
  friend class SafeBrowsingServiceTest;

 public:
  virtual ~SafeBrowsingProtocolManager();

  
  
  static void RegisterFactory(SBProtocolManagerFactory* factory) {
    factory_ = factory;
  }

  
  static SafeBrowsingProtocolManager* Create(
      SafeBrowsingService* sb_service,
      const std::string& client_name,
      const std::string& client_key,
      const std::string& wrapped_key,
      net::URLRequestContextGetter* request_context_getter,
      const std::string& info_url_prefix,
      const std::string& mackey_url_prefix,
      bool disable_auto_update);

  
  
  virtual void Initialize();

  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

  
  
  virtual void GetFullHash(SafeBrowsingService::SafeBrowsingCheck* check,
                           const std::vector<SBPrefix>& prefixes);

  
  void ForceScheduleNextUpdate(int next_update_msec);

  
  void GetNextUpdate();

  
  
  // manager shouldn't fetch updates since they can't be written to disk.  It
  
  void OnGetChunksComplete(const std::vector<SBListChunkRanges>& list,
                           bool database_error);

  
  void OnChunkInserted();

  
  
  
  void ReportSafeBrowsingHit(const GURL& malicious_url,
                             const GURL& page_url,
                             const GURL& referrer_url,
                             bool is_subresource,
                             SafeBrowsingService::UrlCheckResult threat_type,
                             const std::string& post_data);

  
  
  void ReportMalwareDetails(const std::string& report);

  bool is_initial_request() const { return initial_request_; }

  
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

 protected:
  
  
  
  
  SafeBrowsingProtocolManager(
      SafeBrowsingService* sb_service,
      const std::string& client_name,
      const std::string& client_key,
      const std::string& wrapped_key,
      net::URLRequestContextGetter* request_context_getter,
      const std::string& http_url_prefix,
      const std::string& https_url_prefix,
      bool disable_auto_update);
 private:
  friend class SBProtocolManagerFactoryImpl;

  
  
  
  enum SafeBrowsingRequestType {
    NO_REQUEST = 0,     
    UPDATE_REQUEST,     
    CHUNK_REQUEST,      
    GETKEY_REQUEST      
  };

  
  
  
  
  static std::string ComposeUrl(const std::string& prefix,
                                const std::string& method,
                                const std::string& client_name,
                                const std::string& version,
                                const std::string& additional_query);

  
  
  GURL UpdateUrl(bool use_mac) const;
  
  
  GURL GetHashUrl(bool use_mac) const;
  
  GURL MacKeyUrl() const;
  
  GURL SafeBrowsingHitUrl(
      const GURL& malicious_url, const GURL& page_url, const GURL& referrer_url,
      bool is_subresource,
      SafeBrowsingService::UrlCheckResult threat_type) const;
  
  GURL MalwareDetailsUrl() const;

  
  GURL NextChunkUrl(const std::string& input) const;

  
  
  
  int GetNextUpdateTime(bool back_off);

  
  
  
  int GetNextBackOffTime(int* error_count, int* multiplier);

  
  
  
  
  
  void ScheduleNextUpdate(bool back_off);

  
  
  
  
  
  void IssueUpdateRequest();

  
  void IssueChunkRequest();

  
  
  void IssueKeyRequest();

  
  
  static std::string FormatList(const SBListChunkRanges& list, bool use_mac);

  
  
  
  bool HandleServiceResponse(const GURL& url, const char* data, int length);

  
  
  void HandleReKey();

  
  
  void HandleGetHashError(const base::Time& now);

  
  void UpdateFinished(bool success);

  
  
  void UpdateResponseTimeout();

 private:
  
  
  static SBProtocolManagerFactory* factory_;

  
  SafeBrowsingService* sb_service_;

  
  
  
  scoped_ptr<URLFetcher> request_;

  
  SafeBrowsingRequestType request_type_;

  
  int update_error_count_;
  int gethash_error_count_;

  
  int update_back_off_mult_;
  int gethash_back_off_mult_;

  
  float back_off_fuzz_;

  
  std::string list_name_;

  
  
  int next_update_sec_;
  base::OneShotTimer<SafeBrowsingProtocolManager> update_timer_;

  
  std::deque<ChunkUrl> chunk_request_urls_;

  
  typedef base::hash_map<const URLFetcher*,
                         SafeBrowsingService::SafeBrowsingCheck*> HashRequests;
  HashRequests hash_requests_;

  
  enum UpdateRequestState {
    FIRST_REQUEST = 0,
    SECOND_REQUEST,
    NORMAL_REQUEST
  };
  UpdateRequestState update_state_;

  
  
  
  bool initial_request_;

  
  
  bool chunk_pending_to_write_;

  
  std::string client_key_;
  std::string wrapped_key_;

  
  base::Time last_update_;

  
  base::Time next_gethash_time_;

  
  std::string version_;

  
  base::Time chunk_request_start_;

  
  int update_size_;

  
  
  std::set<const URLFetcher*> safebrowsing_reports_;

  
  std::string client_name_;

  
  
  std::string additional_query_;

  
  scoped_refptr<net::URLRequestContextGetter> request_context_getter_;

  
  
  std::string http_url_prefix_;

  
  
  std::string https_url_prefix_;

  
  
  bool disable_auto_update_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingProtocolManager);
};

#endif  
