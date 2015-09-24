// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_HTTP_BRIDGE_H_
#define SYNC_INTERNAL_API_PUBLIC_HTTP_BRIDGE_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "net/base/network_time_notifier.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "net/url_request/url_request_context.h"
#include "net/url_request/url_request_context_getter.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/cancelation_observer.h"
#include "sync/internal_api/public/http_post_provider_factory.h"
#include "sync/internal_api/public/http_post_provider_interface.h"
#include "sync/internal_api/public/network_time_update_callback.h"
#include "url/gurl.h"

class HttpBridgeTest;

namespace base {
class MessageLoop;
}

namespace net {
class HttpResponseHeaders;
class HttpUserAgentSettings;
class URLFetcher;
}

namespace syncer {

class CancelationSignal;

class SYNC_EXPORT_PRIVATE HttpBridge
    : public base::RefCountedThreadSafe<HttpBridge>,
      public HttpPostProviderInterface,
      public net::URLFetcherDelegate {
 public:
  
  
  
  class RequestContext : public net::URLRequestContext {
   public:
    
    
    
    
    RequestContext(
        net::URLRequestContext* baseline_context,
        const scoped_refptr<base::SingleThreadTaskRunner>&
            network_task_runner,
        const std::string& user_agent);

    
    virtual ~RequestContext();

   private:
    net::URLRequestContext* const baseline_context_;
    const scoped_refptr<base::SingleThreadTaskRunner> network_task_runner_;
    scoped_ptr<net::HttpUserAgentSettings> http_user_agent_settings_;

    DISALLOW_COPY_AND_ASSIGN(RequestContext);
  };

  
  class SYNC_EXPORT_PRIVATE RequestContextGetter
      : public net::URLRequestContextGetter {
   public:
    RequestContextGetter(
        net::URLRequestContextGetter* baseline_context_getter,
        const std::string& user_agent);

    
    virtual net::URLRequestContext* GetURLRequestContext() OVERRIDE;
    virtual scoped_refptr<base::SingleThreadTaskRunner>
        GetNetworkTaskRunner() const OVERRIDE;

   protected:
    virtual ~RequestContextGetter();

   private:
    scoped_refptr<net::URLRequestContextGetter> baseline_context_getter_;
    const scoped_refptr<base::SingleThreadTaskRunner> network_task_runner_;
    
    const std::string user_agent_;

    
    scoped_ptr<RequestContext> context_;

    DISALLOW_COPY_AND_ASSIGN(RequestContextGetter);
  };

  HttpBridge(RequestContextGetter* context,
             const NetworkTimeUpdateCallback& network_time_update_callback);

  
  virtual void SetExtraRequestHeaders(const char* headers) OVERRIDE;
  virtual void SetURL(const char* url, int port) OVERRIDE;
  virtual void SetPostPayload(const char* content_type, int content_length,
                              const char* content) OVERRIDE;
  virtual bool MakeSynchronousPost(int* error_code,
                                   int* response_code) OVERRIDE;
  virtual void Abort() OVERRIDE;

  
  
  
  
  virtual int GetResponseContentLength() const OVERRIDE;
  virtual const char* GetResponseContent() const OVERRIDE;
  virtual const std::string GetResponseHeaderValue(
      const std::string& name) const OVERRIDE;

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  net::URLRequestContextGetter* GetRequestContextGetterForTest() const;

 protected:
  friend class base::RefCountedThreadSafe<HttpBridge>;

  virtual ~HttpBridge();

  
  virtual void MakeAsynchronousPost();

 private:
  friend class SyncHttpBridgeTest;
  friend class ::HttpBridgeTest;

  
  
  
  void CallMakeAsynchronousPost() { MakeAsynchronousPost(); }

  
  
  
  
  void DestroyURLFetcherOnIOThread(net::URLFetcher* fetcher);

  void UpdateNetworkTime();

  
  
  
  
  
  base::MessageLoop* const created_on_loop_;

  
  GURL url_for_request_;

  
  std::string content_type_;
  std::string request_content_;
  std::string extra_headers_;

  
  
  
  base::WaitableEvent http_post_completed_;

  struct URLFetchState {
    URLFetchState();
    ~URLFetchState();
    
    
    
    
    
    net::URLFetcher* url_poster;

    
    
    base::Time start_time;
    base::Time end_time;

    
    bool aborted;

    
    bool request_completed;
    bool request_succeeded;
    int http_response_code;
    int error_code;
    std::string response_content;
    scoped_refptr<net::HttpResponseHeaders> response_headers;
  };

  
  
  
  
  
  mutable base::Lock fetch_state_lock_;
  URLFetchState fetch_state_;

  
  
  scoped_refptr<RequestContextGetter> context_getter_for_request_;

  const scoped_refptr<base::SingleThreadTaskRunner> network_task_runner_;

  
  NetworkTimeUpdateCallback network_time_update_callback_;

  DISALLOW_COPY_AND_ASSIGN(HttpBridge);
};

class SYNC_EXPORT HttpBridgeFactory : public HttpPostProviderFactory,
                                      public CancelationObserver {
 public:
  HttpBridgeFactory(
      net::URLRequestContextGetter* baseline_context_getter,
      const NetworkTimeUpdateCallback& network_time_update_callback,
      CancelationSignal* cancelation_signal);
  virtual ~HttpBridgeFactory();

  
  virtual void Init(const std::string& user_agent) OVERRIDE;
  virtual HttpPostProviderInterface* Create() OVERRIDE;
  virtual void Destroy(HttpPostProviderInterface* http) OVERRIDE;

  
  virtual void OnSignalReceived() OVERRIDE;

 private:
  
  base::Lock context_getter_lock_;

  
  
  
  
  scoped_refptr<net::URLRequestContextGetter> baseline_request_context_getter_;

  
  
  
  
  
  scoped_refptr<HttpBridge::RequestContextGetter> request_context_getter_;

  NetworkTimeUpdateCallback network_time_update_callback_;

  CancelationSignal* const cancelation_signal_;

  DISALLOW_COPY_AND_ASSIGN(HttpBridgeFactory);
};

}  

#endif  
