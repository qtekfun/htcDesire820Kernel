// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_CLIENT_SIDE_DETECTION_SERVICE_H_
#define CHROME_BROWSER_SAFE_BROWSING_CLIENT_SIDE_DETECTION_SERVICE_H_
#pragma once

#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_callback_factory.h"
#include "base/memory/scoped_ptr.h"
#include "base/platform_file.h"
#include "base/task.h"
#include "base/time.h"
#include "chrome/common/net/url_fetcher.h"
#include "googleurl/src/gurl.h"
#include "net/base/net_util.h"

namespace net {
class URLRequestContextGetter;
class URLRequestStatus;
}  

namespace safe_browsing {
class ClientPhishingRequest;

class ClientSideDetectionService : public URLFetcher::Delegate {
 public:
  typedef Callback1<base::PlatformFile>::Type OpenModelDoneCallback;

  typedef Callback2<GURL , bool >::Type
      ClientReportPhishingRequestCallback;

  virtual ~ClientSideDetectionService();

  
  
  
  static ClientSideDetectionService* Create(
      const FilePath& model_path,
      net::URLRequestContextGetter* request_context_getter);

  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

  
  
  
  
  
  
  void GetModelFile(OpenModelDoneCallback* callback);

  
  
  
  
  
  
  
  
  virtual void SendClientReportPhishingRequest(
      ClientPhishingRequest* verdict,
      ClientReportPhishingRequestCallback* callback);

  
  
  
  
  
  
  
  
  virtual bool IsPrivateIPAddress(const std::string& ip_address) const;

  
  virtual bool GetValidCachedResult(const GURL& url, bool* is_phishing);

  
  virtual bool IsInCache(const GURL& url);

  
  
  virtual bool OverReportLimit();

 protected:
  
  ClientSideDetectionService(
      const FilePath& model_path,
      net::URLRequestContextGetter* request_context_getter);

 private:
  friend class ClientSideDetectionServiceTest;

  enum ModelStatus {
    
    UNKNOWN_STATUS,
    
    READY_STATUS,
    
    ERROR_STATUS,
  };

  
  
  struct CacheState {
    bool is_phishing;
    base::Time timestamp;

    CacheState(bool phish, base::Time time);
  };
  typedef std::map<GURL, linked_ptr<CacheState> > PhishingCache;

  
  
  typedef std::pair<net::IPAddressNumber, size_t> AddressRange;

  static const char kClientReportPhishingUrl[];
  static const char kClientModelUrl[];
  static const int kMaxReportsPerInterval;
  static const base::TimeDelta kReportsInterval;
  static const base::TimeDelta kNegativeCacheInterval;
  static const base::TimeDelta kPositiveCacheInterval;

  
  
  void SetModelStatus(ModelStatus status);

  
  
  
  
  void OpenModelFileDone(base::PlatformFileError error_code,
                         base::PassPlatformFile file,
                         bool created);

  
  
  
  
  void CreateModelFileDone(base::PlatformFileError error_code,
                           base::PassPlatformFile file,
                           bool created);

  
  
  
  
  void WriteModelFileDone(base::PlatformFileError error_code,
                          int bytes_written);

  
  void CloseModelFile();

  
  
  void StartClientReportPhishingRequest(
      ClientPhishingRequest* verdict,
      ClientReportPhishingRequestCallback* callback);

  
  void StartGetModelFile(OpenModelDoneCallback* callback);

  
  
  void HandleModelResponse(const URLFetcher* source,
                           const GURL& url,
                           const net::URLRequestStatus& status,
                           int response_code,
                           const ResponseCookies& cookies,
                           const std::string& data);

  
  
  void HandlePhishingVerdict(const URLFetcher* source,
                             const GURL& url,
                             const net::URLRequestStatus& status,
                             int response_code,
                             const ResponseCookies& cookies,
                             const std::string& data);

  
  void UpdateCache();

  
  int GetNumReports();

  
  
  bool InitializePrivateNetworks();

  FilePath model_path_;
  ModelStatus model_status_;
  base::PlatformFile model_file_;
  scoped_ptr<URLFetcher> model_fetcher_;
  scoped_ptr<std::string> tmp_model_string_;
  std::vector<OpenModelDoneCallback*> open_callbacks_;

  
  
  struct ClientReportInfo;
  std::map<const URLFetcher*, ClientReportInfo*> client_phishing_reports_;

  
  
  
  
  
  
  PhishingCache cache_;

  
  
  
  std::queue<base::Time> phishing_report_times_;

  
  
  ScopedRunnableMethodFactory<ClientSideDetectionService> method_factory_;

  
  
  
  
  base::ScopedCallbackFactory<ClientSideDetectionService> callback_factory_;

  
  scoped_refptr<net::URLRequestContextGetter> request_context_getter_;

  
  std::vector<AddressRange> private_networks_;

  DISALLOW_COPY_AND_ASSIGN(ClientSideDetectionService);
};

}  

#endif  
