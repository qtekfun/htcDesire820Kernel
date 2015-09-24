// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_NET_PREDICTOR_H_
#define CHROME_BROWSER_NET_PREDICTOR_H_
#pragma once

#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/net/url_info.h"
#include "chrome/browser/net/referrer.h"
#include "chrome/common/net/predictor_common.h"
#include "net/base/host_port_pair.h"

class ListValue;

namespace net {
class HostResolver;
}  

namespace chrome_browser_net {

typedef chrome_common_net::UrlList UrlList;
typedef chrome_common_net::NameList NameList;
typedef std::map<GURL, UrlInfo> Results;

class Predictor : public base::RefCountedThreadSafe<Predictor> {
 public:
  
  
  enum { PREDICTOR_REFERRER_VERSION = 2 };

  
  
  Predictor(net::HostResolver* host_resolver,
            base::TimeDelta max_queue_delay_ms, size_t max_concurrent,
            bool preconnect_enabled);

  
  void Shutdown();

  
  
  
  
  
  void DiscardAllResults();

  
  void ResolveList(const UrlList& urls,
                   UrlInfo::ResolutionMotivation motivation);
  void Resolve(const GURL& url,
               UrlInfo::ResolutionMotivation motivation);

  
  
  
  
  
  void PredictFrameSubresources(const GURL& url);

  
  
  
  void AnticipateOmniboxUrl(const GURL& url, bool preconnectable);

  
  void PreconnectUrlAndSubresources(const GURL& url);

  
  
  
  
  void LearnFromNavigation(const GURL& referring_url, const GURL& target_url);

  
  void GetHtmlReferrerLists(std::string* output);

  
  
  void GetHtmlInfo(std::string* output);

  
  
  
  
  
  
  void TrimReferrersNow();

  
  
  void SerializeReferrers(ListValue* referral_list);

  
  
  
  void DeserializeReferrers(const ListValue& referral_list);

  void DeserializeReferrersThenDelete(ListValue* referral_list);

  
  size_t max_concurrent_dns_lookups() const {
    return max_concurrent_dns_lookups_;
  }

  
  bool preconnect_enabled() const { return preconnect_enabled_; }

  
  
  
  static GURL CanonicalizeUrl(const GURL& url);

 private:
  friend class base::RefCountedThreadSafe<Predictor>;
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, BenefitLookupTest);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, ShutdownWhenResolutionIsPendingTest);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, SingleLookupTest);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, ConcurrentLookupTest);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, MassiveConcurrentLookupTest);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, PriorityQueuePushPopTest);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, PriorityQueueReorderTest);
  FRIEND_TEST_ALL_PREFIXES(PredictorTest, ReferrerSerializationTrimTest);
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

  
  
  
  typedef std::map<GURL, Referrer> Referrers;

  
  
  
  static const double kPreconnectWorthyExpectedValue;
  static const double kDNSPreresolutionWorthyExpectedValue;
  
  
  static const double kDiscardableExpectedValue;
  
  
  
  
  static const double kReferrerTrimRatio;

  
  
  
  static const base::TimeDelta kDurationBetweenTrimmings;
  
  static const base::TimeDelta kDurationBetweenTrimmingIncrements;
  
  static const size_t kUrlsTrimmedPerIncrement;

  ~Predictor();

  
  
  
  void PrepareFrameSubresources(const GURL& url);

  
  
  bool WasFound(const GURL& url) const {
    Results::const_iterator it(results_.find(url));
    return (it != results_.end()) &&
            it->second.was_found();
  }

  
  
  base::TimeDelta GetResolutionDuration(const GURL& url) {
    if (results_.find(url) == results_.end())
      return UrlInfo::kNullDuration;
    return results_[url].resolve_duration();
  }

  
  size_t peak_pending_lookups() const { return peak_pending_lookups_; }

  
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

  
  HostNameQueue work_queue_;

  
  Results results_;

  std::set<LookupRequest*> pending_lookups_;

  
  size_t peak_pending_lookups_;

  
  bool shutdown_;

  
  
  
  
  const size_t max_concurrent_dns_lookups_;

  
  
  const base::TimeDelta max_dns_queue_delay_;

  
  net::HostResolver* const host_resolver_;

  
  
  bool preconnect_enabled_;

  
  std::string last_omnibox_host_;

  
  base::TimeTicks last_omnibox_preresolve_;

  
  
  int consecutive_omnibox_preconnect_count_;

  
  base::TimeTicks last_omnibox_preconnect_;

  
  
  
  
  Referrers referrers_;

  
  
  std::vector<GURL> urls_being_trimmed_;

  
  base::TimeTicks next_trim_time_;

  ScopedRunnableMethodFactory<Predictor> trim_task_factory_;

  DISALLOW_COPY_AND_ASSIGN(Predictor);
};

}  

#endif  
