// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_COOKIES_COOKIE_MONSTER_H_
#define NET_COOKIES_COOKIE_MONSTER_H_

#include <deque>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "net/base/net_export.h"
#include "net/cookies/canonical_cookie.h"
#include "net/cookies/cookie_constants.h"
#include "net/cookies/cookie_store.h"

class GURL;

namespace base {
class Histogram;
class HistogramBase;
class TimeTicks;
}  

namespace net {

class ParsedCookie;

class NET_EXPORT CookieMonster : public CookieStore {
 public:
  class Delegate;
  class PersistentCookieStore;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  
  
  
  
  
  
  
  

  
  
  
  
  
  
  
  
  
  typedef std::multimap<std::string, CanonicalCookie*> CookieMap;
  typedef std::pair<CookieMap::iterator, CookieMap::iterator> CookieMapItPair;
  typedef std::vector<CookieMap::iterator> CookieItVector;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static const size_t kDomainMaxCookies;
  static const size_t kDomainPurgeCookies;
  static const size_t kMaxCookies;
  static const size_t kPurgeCookies;

  
  static const size_t kDomainCookiesQuotaLow;
  static const size_t kDomainCookiesQuotaMedium;
  static const size_t kDomainCookiesQuotaHigh;

  
  
  
  
  
  
  CookieMonster(PersistentCookieStore* store, Delegate* delegate);

  
  CookieMonster(PersistentCookieStore* store,
                Delegate* delegate,
                int last_access_threshold_milliseconds);

  
  bool InitializeFrom(const CookieList& list);

  typedef base::Callback<void(const CookieList& cookies)> GetCookieListCallback;
  typedef base::Callback<void(bool success)> DeleteCookieCallback;
  typedef base::Callback<void(bool cookies_exist)> HasCookiesForETLDP1Callback;

  
  
  
  
  
  
  void SetCookieWithDetailsAsync(const GURL& url,
                                 const std::string& name,
                                 const std::string& value,
                                 const std::string& domain,
                                 const std::string& path,
                                 const base::Time& expiration_time,
                                 bool secure,
                                 bool http_only,
                                 CookiePriority priority,
                                 const SetCookiesCallback& callback);


  
  
  
  
  void GetAllCookiesAsync(const GetCookieListCallback& callback);

  
  
  
  
  
  void GetAllCookiesForURLWithOptionsAsync(
      const GURL& url,
      const CookieOptions& options,
      const GetCookieListCallback& callback);

  
  
  void GetAllCookiesForURLAsync(const GURL& url,
                                const GetCookieListCallback& callback);

  
  void DeleteAllAsync(const DeleteCallback& callback);

  
  
  
  
  void DeleteAllForHostAsync(const GURL& url,
                             const DeleteCallback& callback);

  
  
  
  void DeleteAllCreatedBetweenForHostAsync(const base::Time delete_begin,
                                           const base::Time delete_end,
                                           const GURL& url,
                                           const DeleteCallback& callback);

  
  void DeleteCanonicalCookieAsync(const CanonicalCookie& cookie,
                                  const DeleteCookieCallback& callback);

  
  void HasCookiesForETLDP1Async(const std::string& etldp1,
                                const HasCookiesForETLDP1Callback& callback);

  
  
  
  void SetCookieableSchemes(const char* schemes[], size_t num_schemes);

  
  
  void SetEnableFileScheme(bool accept);

  
  
  
  void SetKeepExpiredCookies();

  
  void SetForceKeepSessionState();

  
  
  
  
  static void EnableFileScheme();

  
  
  
  
  
  
  void FlushStore(const base::Closure& callback);

  

  
  
  virtual void SetCookieWithOptionsAsync(
      const GURL& url,
      const std::string& cookie_line,
      const CookieOptions& options,
      const SetCookiesCallback& callback) OVERRIDE;

  
  
  
  virtual void GetCookiesWithOptionsAsync(
      const GURL& url,
      const CookieOptions& options,
      const GetCookiesCallback& callback) OVERRIDE;

  
  virtual void DeleteCookieAsync(
      const GURL& url, const std::string& cookie_name,
      const base::Closure& callback) OVERRIDE;

  
  
  
  virtual void DeleteAllCreatedBetweenAsync(
      const base::Time& delete_begin,
      const base::Time& delete_end,
      const DeleteCallback& callback) OVERRIDE;

  virtual void DeleteSessionCookiesAsync(const DeleteCallback&) OVERRIDE;

  virtual CookieMonster* GetCookieMonster() OVERRIDE;

  
  
  
  void SetPersistSessionCookies(bool persist_session_cookies);

  
  
  
  
  
  void ValidateMap(int arg);

  
  
  bool IsCookieableScheme(const std::string& scheme);

  
  static const char* kDefaultCookieableSchemes[];
  static const int kDefaultCookieableSchemesCount;

 private:
  
  class CookieMonsterTask;
  template <typename Result> class DeleteTask;
  class DeleteAllCreatedBetweenTask;
  class DeleteAllCreatedBetweenForHostTask;
  class DeleteAllForHostTask;
  class DeleteAllTask;
  class DeleteCookieTask;
  class DeleteCanonicalCookieTask;
  class GetAllCookiesForURLWithOptionsTask;
  class GetAllCookiesTask;
  class GetCookiesWithOptionsTask;
  class SetCookieWithDetailsTask;
  class SetCookieWithOptionsTask;
  class DeleteSessionCookiesTask;
  class HasCookiesForETLDP1Task;

  
  
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest,
                           TestCookieDeleteAllCreatedBetweenTimestamps);
  
  FRIEND_TEST_ALL_PREFIXES(MultiThreadedCookieMonsterTest,
                           ThreadCheckDeleteAllCreatedBetweenForHost);

  
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, TestHostGarbageCollection);
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, TestTotalGarbageCollection);
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, GarbageCollectionTriggers);
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, TestGCTimes);

  
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, TestDomainTree);
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, TestImport);
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, GetKey);
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, TestGetKey);

  
  FRIEND_TEST_ALL_PREFIXES(CookieMonsterTest, ShortLivedSessionCookies);

  
  
  
  
  
  
  
  
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

    
    
    
    DELETE_COOKIE_CONTROL_CHAR,

    DELETE_COOKIE_LAST_ENTRY
  };

  
  
  static const int kSafeFromGlobalPurgeDays;

  
  static const int kRecordStatisticsIntervalSeconds = 10 * 60;

  virtual ~CookieMonster();

  
  
  
  bool SetCookieWithDetails(const GURL& url,
                            const std::string& name,
                            const std::string& value,
                            const std::string& domain,
                            const std::string& path,
                            const base::Time& expiration_time,
                            bool secure,
                            bool http_only,
                            CookiePriority priority);

  CookieList GetAllCookies();

  CookieList GetAllCookiesForURLWithOptions(const GURL& url,
                                            const CookieOptions& options);

  CookieList GetAllCookiesForURL(const GURL& url);

  int DeleteAll(bool sync_to_store);

  int DeleteAllCreatedBetween(const base::Time& delete_begin,
                              const base::Time& delete_end);

  int DeleteAllForHost(const GURL& url);
  int DeleteAllCreatedBetweenForHost(const base::Time delete_begin,
                                     const base::Time delete_end,
                                     const GURL& url);

  bool DeleteCanonicalCookie(const CanonicalCookie& cookie);

  bool SetCookieWithOptions(const GURL& url,
                            const std::string& cookie_line,
                            const CookieOptions& options);

  std::string GetCookiesWithOptions(const GURL& url,
                                    const CookieOptions& options);

  void DeleteCookie(const GURL& url, const std::string& cookie_name);

  bool SetCookieWithCreationTime(const GURL& url,
                                 const std::string& cookie_line,
                                 const base::Time& creation_time);

  int DeleteSessionCookies();

  bool HasCookiesForETLDP1(const std::string& etldp1);

  
  
  
  
  void InitIfNecessary() {
    if (!initialized_) {
      if (store_.get()) {
        InitStore();
      } else {
        loaded_ = true;
      }
      initialized_ = true;
    }
  }

  
  
  void InitStore();

  
  
  
  
  void OnLoaded(base::TimeTicks beginning_time,
                const std::vector<CanonicalCookie*>& cookies);

  
  
  
  
  
  void OnKeyLoaded(
    const std::string& key,
    const std::vector<CanonicalCookie*>& cookies);

  
  void StoreLoadedCookies(const std::vector<CanonicalCookie*>& cookies);

  
  void InvokeQueue();

  
  
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

  
  
  CookieMap::iterator InternalInsertCookie(const std::string& key,
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

  
  
  int GarbageCollectDeleteRange(const base::Time& current,
                                DeletionCause cause,
                                CookieItVector::iterator cookie_its_begin,
                                CookieItVector::iterator cookie_its_end);

  
  
  std::string GetKey(const std::string& domain) const;

  bool HasCookieableScheme(const GURL& url);

  

  
  
  void RecordPeriodicStats(const base::Time& current_time);

  
  
  void InitializeHistograms();

  
  
  base::Time CurrentTime();

  
  
  void DoCookieTask(const scoped_refptr<CookieMonsterTask>& task_item);

  
  
  void DoCookieTaskForURL(const scoped_refptr<CookieMonsterTask>& task_item,
    const GURL& url);

  
  
  base::HistogramBase* histogram_expiration_duration_minutes_;
  base::HistogramBase* histogram_between_access_interval_minutes_;
  base::HistogramBase* histogram_evicted_last_access_minutes_;
  base::HistogramBase* histogram_count_;
  base::HistogramBase* histogram_domain_count_;
  base::HistogramBase* histogram_etldp1_count_;
  base::HistogramBase* histogram_domain_per_etldp1_count_;
  base::HistogramBase* histogram_number_duplicate_db_cookies_;
  base::HistogramBase* histogram_cookie_deletion_cause_;
  base::HistogramBase* histogram_time_get_;
  base::HistogramBase* histogram_time_mac_;
  base::HistogramBase* histogram_time_blocked_on_load_;

  CookieMap cookies_;

  
  
  bool initialized_;

  
  
  bool loaded_;

  
  std::set<std::string> keys_loaded_;

  
  
  
  std::map<std::string, std::deque<scoped_refptr<CookieMonsterTask> > >
      tasks_pending_for_key_;

  
  
  std::queue<scoped_refptr<CookieMonsterTask> > tasks_pending_;

  scoped_refptr<PersistentCookieStore> store_;

  base::Time last_time_seen_;

  
  
  const base::TimeDelta last_access_threshold_;

  
  
  
  
  
  
  
  
  
  base::Time earliest_access_time_;

  
  
  
  
  std::set<int64> creation_times_;

  std::vector<std::string> cookieable_schemes_;

  scoped_refptr<Delegate> delegate_;

  
  base::Lock lock_;

  base::Time last_statistic_record_time_;

  bool keep_expired_cookies_;
  bool persist_session_cookies_;

  
  
  
  static bool default_enable_file_scheme_;

  DISALLOW_COPY_AND_ASSIGN(CookieMonster);
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
  
  virtual void OnCookieChanged(const CanonicalCookie& cookie,
                               bool removed,
                               ChangeCause cause) = 0;
 protected:
  friend class base::RefCountedThreadSafe<CookieMonster::Delegate>;
  virtual ~Delegate() {}
};

typedef base::RefCountedThreadSafe<CookieMonster::PersistentCookieStore>
    RefcountedPersistentCookieStore;

class NET_EXPORT CookieMonster::PersistentCookieStore
    : public RefcountedPersistentCookieStore {
 public:
  typedef base::Callback<void(const std::vector<CanonicalCookie*>&)>
      LoadedCallback;

  
  
  
  virtual void Load(const LoadedCallback& loaded_callback) = 0;

  
  
  
  
  
  virtual void LoadCookiesForKey(const std::string& key,
                                 const LoadedCallback& loaded_callback) = 0;

  virtual void AddCookie(const CanonicalCookie& cc) = 0;
  virtual void UpdateCookieAccessTime(const CanonicalCookie& cc) = 0;
  virtual void DeleteCookie(const CanonicalCookie& cc) = 0;

  
  virtual void SetForceKeepSessionState() = 0;

  
  virtual void Flush(const base::Closure& callback) = 0;

 protected:
  PersistentCookieStore() {}
  virtual ~PersistentCookieStore() {}

 private:
  friend class base::RefCountedThreadSafe<PersistentCookieStore>;
  DISALLOW_COPY_AND_ASSIGN(PersistentCookieStore);
};

}  

#endif  
