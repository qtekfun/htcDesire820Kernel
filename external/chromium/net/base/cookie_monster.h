// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_BASE_COOKIE_MONSTER_H_
#define NET_BASE_COOKIE_MONSTER_H_
#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/task.h"
#include "base/time.h"
#include "net/base/cookie_store.h"

class GURL;

namespace base {
class Histogram;
}

namespace net {

class CookieList;

class NET_EXPORT CookieMonster : public CookieStore {
 public:
  class CanonicalCookie;
  class Delegate;
  class ParsedCookie;
  class PersistentCookieStore;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  
  
  
  
  

  
  
  
  
  
  
  
  

  
  
  

  
  
  
  
  
  
  
  
  
  typedef std::multimap<std::string, CanonicalCookie*> CookieMap;
  typedef std::pair<CookieMap::iterator, CookieMap::iterator> CookieMapItPair;

  
  
  
  
  
  
  enum ExpiryAndKeyScheme {
    EKS_KEEP_RECENT_AND_PURGE_ETLDP1,
    EKS_DISCARD_RECENT_AND_PURGE_DOMAIN,
    EKS_LAST_ENTRY
  };

  
  
  
  
  
  
  CookieMonster(PersistentCookieStore* store, Delegate* delegate);

  
  CookieMonster(PersistentCookieStore* store,
                Delegate* delegate,
                int last_access_threshold_milliseconds);

  
  static base::Time ParseCookieTime(const std::string& time_string);

  
  
  static bool DomainIsHostOnly(const std::string& domain_string);

  
  
  
  
  
  
  bool SetCookieWithDetails(const GURL& url,
                            const std::string& name,
                            const std::string& value,
                            const std::string& domain,
                            const std::string& path,
                            const base::Time& expiration_time,
                            bool secure, bool http_only);

  
  
  
  
  CookieList GetAllCookies();

  
  
  
  
  
  CookieList GetAllCookiesForURLWithOptions(const GURL& url,
                                            const CookieOptions& options);

  
  
  CookieList GetAllCookiesForURL(const GURL& url);

  
  int DeleteAll(bool sync_to_store);
  
  
  int DeleteAllCreatedBetween(const base::Time& delete_begin,
                              const base::Time& delete_end,
                              bool sync_to_store);
  
  
  int DeleteAllCreatedAfter(const base::Time& delete_begin, bool sync_to_store);

  
  
  
  
  int DeleteAllForHost(const GURL& url);

  
  bool DeleteCanonicalCookie(const CanonicalCookie& cookie);

  
  
  
  
  
  void SetCookieableSchemes(const char* schemes[], size_t num_schemes);

  
  
  
  void SetExpiryAndKeyScheme(ExpiryAndKeyScheme key_scheme);

  
  
  
  void SetKeepExpiredCookies();

  
  
  void SetClearPersistentStoreOnExit(bool clear_local_store);

  
  
  
  
  static void EnableFileScheme();

  
  
  
  
  
  void FlushStore(Task* completion_task);

  

  
  
  virtual bool SetCookieWithOptions(const GURL& url,
                                    const std::string& cookie_line,
                                    const CookieOptions& options);

  
  
  
  virtual std::string GetCookiesWithOptions(const GURL& url,
                                            const CookieOptions& options);

  
  virtual void DeleteCookie(const GURL& url, const std::string& cookie_name);

  virtual CookieMonster* GetCookieMonster();

  
  
  
  
  
  void ValidateMap(int arg);

  
  static const char* kDefaultCookieableSchemes[];
  static const int kDefaultCookieableSchemesCount;

 private:
  
  
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest,
                           TestCookieDeleteAllCreatedAfterTimestamp);
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest,
                           TestCookieDeleteAllCreatedBetweenTimestamps);

  
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, TestHostGarbageCollection);
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, TestTotalGarbageCollection);
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, GarbageCollectionTriggers);
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, TestGCTimes);

  
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, TestDomainTree);
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, TestImport);
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, GetKey);
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, TestGetKey);

  
  
  
  
  
  
  
  
  enum DeletionCause {
    DELETE_COOKIE_EXPLICIT = 0,
    DELETE_COOKIE_OVERWRITE,
    DELETE_COOKIE_EXPIRED,
    DELETE_COOKIE_EVICTED,
    DELETE_COOKIE_DUPLICATE_IN_BACKING_STORE,
    DELETE_COOKIE_DONT_RECORD,  
    DELETE_COOKIE_EVICTED_DOMAIN,
    DELETE_COOKIE_EVICTED_GLOBAL,

    
    
    DELETE_COOKIE_EVICTED_DOMAIN_PRE_SAFE,

    
    
    
    DELETE_COOKIE_EVICTED_DOMAIN_POST_SAFE,

    
    
    DELETE_COOKIE_EXPIRED_OVERWRITE,

    DELETE_COOKIE_LAST_ENTRY
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static const size_t kDomainMaxCookies;
  static const size_t kDomainPurgeCookies;
  static const size_t kMaxCookies;
  static const size_t kPurgeCookies;

  
  
  static const int kSafeFromGlobalPurgeDays;

  
  static const ExpiryAndKeyScheme expiry_and_key_default_ =
      EKS_KEEP_RECENT_AND_PURGE_ETLDP1;

  
  static const int kRecordStatisticsIntervalSeconds = 10 * 60;

  ~CookieMonster();

  bool SetCookieWithCreationTime(const GURL& url,
                                 const std::string& cookie_line,
                                 const base::Time& creation_time);

  
  
  
  
  void InitIfNecessary() {
    if (!initialized_) {
      if (store_)
        InitStore();
      initialized_ = true;
    }
  }

  
  
  void InitStore();

  
  
  void EnsureCookiesMapIsValid();

  
  
  
  int TrimDuplicateCookiesForKey(const std::string& key,
                                 CookieMap::iterator begin,
                                 CookieMap::iterator end);

  void SetDefaultCookieableSchemes();

  void FindCookiesForHostAndDomain(const GURL& url,
                                   const CookieOptions& options,
                                   bool update_access_time,
                                   std::vector<CanonicalCookie*>* cookies);

  void FindCookiesForKey(const std::string& key,
                         const GURL& url,
                         const CookieOptions& options,
                         const base::Time& current,
                         bool update_access_time,
                         std::vector<CanonicalCookie*>* cookies);

  
  
  
  
  
  
  bool DeleteAnyEquivalentCookie(const std::string& key,
                                 const CanonicalCookie& ecc,
                                 bool skip_httponly,
                                 bool already_expired);

  
  void InternalInsertCookie(const std::string& key,
                            CanonicalCookie* cc,
                            bool sync_to_store);

  
  
  
  bool SetCookieWithCreationTimeAndOptions(const GURL& url,
                                           const std::string& cookie_line,
                                           const base::Time& creation_time,
                                           const CookieOptions& options);


  
  
  bool SetCanonicalCookie(scoped_ptr<CanonicalCookie>* cc,
                          const base::Time& creation_time,
                          const CookieOptions& options);

  void InternalUpdateCookieAccessTime(CanonicalCookie* cc,
                                      const base::Time& current_time);

  
  
  void InternalDeleteCookie(CookieMap::iterator it, bool sync_to_store,
                            DeletionCause deletion_cause);

  
  
  
  
  
  
  int GarbageCollect(const base::Time& current, const std::string& key);

  
  
  
  
  
  int GarbageCollectExpired(const base::Time& current,
                            const CookieMapItPair& itpair,
                            std::vector<CookieMap::iterator>* cookie_its);

  
  
  
  
  int GarbageCollectDeleteList(const base::Time& current,
                               const base::Time& keep_accessed_after,
                               DeletionCause cause,
                               std::vector<CookieMap::iterator>& cookie_its);

  
  
  std::string GetKey(const std::string& domain) const;

  bool HasCookieableScheme(const GURL& url);

  

  
  
  void RecordPeriodicStats(const base::Time& current_time);

  
  
  void InitializeHistograms();

  
  
  base::Time CurrentTime();

  
  
  base::Histogram* histogram_expiration_duration_minutes_;
  base::Histogram* histogram_between_access_interval_minutes_;
  base::Histogram* histogram_evicted_last_access_minutes_;
  base::Histogram* histogram_count_;
  base::Histogram* histogram_domain_count_;
  base::Histogram* histogram_etldp1_count_;
  base::Histogram* histogram_domain_per_etldp1_count_;
  base::Histogram* histogram_number_duplicate_db_cookies_;
  base::Histogram* histogram_cookie_deletion_cause_;
  base::Histogram* histogram_time_get_;
  base::Histogram* histogram_time_load_;

  CookieMap cookies_;

  
  
  bool initialized_;

  
  
  ExpiryAndKeyScheme expiry_and_key_scheme_;

  scoped_refptr<PersistentCookieStore> store_;

  base::Time last_time_seen_;

  
  
  const base::TimeDelta last_access_threshold_;

  
  
  
  
  
  
  
  
  
  base::Time earliest_access_time_;

  std::vector<std::string> cookieable_schemes_;

  scoped_refptr<Delegate> delegate_;

  
  base::Lock lock_;

  base::Time last_statistic_record_time_;

  bool keep_expired_cookies_;

  static bool enable_file_scheme_;

  DISALLOW_COPY_AND_ASSIGN(CookieMonster);
};

class NET_EXPORT CookieMonster::CanonicalCookie {
 public:

  
  
  
  
  CanonicalCookie();
  CanonicalCookie(const GURL& url,
                  const std::string& name,
                  const std::string& value,
                  const std::string& domain,
                  const std::string& path,
                  const base::Time& creation,
                  const base::Time& expiration,
                  const base::Time& last_access,
                  bool secure,
                  bool httponly,
                  bool has_expires);

  
  
  
  CanonicalCookie(const GURL& url, const ParsedCookie& pc);

  ~CanonicalCookie();

  

  
  
  
  static CanonicalCookie* Create(const GURL& url,
                                 const std::string& name,
                                 const std::string& value,
                                 const std::string& domain,
                                 const std::string& path,
                                 const base::Time& creation,
                                 const base::Time& expiration,
                                 bool secure,
                                 bool http_only);

  const std::string& Source() const { return source_; }
  const std::string& Name() const { return name_; }
  const std::string& Value() const { return value_; }
  const std::string& Domain() const { return domain_; }
  const std::string& Path() const { return path_; }
  const base::Time& CreationDate() const { return creation_date_; }
  const base::Time& LastAccessDate() const { return last_access_date_; }
  bool DoesExpire() const { return has_expires_; }
#if defined(ANDROID)
  
  bool IsPersistent() const { return true; }
  bool IsSessionCookie() const { return !DoesExpire(); }
#else
  bool IsPersistent() const { return DoesExpire(); }
#endif
  const base::Time& ExpiryDate() const { return expiry_date_; }
  bool IsSecure() const { return secure_; }
  bool IsHttpOnly() const { return httponly_; }
  bool IsDomainCookie() const {
    return !domain_.empty() && domain_[0] == '.'; }
  bool IsHostCookie() const { return !IsDomainCookie(); }

  bool IsExpired(const base::Time& current) {
    return has_expires_ && current >= expiry_date_;
  }

  
  
  
  
  
  
  bool IsEquivalent(const CanonicalCookie& ecc) const {
    
    
    
    return (name_ == ecc.Name() && domain_ == ecc.Domain()
            && path_ == ecc.Path());
  }

  void SetLastAccessDate(const base::Time& date) {
    last_access_date_ = date;
  }

  bool IsOnPath(const std::string& url_path) const;
  bool IsDomainMatch(const std::string& scheme, const std::string& host) const;

  std::string DebugString() const;

  
  
  static std::string GetCookieSourceFromURL(const GURL& url);

 private:
  
  
  
  
  
  
  
  
  std::string source_;
  std::string name_;
  std::string value_;
  std::string domain_;
  std::string path_;
  base::Time creation_date_;
  base::Time expiry_date_;
  base::Time last_access_date_;
  bool secure_;
  bool httponly_;
  bool has_expires_;
};

class NET_EXPORT CookieMonster::Delegate
    : public base::RefCountedThreadSafe<CookieMonster::Delegate> {
 public:
  
  enum ChangeCause {
    
    CHANGE_COOKIE_EXPLICIT,
    
    
    CHANGE_COOKIE_OVERWRITE,
    
    CHANGE_COOKIE_EXPIRED,
    
    CHANGE_COOKIE_EVICTED,
    // The cookie was overwritten with an already-expired expiration date.
    CHANGE_COOKIE_EXPIRED_OVERWRITE
  };

  
  
  
  
  
  
  
  
  
  // a new cookie is written with the updated values, generating a notification
  
  virtual void OnCookieChanged(const CookieMonster::CanonicalCookie& cookie,
                               bool removed,
                               ChangeCause cause) = 0;
 protected:
  friend class base::RefCountedThreadSafe<CookieMonster::Delegate>;
  virtual ~Delegate() {}
};

class NET_EXPORT CookieMonster::ParsedCookie {
 public:
  typedef std::pair<std::string, std::string> TokenValuePair;
  typedef std::vector<TokenValuePair> PairList;

  
  static const size_t kMaxCookieSize = 4096;
  
  static const int kMaxPairs = 16;

  
  ParsedCookie(const std::string& cookie_line);
  ~ParsedCookie();

  
  bool IsValid() const { return is_valid_; }

  const std::string& Name() const { return pairs_[0].first; }
  const std::string& Token() const { return Name(); }
  const std::string& Value() const { return pairs_[0].second; }

  bool HasPath() const { return path_index_ != 0; }
  const std::string& Path() const { return pairs_[path_index_].second; }
  bool HasDomain() const { return domain_index_ != 0; }
  const std::string& Domain() const { return pairs_[domain_index_].second; }
  bool HasExpires() const { return expires_index_ != 0; }
  const std::string& Expires() const { return pairs_[expires_index_].second; }
  bool HasMaxAge() const { return maxage_index_ != 0; }
  const std::string& MaxAge() const { return pairs_[maxage_index_].second; }
  bool IsSecure() const { return secure_index_ != 0; }
  bool IsHttpOnly() const { return httponly_index_ != 0; }

  
  
  size_t NumberOfAttributes() const { return pairs_.size() - 1; }

  
  std::string DebugString() const;

  
  
  static std::string::const_iterator FindFirstTerminator(const std::string& s);

  
  
  
  
  
  static bool ParseToken(std::string::const_iterator* it,
                         const std::string::const_iterator& end,
                         std::string::const_iterator* token_start,
                         std::string::const_iterator* token_end);

  
  
  
  
  static void ParseValue(std::string::const_iterator* it,
                         const std::string::const_iterator& end,
                         std::string::const_iterator* value_start,
                         std::string::const_iterator* value_end);

  
  
  static std::string ParseTokenString(const std::string& token);
  static std::string ParseValueString(const std::string& value);

 private:
  static const char kTerminator[];
  static const int  kTerminatorLen;
  static const char kWhitespace[];
  static const char kValueSeparator[];
  static const char kTokenSeparator[];

  void ParseTokenValuePairs(const std::string& cookie_line);
  void SetupAttributes();

  PairList pairs_;
  bool is_valid_;
  
  
  
  
  size_t path_index_;
  size_t domain_index_;
  size_t expires_index_;
  size_t maxage_index_;
  size_t secure_index_;
  size_t httponly_index_;

  DISALLOW_COPY_AND_ASSIGN(ParsedCookie);
};

typedef base::RefCountedThreadSafe<CookieMonster::PersistentCookieStore>
    RefcountedPersistentCookieStore;

class CookieMonster::PersistentCookieStore
    : public RefcountedPersistentCookieStore {
 public:
  virtual ~PersistentCookieStore() {}

  
  
  virtual bool Load(std::vector<CookieMonster::CanonicalCookie*>* cookies) = 0;

  virtual void AddCookie(const CanonicalCookie& cc) = 0;
  virtual void UpdateCookieAccessTime(const CanonicalCookie& cc) = 0;
  virtual void DeleteCookie(const CanonicalCookie& cc) = 0;

  
  
  virtual void SetClearLocalStateOnExit(bool clear_local_state) = 0;

  
  virtual void Flush(Task* completion_task) = 0;

 protected:
  PersistentCookieStore() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(PersistentCookieStore);
};

class NET_EXPORT CookieList : public std::vector<CookieMonster::CanonicalCookie> {
};

}  

#endif  
