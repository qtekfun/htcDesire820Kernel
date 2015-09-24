// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_NET_PREDICTOR_H_
#define CHROME_BROWSER_NET_PREDICTOR_H_

#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/net/referrer.h"
#include "chrome/browser/net/spdyproxy/proxy_advisor.h"
#include "chrome/browser/net/timed_cache.h"
#include "chrome/browser/net/url_info.h"
#include "chrome/common/net/predictor_common.h"
#include "net/base/host_port_pair.h"

class IOThread;
class PrefService;
class Profile;

namespace base {
class ListValue;
class WaitableEvent;
}

namespace net {
class HostResolver;
class URLRequestContextGetter;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace chrome_browser_net {

typedef chrome_common_net::UrlList UrlList;
typedef chrome_common_net::NameList NameList;
typedef std::map<GURL, UrlInfo> Results;

class Predictor {
 public:
  
  
  static const int kPredictorReferrerVersion;

  
  
  
  
  
  static const size_t kMaxSpeculativeParallelResolves;

  
  
  
  
  
  
  
  static const int kTypicalSpeculativeGroupSize;

  
  
  
  
  static const int kMaxSpeculativeResolveQueueDelayMs;

  
  
  
  
  
  static const int kMaxUnusedSocketLifetimeSecondsWithoutAGet;

  
  
  explicit Predictor(bool preconnect_enabled);

  virtual ~Predictor();

  
  
  static Predictor* CreatePredictor(bool preconnect_enabled,
                                    bool simple_shutdown);

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  

  virtual void InitNetworkPredictor(PrefService* user_prefs,
                                    PrefService* local_state,
                                    IOThread* io_thread,
                                    net::URLRequestContextGetter* getter);

  
  
  
  void AnticipateOmniboxUrl(const GURL& url, bool preconnectable);

  
  void PreconnectUrlAndSubresources(const GURL& url,
                                    const GURL& first_party_for_cookies);

  static UrlList GetPredictedUrlListAtStartup(PrefService* user_prefs,
                                              PrefService* local_state);

  static void set_max_queueing_delay(int max_queueing_delay_ms);

  static void set_max_parallel_resolves(size_t max_parallel_resolves);

  virtual void ShutdownOnUIThread();

  

  

  
  void Shutdown();

  
  
  
  
  
  void DiscardAllResults();

  
  void ResolveList(const UrlList& urls,
                   UrlInfo::ResolutionMotivation motivation);

  void Resolve(const GURL& url, UrlInfo::ResolutionMotivation motivation);

  
  
  
  
  void LearnFromNavigation(const GURL& referring_url, const GURL& target_url);

  
  static void PredictorGetHtmlInfo(Predictor* predictor, std::string* output);

  
  void GetHtmlReferrerLists(std::string* output);

  
  
  void GetHtmlInfo(std::string* output);

  
  
  
  
  
  
  void TrimReferrersNow();

  
  
  void SerializeReferrers(base::ListValue* referral_list);

  
  
  
  void DeserializeReferrers(const base::ListValue& referral_list);

  void DeserializeReferrersThenDelete(base::ListValue* referral_list);

  void DiscardInitialNavigationHistory();

  void FinalizeInitializationOnIOThread(
      const std::vector<GURL>& urls_to_prefetch,
      base::ListValue* referral_list,
      IOThread* io_thread,
      bool predictor_enabled);

  
  
  void LearnAboutInitialNavigation(const GURL& url);

  
  
  void DnsPrefetchList(const NameList& hostnames);

  
  
  void DnsPrefetchMotivatedList(const UrlList& urls,
                                UrlInfo::ResolutionMotivation motivation);

  
  
  void SaveStateForNextStartupAndTrim(PrefService* prefs);

  void SaveDnsPrefetchStateForNextStartupAndTrim(
      base::ListValue* startup_list,
      base::ListValue* referral_list,
      base::WaitableEvent* completion);

  
  
  void EnablePredictor(bool enable);

  void EnablePredictorOnIOThread(bool enable);

  
  
  void PreconnectUrl(const GURL& url, const GURL& first_party_for_cookies,
                     UrlInfo::ResolutionMotivation motivation, int count);

  void PreconnectUrlOnIOThread(const GURL& url,
                               const GURL& first_party_for_cookies,
                               UrlInfo::ResolutionMotivation motivation,
                               int count);

  void RecordPreconnectTrigger(const GURL& url);

  void RecordPreconnectNavigationStat(const std::vector<GURL>& url_chain,
                                      bool is_subresource);

  void RecordLinkNavigation(const GURL& url);

  

  

  
  
  
  
  
  void PredictFrameSubresources(const GURL& url,
                                const GURL& first_party_for_cookies);

  
  
  
  static GURL CanonicalizeUrl(const GURL& url);

  
  void SetHostResolver(net::HostResolver* host_resolver) {
    host_resolver_ = host_resolver;
  }
  
  void SetProxyAdvisor(ProxyAdvisor* proxy_advisor) {
    proxy_advisor_.reset(proxy_advisor);
  }
  
  size_t max_concurrent_dns_lookups() const {
    return max_concurrent_dns_lookups_;
  }
  
  void SetShutdown(bool shutdown) {
    shutdown_ = shutdown;
  }

  
  bool preconnect_enabled() const {
    return preconnect_enabled_;
  }

  
  bool predictor_enabled() const {
    return predictor_enabled_;
  }


 private:
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, BenefitLookupTest);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, ShutdownWhenResolutionIsPendingTest);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, SingleLookupTest);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, ConcurrentLookupTest);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, MassiveConcurrentLookupTest);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, PriorityQueuePushPopTest);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, PriorityQueueReorderTest);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, ReferrerSerializationTrimTest);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, SingleLookupTestWithDisabledAdvisor);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, SingleLookupTestWithEnabledAdvisor);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, TestSimplePreconnectAdvisor);
  friend class WaitForResolutionHelper;  

  class LookupRequest;

  
  
  
  
  
  
  
  
  class HostNameQueue {
   public:
    HostNameQueue();
    ~HostNameQueue();
    void Push(const GURL& url,
              UrlInfo::ResolutionMotivation motivation);
    bool IsEmpty() const;
    GURL Pop();

   private:
    
    std::queue<GURL> rush_queue_;
    
    
    std::queue<GURL> background_queue_;

  DISALLOW_COPY_AND_ASSIGN(HostNameQueue);
  };

  
  
  
  
  
  class InitialObserver {
   public:
    InitialObserver();
    ~InitialObserver();
    
    typedef std::map<GURL, base::TimeTicks> FirstNavigations;

    
    void Append(const GURL& url, Predictor* predictor);

    
    void GetFirstResolutionsHtml(std::string* output);

    
    void GetInitialDnsResolutionList(base::ListValue* startup_list);

    
    void DiscardInitialNavigationHistory() { first_navigations_.clear(); }

   private:
    
    FirstNavigations first_navigations_;

    
    static const size_t kStartupResolutionCount = 10;
  };

  
  
  
  typedef std::map<GURL, Referrer> Referrers;

  
  
  
  static const double kPreconnectWorthyExpectedValue;
  static const double kDNSPreresolutionWorthyExpectedValue;
  
  
  static const double kDiscardableExpectedValue;
  
  
  
  
  static const double kReferrerTrimRatio;

  
  
  
  static const int64 kDurationBetweenTrimmingsHours;
  
  static const int64 kDurationBetweenTrimmingIncrementsSeconds;
  
  static const size_t kUrlsTrimmedPerIncrement;

  
  
  bool WasFound(const GURL& url) const {
    Results::const_iterator it(results_.find(url));
    return (it != results_.end()) &&
            it->second.was_found();
  }

  
  
  base::TimeDelta GetResolutionDuration(const GURL& url) {
    if (results_.find(url) == results_.end())
      return UrlInfo::NullDuration();
    return results_[url].resolve_duration();
  }

  
  size_t peak_pending_lookups() const { return peak_pending_lookups_; }

  
  
  
  void AdviseProxy(const GURL& url,
                   UrlInfo::ResolutionMotivation motivation,
                   bool is_preconnect);

  

  
  
  
  void PrepareFrameSubresources(const GURL& url,
                                const GURL& first_party_for_cookies);

  
  void OnLookupFinished(LookupRequest* request, const GURL& url, bool found);

  
  void LookupFinished(LookupRequest* request,
                      const GURL& url, bool found);

  
  
  
  
  UrlInfo* AppendToResolutionQueue(const GURL& url,
                                   UrlInfo::ResolutionMotivation motivation);

  
  
  
  
  
  
  
  
  
  
  
  
  bool CongestionControlPerformed(UrlInfo* info);

  
  
  void StartSomeQueuedResolutions();

  
  
  void TrimReferrers();

  
  void LoadUrlsForTrimming();

  
  void PostIncrementalTrimTask();

  
  
  
  void IncrementalTrimReferrers(bool trim_all_now);

  
  
  void AdviseProxyOnIOThread(const GURL& url,
                             UrlInfo::ResolutionMotivation motivation,
                             bool is_preconnect);

  

  scoped_ptr<InitialObserver> initial_observer_;

  
  
  scoped_refptr<net::URLRequestContextGetter> url_request_context_getter_;

  
  
  bool predictor_enabled_;

  
  HostNameQueue work_queue_;

  
  Results results_;

  std::set<LookupRequest*> pending_lookups_;

  
  size_t peak_pending_lookups_;

  
  bool shutdown_;

  
  
  
  
  const size_t max_concurrent_dns_lookups_;

  
  
  const base::TimeDelta max_dns_queue_delay_;

  
  net::HostResolver* host_resolver_;

  
  
  bool preconnect_enabled_;

  
  std::string last_omnibox_host_;

  
  base::TimeTicks last_omnibox_preresolve_;

  
  
  int consecutive_omnibox_preconnect_count_;

  
  base::TimeTicks last_omnibox_preconnect_;

  class PreconnectUsage;
  scoped_ptr<PreconnectUsage> preconnect_usage_;

  
  
  
  
  Referrers referrers_;

  
  
  std::vector<GURL> urls_being_trimmed_;

  
  base::TimeTicks next_trim_time_;

  scoped_ptr<base::WeakPtrFactory<Predictor> > weak_factory_;

  scoped_ptr<ProxyAdvisor> proxy_advisor_;

  DISALLOW_COPY_AND_ASSIGN(Predictor);
};

class SimplePredictor : public Predictor {
 public:
  explicit SimplePredictor(bool preconnect_enabled)
      : Predictor(preconnect_enabled) {}
  virtual ~SimplePredictor() {}
  virtual void InitNetworkPredictor(
      PrefService* user_prefs,
      PrefService* local_state,
      IOThread* io_thread,
      net::URLRequestContextGetter* getter) OVERRIDE;
  virtual void ShutdownOnUIThread() OVERRIDE;
};

}  

#endif  
