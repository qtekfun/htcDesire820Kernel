// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREDICTORS_RESOURCE_PREFETCHER_H_
#define CHROME_BROWSER_PREDICTORS_RESOURCE_PREFETCHER_H_

#include <list>
#include <map>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/predictors/resource_prefetch_common.h"
#include "net/url_request/url_request.h"
#include "url/gurl.h"

namespace net {
class URLRequestContext;
}

namespace predictors {

class ResourcePrefetcher : public base::NonThreadSafe,
                           public net::URLRequest::Delegate {
 public:
  
  struct Request {
    explicit Request(const GURL& i_resource_url);
    Request(const Request& other);

    enum PrefetchStatus {
      PREFETCH_STATUS_NOT_STARTED,
      PREFETCH_STATUS_STARTED,

      
      PREFETCH_STATUS_REDIRECTED,
      PREFETCH_STATUS_AUTH_REQUIRED,
      PREFETCH_STATUS_CERT_REQUIRED,
      PREFETCH_STATUS_CERT_ERROR,
      PREFETCH_STATUS_CANCELLED,
      PREFETCH_STATUS_FAILED,

      
      PREFETCH_STATUS_FROM_CACHE,
      PREFETCH_STATUS_FROM_NETWORK
    };

    enum UsageStatus {
      USAGE_STATUS_NOT_REQUESTED,
      USAGE_STATUS_FROM_CACHE,
      USAGE_STATUS_FROM_NETWORK,
      USAGE_STATUS_NAVIGATION_ABANDONED
    };

    GURL resource_url;
    PrefetchStatus prefetch_status;
    UsageStatus usage_status;
  };
  typedef ScopedVector<Request> RequestVector;

  
  
  class Delegate {
   public:
    virtual ~Delegate() { }

    
    
    virtual void ResourcePrefetcherFinished(
        ResourcePrefetcher* prefetcher,
        RequestVector* requests) = 0;

    virtual net::URLRequestContext* GetURLRequestContext() = 0;
  };

  
  
  ResourcePrefetcher(Delegate* delegate,
                     const ResourcePrefetchPredictorConfig& config,
                     const NavigationID& navigation_id,
                     PrefetchKeyType key_type,
                     scoped_ptr<RequestVector> requests);
  virtual ~ResourcePrefetcher();

  void Start();  
  void Stop();   

  const NavigationID& navigation_id() const { return navigation_id_; }
  PrefetchKeyType key_type() const { return key_type_; }

 private:
  friend class ResourcePrefetcherTest;
  friend class TestResourcePrefetcher;

  
  void TryToLaunchPrefetchRequests();

  
  void SendRequest(Request* request);

  
  
  virtual void StartURLRequest(net::URLRequest* request);

  
  void FinishRequest(net::URLRequest* request, Request::PrefetchStatus status);

  
  
  virtual void ReadFullResponse(net::URLRequest* request);

  
  
  bool ShouldContinueReadingRequest(net::URLRequest* request, int bytes_read);

  
  virtual void OnReceivedRedirect(net::URLRequest* request,
                                  const GURL& new_url,
                                  bool* defer_redirect) OVERRIDE;
  virtual void OnAuthRequired(net::URLRequest* request,
                              net::AuthChallengeInfo* auth_info) OVERRIDE;
  virtual void OnCertificateRequested(
      net::URLRequest* request,
      net::SSLCertRequestInfo* cert_request_info) OVERRIDE;
  virtual void OnSSLCertificateError(net::URLRequest* request,
                                     const net::SSLInfo& ssl_info,
                                     bool fatal) OVERRIDE;
  virtual void OnResponseStarted(net::URLRequest* request) OVERRIDE;
  virtual void OnReadCompleted(net::URLRequest* request,
                               int bytes_read) OVERRIDE;

  enum PrefetcherState {
    INITIALIZED = 0,  
    RUNNING = 1,      
    STOPPED = 2,      
    FINISHED = 3      
  };

  PrefetcherState state_;
  Delegate* const delegate_;
  ResourcePrefetchPredictorConfig const config_;
  NavigationID navigation_id_;
  PrefetchKeyType key_type_;
  scoped_ptr<RequestVector> request_vector_;

  std::map<net::URLRequest*, Request*> inflight_requests_;
  std::list<Request*> request_queue_;
  std::map<std::string, int> host_inflight_counts_;

  DISALLOW_COPY_AND_ASSIGN(ResourcePrefetcher);
};

}  

#endif  
