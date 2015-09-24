// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_CLIENT_SIDE_DETECTION_SERVICE_H_
#define CHROME_BROWSER_SAFE_BROWSING_CLIENT_SIDE_DETECTION_SERVICE_H_

#include <map>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "net/base/net_util.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "url/gurl.h"

class SafeBrowsingService;

namespace base {
class TimeDelta;
}

namespace content {
class RenderProcessHost;
}

namespace net {
class URLFetcher;
class URLRequestContextGetter;
class URLRequestStatus;
typedef std::vector<std::string> ResponseCookies;
}  

namespace safe_browsing {
class ClientMalwareRequest;
class ClientPhishingRequest;
class ClientPhishingResponse;
class ClientSideModel;

class ClientSideDetectionService : public net::URLFetcherDelegate,
                                   public content::NotificationObserver {
 public:
  
  typedef base::Callback<void(GURL, bool)> ClientReportPhishingRequestCallback;
  
  typedef base::Callback<void(GURL, GURL, bool)>
      ClientReportMalwareRequestCallback;

  virtual ~ClientSideDetectionService();

  
  
  
  static ClientSideDetectionService* Create(
      net::URLRequestContextGetter* request_context_getter);

  
  
  
  
  
  
  
  void SetEnabledAndRefreshState(bool enabled);

  bool enabled() const {
    return enabled_;
  }

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  
  
  
  
  
  
  virtual void SendClientReportPhishingRequest(
      ClientPhishingRequest* verdict,
      const ClientReportPhishingRequestCallback& callback);

  
  virtual void SendClientReportMalwareRequest(
      ClientMalwareRequest* verdict,
      const ClientReportMalwareRequestCallback& callback);

  
  
  
  
  
  
  
  
  virtual bool IsPrivateIPAddress(const std::string& ip_address) const;

  
  virtual bool GetValidCachedResult(const GURL& url, bool* is_phishing);

  
  virtual bool IsInCache(const GURL& url);

  
  
  virtual bool OverPhishingReportLimit();

  
  
  virtual bool OverMalwareReportLimit();

 protected:
  
  explicit ClientSideDetectionService(
      net::URLRequestContextGetter* request_context_getter);

  
  enum ClientModelStatus {
    MODEL_SUCCESS,
    MODEL_NOT_CHANGED,
    MODEL_FETCH_FAILED,
    MODEL_EMPTY,
    MODEL_TOO_LARGE,
    MODEL_PARSE_ERROR,
    MODEL_MISSING_FIELDS,
    MODEL_INVALID_VERSION_NUMBER,
    MODEL_BAD_HASH_IDS,
    MODEL_STATUS_MAX  
  };

  
  
  
  void StartFetchModel();

  
  virtual void ScheduleFetchModel(int64 delay_ms);  

  
  
  
  virtual void EndFetchModel(ClientModelStatus status);  

 private:
  friend class ClientSideDetectionServiceTest;
  FRIEND_TEST_ALL_PREFIXES(ClientSideDetectionServiceTest, FetchModelTest);
  FRIEND_TEST_ALL_PREFIXES(ClientSideDetectionServiceTest, SetBadSubnets);
  FRIEND_TEST_ALL_PREFIXES(ClientSideDetectionServiceTest,
                           SetEnabledAndRefreshState);
  FRIEND_TEST_ALL_PREFIXES(ClientSideDetectionServiceTest, IsBadIpAddress);
  FRIEND_TEST_ALL_PREFIXES(ClientSideDetectionServiceTest,
                           ModelHasValidHashIds);

  
  
  struct CacheState {
    bool is_phishing;
    base::Time timestamp;

    CacheState(bool phish, base::Time time);
  };
  typedef std::map<GURL, linked_ptr<CacheState> > PhishingCache;

  
  
  typedef std::pair<net::IPAddressNumber, size_t> AddressRange;

  
  
  typedef std::map<std::string ,
                   std::set<std::string > > BadSubnetMap;

  static const char kClientReportMalwareUrl[];
  static const char kClientReportPhishingUrl[];
  static const char kClientModelUrl[];
  static const size_t kMaxModelSizeBytes;
  static const int kMaxReportsPerInterval;
  static const int kClientModelFetchIntervalMs;
  static const int kInitialClientModelFetchDelayMs;
  static const int kReportsIntervalDays;
  static const int kNegativeCacheIntervalDays;
  static const int kPositiveCacheIntervalMinutes;

  
  
  void StartClientReportPhishingRequest(
      ClientPhishingRequest* verdict,
      const ClientReportPhishingRequestCallback& callback);

  void StartClientReportMalwareRequest(
      ClientMalwareRequest* verdict,
      const ClientReportMalwareRequestCallback& callback);

  
  
  void HandleModelResponse(const net::URLFetcher* source,
                           const GURL& url,
                           const net::URLRequestStatus& status,
                           int response_code,
                           const net::ResponseCookies& cookies,
                           const std::string& data);

  
  
  void HandlePhishingVerdict(const net::URLFetcher* source,
                             const GURL& url,
                             const net::URLRequestStatus& status,
                             int response_code,
                             const net::ResponseCookies& cookies,
                             const std::string& data);

  
  
  void HandleMalwareVerdict(const net::URLFetcher* source,
                            const GURL& url,
                            const net::URLRequestStatus& status,
                            int response_code,
                            const net::ResponseCookies& cookies,
                            const std::string& data);

  
  void UpdateCache();

  
  int GetMalwareNumReports();

  
  int GetPhishingNumReports();

  
  
  int GetNumReports(std::queue<base::Time>* report_times);

  
  
  bool InitializePrivateNetworks();

  
  void SendModelToProcess(content::RenderProcessHost* process);

  
  void SendModelToRenderers();

  
  
  
  static void SetBadSubnets(const ClientSideModel& model,
                            BadSubnetMap* bad_subnets);


  
  
  static bool ModelHasValidHashIds(const ClientSideModel& model);

  
  static GURL GetClientReportUrl(const std::string& report_url);

  
  
  bool enabled_;

  std::string model_str_;
  scoped_ptr<ClientSideModel> model_;
  scoped_ptr<base::TimeDelta> model_max_age_;
  scoped_ptr<net::URLFetcher> model_fetcher_;

  
  
  struct ClientReportInfo;
  std::map<const net::URLFetcher*, ClientReportInfo*>
      client_phishing_reports_;
  
  
  struct ClientMalwareReportInfo;
  std::map<const net::URLFetcher*, ClientMalwareReportInfo*>
      client_malware_reports_;

  
  
  
  
  
  
  PhishingCache cache_;

  
  
  
  std::queue<base::Time> phishing_report_times_;

  
  
  std::queue<base::Time> malware_report_times_;

  
  
  base::WeakPtrFactory<ClientSideDetectionService> weak_factory_;

  
  scoped_refptr<net::URLRequestContextGetter> request_context_getter_;

  
  std::vector<AddressRange> private_networks_;

  
  
  BadSubnetMap bad_subnets_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ClientSideDetectionService);
};
}  

#endif  
