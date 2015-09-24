// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREDICTORS_RESOURCE_PREFETCH_PREDICTOR_H_
#define CHROME_BROWSER_PREDICTORS_RESOURCE_PREFETCH_PREDICTOR_H_

#include <map>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/predictors/resource_prefetch_common.h"
#include "chrome/browser/predictors/resource_prefetch_predictor_tables.h"
#include "chrome/browser/predictors/resource_prefetcher.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "url/gurl.h"
#include "webkit/common/resource_type.h"

class PredictorsHandler;
class Profile;

namespace content {
class WebContents;
}

namespace net {
class URLRequest;
}

namespace predictors {

class ResourcePrefetcherManager;

class ResourcePrefetchPredictor
    : public BrowserContextKeyedService,
      public content::NotificationObserver,
      public base::SupportsWeakPtr<ResourcePrefetchPredictor> {
 public:
  
  struct URLRequestSummary {
    URLRequestSummary();
    URLRequestSummary(const URLRequestSummary& other);
    ~URLRequestSummary();

    NavigationID navigation_id;
    GURL resource_url;
    ResourceType::Type resource_type;

    
    std::string mime_type;
    bool was_cached;
    GURL redirect_url;  
  };

  ResourcePrefetchPredictor(const ResourcePrefetchPredictorConfig& config,
                            Profile* profile);
  virtual ~ResourcePrefetchPredictor();

  
  static bool ShouldRecordRequest(net::URLRequest* request,
                                  ResourceType::Type resource_type);
  static bool ShouldRecordResponse(net::URLRequest* response);
  static bool ShouldRecordRedirect(net::URLRequest* response);

  
  
  static ResourceType::Type GetResourceTypeFromMimeType(
      const std::string& mime_type,
      ResourceType::Type fallback);

  
  
  
  void RecordURLRequest(const URLRequestSummary& request);
  void RecordURLResponse(const URLRequestSummary& response);
  void RecordURLRedirect(const URLRequestSummary& response);

  
  void RecordMainFrameLoadComplete(const NavigationID& navigation_id);

  
  
  virtual void FinishedPrefetchForNavigation(
      const NavigationID& navigation_id,
      PrefetchKeyType key_type,
      ResourcePrefetcher::RequestVector* requests);

 private:
  friend class ::PredictorsHandler;
  friend class ResourcePrefetchPredictorTest;

  FRIEND_TEST_ALL_PREFIXES(ResourcePrefetchPredictorTest, DeleteUrls);
  FRIEND_TEST_ALL_PREFIXES(ResourcePrefetchPredictorTest,
                           LazilyInitializeEmpty);
  FRIEND_TEST_ALL_PREFIXES(ResourcePrefetchPredictorTest,
                           LazilyInitializeWithData);
  FRIEND_TEST_ALL_PREFIXES(ResourcePrefetchPredictorTest,
                           NavigationNotRecorded);
  FRIEND_TEST_ALL_PREFIXES(ResourcePrefetchPredictorTest, NavigationUrlInDB);
  FRIEND_TEST_ALL_PREFIXES(ResourcePrefetchPredictorTest, NavigationUrlNotInDB);
  FRIEND_TEST_ALL_PREFIXES(ResourcePrefetchPredictorTest,
                           NavigationUrlNotInDBAndDBFull);
  FRIEND_TEST_ALL_PREFIXES(ResourcePrefetchPredictorTest, OnMainFrameRequest);
  FRIEND_TEST_ALL_PREFIXES(ResourcePrefetchPredictorTest, OnMainFrameRedirect);
  FRIEND_TEST_ALL_PREFIXES(ResourcePrefetchPredictorTest,
                           OnSubresourceResponse);

  enum InitializationState {
    NOT_INITIALIZED = 0,
    INITIALIZING = 1,
    INITIALIZED = 2
  };

  
  struct Result {
    
    Result(PrefetchKeyType key_type,
           ResourcePrefetcher::RequestVector* requests);
    ~Result();

    PrefetchKeyType key_type;
    scoped_ptr<ResourcePrefetcher::RequestVector> requests;

   private:
    DISALLOW_COPY_AND_ASSIGN(Result);
  };

  typedef ResourcePrefetchPredictorTables::ResourceRow ResourceRow;
  typedef ResourcePrefetchPredictorTables::ResourceRows ResourceRows;
  typedef ResourcePrefetchPredictorTables::PrefetchData PrefetchData;
  typedef ResourcePrefetchPredictorTables::PrefetchDataMap PrefetchDataMap;
  typedef std::map<NavigationID, linked_ptr<std::vector<URLRequestSummary> > >
      NavigationMap;
  typedef std::map<NavigationID, Result*> ResultsMap;

  
  static bool IsHandledMainPage(net::URLRequest* request);

  
  static bool IsHandledSubresource(net::URLRequest* request);

  
  static bool IsCacheable(const net::URLRequest* request);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void Shutdown() OVERRIDE;

  
  
  void OnMainFrameRequest(const URLRequestSummary& request);
  void OnMainFrameResponse(const URLRequestSummary& response);
  void OnMainFrameRedirect(const URLRequestSummary& response);
  void OnSubresourceResponse(const URLRequestSummary& response);

  
  
  
  void OnNavigationComplete(const NavigationID& navigation_id);

  
  
  
  bool GetPrefetchData(const NavigationID& navigation_id,
                       ResourcePrefetcher::RequestVector* prefetch_requests,
                       PrefetchKeyType* key_type);

  
  void PopulatePrefetcherRequest(const PrefetchData& data,
                                 ResourcePrefetcher::RequestVector* requests);

  
  
  void StartPrefetching(const NavigationID& navigation_id);

  
  void StopPrefetching(const NavigationID& navigation_id);

  
  
  void StartInitialization();

  
  
  void CreateCaches(scoped_ptr<PrefetchDataMap> url_data_map,
                    scoped_ptr<PrefetchDataMap> host_data_map);

  
  
  void OnHistoryAndCacheLoaded();

  
  
  void CleanupAbandonedNavigations(const NavigationID& navigation_id);

  
  
  void DeleteAllUrls();

  
  
  void DeleteUrls(const history::URLRows& urls);

  
  void OnVisitCountLookup(int visit_count,
                          const NavigationID& navigation_id,
                          const std::vector<URLRequestSummary>& requests);

  
  
  void RemoveOldestEntryInPrefetchDataMap(PrefetchKeyType key_type,
                                          PrefetchDataMap* data_map);

  
  
  void LearnNavigation(const std::string& key,
                       PrefetchKeyType key_type,
                       const std::vector<URLRequestSummary>& new_resources,
                       int max_data_map_size,
                       PrefetchDataMap* data_map);

  
  
  void ReportAccuracyStats(PrefetchKeyType key_type,
                           const std::vector<URLRequestSummary>& actual,
                           ResourcePrefetcher::RequestVector* prefetched) const;

  
  
  void ReportPredictedAccuracyStats(
      PrefetchKeyType key_type,
      const std::vector<URLRequestSummary>& actual,
      const ResourcePrefetcher::RequestVector& predicted) const;
  void ReportPredictedAccuracyStatsHelper(
      PrefetchKeyType key_type,
      const ResourcePrefetcher::RequestVector& predicted,
      const std::map<GURL, bool>& actual,
      int total_resources_fetched_from_network,
      int max_assumed_prefetched) const;

  
  void set_mock_tables(scoped_refptr<ResourcePrefetchPredictorTables> tables) {
    tables_ = tables;
  }

  Profile* const profile_;
  ResourcePrefetchPredictorConfig const config_;
  InitializationState initialization_state_;
  scoped_refptr<ResourcePrefetchPredictorTables> tables_;
  scoped_refptr<ResourcePrefetcherManager> prefetch_manager_;
  content::NotificationRegistrar notification_registrar_;
  CancelableRequestConsumer history_lookup_consumer_;

  
  NavigationMap inflight_navigations_;

  
  scoped_ptr<PrefetchDataMap> url_table_cache_;
  scoped_ptr<PrefetchDataMap> host_table_cache_;

  ResultsMap results_map_;
  STLValueDeleter<ResultsMap> results_map_deleter_;

  DISALLOW_COPY_AND_ASSIGN(ResourcePrefetchPredictor);
};

}  

#endif  
