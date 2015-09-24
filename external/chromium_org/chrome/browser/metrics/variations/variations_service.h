// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_METRICS_VARIATIONS_VARIATIONS_SERVICE_H_
#define CHROME_BROWSER_METRICS_VARIATIONS_VARIATIONS_SERVICE_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/metrics/field_trial.h"
#include "base/time/time.h"
#include "chrome/browser/metrics/variations/variations_request_scheduler.h"
#include "chrome/browser/web_resource/resource_request_allowed_notifier.h"
#include "chrome/common/chrome_version_info.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "url/gurl.h"

#if defined(OS_WIN)
#include "chrome/browser/metrics/variations/variations_registry_syncer_win.h"
#endif

class PrefService;
class PrefRegistrySimple;

namespace chrome_variations {

class VariationsSeed;

class VariationsService
    : public net::URLFetcherDelegate,
      public ResourceRequestAllowedNotifier::Observer {
 public:
  virtual ~VariationsService();

  
  
  
  bool CreateTrialsFromSeed();

  
  
  
  void StartRepeatedVariationsSeedFetch();

  
  
  
  static GURL GetVariationsServerURL(PrefService* local_prefs);

#if defined(OS_WIN)
  
  void StartGoogleUpdateRegistrySync();
#endif

  
  void SetCreateTrialsFromSeedCalledForTesting(bool called);

  
  static std::string GetDefaultVariationsServerURLForTesting();

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  static VariationsService* Create(PrefService* local_state);

 protected:
  
  
  virtual void DoActualFetch();

  
  
  VariationsService(ResourceRequestAllowedNotifier* notifier,
                    PrefService* local_state);

 private:
  FRIEND_TEST_ALL_PREFIXES(VariationsServiceTest, DoNotFetchIfOffline);
  FRIEND_TEST_ALL_PREFIXES(VariationsServiceTest, DoNotFetchIfOnlineToOnline);
  FRIEND_TEST_ALL_PREFIXES(VariationsServiceTest, FetchOnReconnect);
  FRIEND_TEST_ALL_PREFIXES(VariationsServiceTest, LoadSeed);
  FRIEND_TEST_ALL_PREFIXES(VariationsServiceTest, StoreSeed);
  FRIEND_TEST_ALL_PREFIXES(VariationsServiceTest, SeedStoredWhenOKStatus);
  FRIEND_TEST_ALL_PREFIXES(VariationsServiceTest, SeedNotStoredWhenNonOKStatus);
  FRIEND_TEST_ALL_PREFIXES(VariationsServiceTest, SeedDateUpdatedOn304Status);

  
  
  explicit VariationsService(PrefService* local_state);

  
  
  void FetchVariationsSeed();

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  virtual void OnResourceRequestsAllowed() OVERRIDE;

  
  
  
  
  bool StoreSeedData(const std::string& seed_data, const base::Time& seed_date);

  
  
  
  bool LoadVariationsSeedFromPref(VariationsSeed* seed);

  
  void RecordLastFetchTime();

  
  PrefService* local_state_;

  
  
  
  scoped_ptr<VariationsRequestScheduler> request_scheduler_;

  
  
  scoped_ptr<net::URLFetcher> pending_seed_request_;

  
  GURL variations_server_url_;

  
  std::string variations_serial_number_;

  
  
  bool create_trials_from_seed_called_;

  
  bool initial_request_completed_;

  
  
  scoped_ptr<ResourceRequestAllowedNotifier> resource_request_allowed_notifier_;

  
  
  base::TimeTicks last_request_started_time_;

#if defined(OS_WIN)
  
  VariationsRegistrySyncer registry_syncer_;
#endif

  DISALLOW_COPY_AND_ASSIGN(VariationsService);
};

}  

#endif  
