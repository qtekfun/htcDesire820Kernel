// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.




#ifndef NET_BASE_SDCH_MANAGER_H_
#define NET_BASE_SDCH_MANAGER_H_

#include <map>
#include <set>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "net/base/net_export.h"
#include "url/gurl.h"

namespace net {

class SdchFetcher {
 public:
  SdchFetcher() {}
  virtual ~SdchFetcher() {}

  
  
  
  virtual void Schedule(const GURL& dictionary_url) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(SdchFetcher);
};


class NET_EXPORT SdchManager : public NON_EXPORTED_BASE(base::NonThreadSafe) {
 public:
  
  
  enum ProblemCodes {
    MIN_PROBLEM_CODE,

    
    ADDED_CONTENT_ENCODING = 1,
    FIXED_CONTENT_ENCODING = 2,
    FIXED_CONTENT_ENCODINGS = 3,

    
    DECODE_HEADER_ERROR = 4,
    DECODE_BODY_ERROR = 5,

    
    OPTIONAL_GUNZIP_ENCODING_ADDED = 6,

    
    BINARY_ADDED_CONTENT_ENCODING = 7,
    BINARY_FIXED_CONTENT_ENCODING = 8,
    BINARY_FIXED_CONTENT_ENCODINGS = 9,

    
    DICTIONARY_FOUND_HAS_WRONG_DOMAIN = 10,
    DICTIONARY_FOUND_HAS_WRONG_PORT_LIST = 11,
    DICTIONARY_FOUND_HAS_WRONG_PATH = 12,
    DICTIONARY_FOUND_HAS_WRONG_SCHEME = 13,
    DICTIONARY_HASH_NOT_FOUND = 14,
    DICTIONARY_HASH_MALFORMED = 15,

    
    DICTIONARY_HAS_NO_HEADER = 20,
    DICTIONARY_HEADER_LINE_MISSING_COLON = 21,
    DICTIONARY_MISSING_DOMAIN_SPECIFIER = 22,
    DICTIONARY_SPECIFIES_TOP_LEVEL_DOMAIN = 23,
    DICTIONARY_DOMAIN_NOT_MATCHING_SOURCE_URL = 24,
    DICTIONARY_PORT_NOT_MATCHING_SOURCE_URL = 25,
    DICTIONARY_HAS_NO_TEXT = 26,
    DICTIONARY_REFERER_URL_HAS_DOT_IN_PREFIX = 27,

    
    DICTIONARY_LOAD_ATTEMPT_FROM_DIFFERENT_HOST = 30,
    DICTIONARY_SELECTED_FOR_SSL = 31,
    DICTIONARY_ALREADY_LOADED = 32,
    DICTIONARY_SELECTED_FROM_NON_HTTP = 33,
    DICTIONARY_IS_TOO_LARGE= 34,
    DICTIONARY_COUNT_EXCEEDED = 35,
    DICTIONARY_ALREADY_SCHEDULED_TO_DOWNLOAD = 36,
    DICTIONARY_ALREADY_TRIED_TO_DOWNLOAD = 37,

    
    ATTEMPT_TO_DECODE_NON_HTTP_DATA = 40,


    
    MULTIENCODING_FOR_NON_SDCH_REQUEST = 50,
    SDCH_CONTENT_ENCODE_FOR_NON_SDCH_REQUEST = 51,

    
    DOMAIN_BLACKLIST_INCLUDES_TARGET = 61,

    
    META_REFRESH_RECOVERY = 70,            
    
    
    
    META_REFRESH_UNSUPPORTED = 74,         
    CACHED_META_REFRESH_UNSUPPORTED = 75,  
    PASSING_THROUGH_NON_SDCH = 76,  
    INCOMPLETE_SDCH_CONTENT = 77,   
    PASS_THROUGH_404_CODE = 78,     

    
    
    PASS_THROUGH_OLD_CACHED = 79,   

    
    META_REFRESH_CACHED_RECOVERY = 80,  
    DISCARD_TENTATIVE_SDCH = 81,        

    
    
    

    UNFLUSHED_CONTENT = 90,    
    
    CACHE_DECODED = 92,        
    
    UNINITIALIZED = 94,        
    PRIOR_TO_DICTIONARY = 95,  
    DECODE_ERROR = 96,         

    
    LATENCY_TEST_DISALLOWED = 100,  

    MAX_PROBLEM_CODE  
  };

  
  
  static const size_t kMaxDictionarySize;
  static const size_t kMaxDictionaryCount;

  
  
  class NET_EXPORT_PRIVATE Dictionary : public base::RefCounted<Dictionary> {
   public:
    
    const std::string& text() const { return text_; }

   private:
    friend class base::RefCounted<Dictionary>;
    friend class SdchManager;  
    FRIEND_TEST_ALL_PREFIXES(SdchFilterTest, PathMatch);

    
    
    
    Dictionary(const std::string& dictionary_text,
               size_t offset,
               const std::string& client_hash,
               const GURL& url,
               const std::string& domain,
               const std::string& path,
               const base::Time& expiration,
               const std::set<int>& ports);
    ~Dictionary();

    const GURL& url() const { return url_; }
    const std::string& client_hash() const { return client_hash_; }

    
    
    bool CanAdvertise(const GURL& target_url);

    
    
    static bool CanSet(const std::string& domain, const std::string& path,
                       const std::set<int>& ports, const GURL& dictionary_url);

    
    
    bool CanUse(const GURL& referring_url);

    
    static bool PathMatch(const std::string& path,
                          const std::string& restriction);

    
    static bool DomainMatch(const GURL& url, const std::string& restriction);


    
    std::string text_;

    
    
    std::string client_hash_;

    
    
    const GURL url_;

    
    
    
    const std::string domain_;
    const std::string path_;
    const base::Time expiration_;  
    const std::set<int> ports_;

    DISALLOW_COPY_AND_ASSIGN(Dictionary);
  };

  SdchManager();
  ~SdchManager();

  
  static void Shutdown();

  
  static SdchManager* Global();

  
  static void SdchErrorRecovery(ProblemCodes problem);

  
  void set_sdch_fetcher(SdchFetcher* fetcher);

  
  static void EnableSdchSupport(bool enabled);

  static bool sdch_enabled() { return g_sdch_enabled_; }

  
  
  
  
  
  
  
  static void BlacklistDomain(const GURL& url);

  
  
  static void BlacklistDomainForever(const GURL& url);

  
  
  static void ClearBlacklistings();

  
  static void ClearDomainBlacklisting(const std::string& domain);

  
  static int BlackListDomainCount(const std::string& domain);

  
  static int BlacklistDomainExponential(const std::string& domain);

  
  
  
  
  bool IsInSupportedDomain(const GURL& url);

  
  
  
  
  void FetchDictionary(const GURL& request_url, const GURL& dictionary_url);

  
  
  bool CanFetchDictionary(const GURL& referring_url,
                          const GURL& dictionary_url) const;

  
  
  
  bool AddSdchDictionary(const std::string& dictionary_text,
                         const GURL& dictionary_url);

  
  
  
  
  
  
  
  
  void GetVcdiffDictionary(const std::string& server_hash,
                           const GURL& referring_url,
                           Dictionary** dictionary);

  
  
  
  void GetAvailDictionaryList(const GURL& target_url, std::string* list);

  
  
  
  static void GenerateHash(const std::string& dictionary_text,
                           std::string* client_hash, std::string* server_hash);

  
  
  
  
  bool AllowLatencyExperiment(const GURL& url) const;

  void SetAllowLatencyExperiment(const GURL& url, bool enable);

 private:
  typedef std::map<std::string, int> DomainCounter;
  typedef std::set<std::string> ExperimentSet;

  
  typedef std::map<std::string, Dictionary*> DictionaryMap;

  
  static SdchManager* global_;

  
  static bool g_sdch_enabled_;

  
  static void UrlSafeBase64Encode(const std::string& input,
                                  std::string* output);
  DictionaryMap dictionaries_;

  
  scoped_ptr<SdchFetcher> fetcher_;

  
  
  DomainCounter blacklisted_domains_;

  
  
  DomainCounter exponential_blacklist_count;

  
  
  ExperimentSet allow_latency_experiment_;

  DISALLOW_COPY_AND_ASSIGN(SdchManager);
};

}  

#endif  
