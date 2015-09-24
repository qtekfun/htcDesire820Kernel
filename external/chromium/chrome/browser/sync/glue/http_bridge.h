// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_HTTP_BRIDGE_H_
#define CHROME_BROWSER_SYNC_GLUE_HTTP_BRIDGE_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "chrome/browser/sync/engine/http_post_provider_factory.h"
#include "chrome/browser/sync/engine/http_post_provider_interface.h"
#include "chrome/browser/sync/engine/syncapi.h"
#include "chrome/common/net/url_fetcher.h"
#include "googleurl/src/gurl.h"
#include "net/url_request/url_request_context.h"
#include "net/url_request/url_request_context_getter.h"
#include "testing/gtest/include/gtest/gtest_prod.h"

class MessageLoop;
class HttpBridgeTest;

namespace browser_sync {

class HttpBridge : public base::RefCountedThreadSafe<HttpBridge>,
                   public sync_api::HttpPostProviderInterface,
                   public URLFetcher::Delegate {
 public:
  
  
  
  class RequestContext : public net::URLRequestContext {
   public:
    
    
    
    
    explicit RequestContext(net::URLRequestContext* baseline_context);

    
    
    void set_user_agent(const std::string& ua) { user_agent_ = ua; }

    virtual const std::string& GetUserAgent(const GURL& url) const {
      
      
      return user_agent_.empty() ?
          net::URLRequestContext::GetUserAgent(url) : user_agent_;
    }

   private:
    
    ~RequestContext();

    std::string user_agent_;
    net::URLRequestContext* baseline_context_;

    DISALLOW_COPY_AND_ASSIGN(RequestContext);
  };

  
  class RequestContextGetter : public net::URLRequestContextGetter {
   public:
    explicit RequestContextGetter(
        net::URLRequestContextGetter* baseline_context_getter);

    void set_user_agent(const std::string& ua) { user_agent_ = ua; }
    bool is_user_agent_set() const { return !user_agent_.empty(); }

    
    virtual net::URLRequestContext* GetURLRequestContext();
    virtual scoped_refptr<base::MessageLoopProxy> GetIOMessageLoopProxy() const;

   private:
    ~RequestContextGetter() {}

    
    std::string user_agent_;

    scoped_refptr<net::URLRequestContextGetter> baseline_context_getter_;

    
    scoped_refptr<RequestContext> context_;

    DISALLOW_COPY_AND_ASSIGN(RequestContextGetter);
  };

  explicit HttpBridge(RequestContextGetter* context);

  
  virtual void SetUserAgent(const char* user_agent);
  virtual void SetExtraRequestHeaders(const char* headers);
  virtual void SetURL(const char* url, int port);
  virtual void SetPostPayload(const char* content_type, int content_length,
                              const char* content);
  virtual bool MakeSynchronousPost(int* os_error_code, int* response_code);
  virtual void Abort();

  
  
  
  
  virtual int GetResponseContentLength() const;
  virtual const char* GetResponseContent() const;
  virtual const std::string GetResponseHeaderValue(
      const std::string& name) const;

  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

#if defined(UNIT_TEST)
  net::URLRequestContextGetter* GetRequestContextGetter() const {
    return context_getter_for_request_;
  }
#endif

 protected:
  friend class base::RefCountedThreadSafe<HttpBridge>;

  virtual ~HttpBridge();

  
  virtual void MakeAsynchronousPost();

 private:
  friend class ::HttpBridgeTest;

  
  
  
  void CallMakeAsynchronousPost() { MakeAsynchronousPost(); }

  
  
  scoped_refptr<RequestContextGetter> context_getter_for_request_;

  
  
  
  
  
  MessageLoop* const created_on_loop_;

  
  GURL url_for_request_;

  
  std::string content_type_;
  std::string request_content_;
  std::string extra_headers_;

  
  
  
  base::WaitableEvent http_post_completed_;

  struct URLFetchState {
    URLFetchState();
    ~URLFetchState();
    
    
    
    
    
    URLFetcher* url_poster;

    
    bool aborted;

    
    bool request_completed;
    bool request_succeeded;
    int http_response_code;
    int os_error_code;
    std::string response_content;
    scoped_refptr<net::HttpResponseHeaders> response_headers;
  };

  
  
  
  
  mutable base::Lock fetch_state_lock_;
  URLFetchState fetch_state_;

  DISALLOW_COPY_AND_ASSIGN(HttpBridge);
};

class HttpBridgeFactory : public sync_api::HttpPostProviderFactory {
 public:
  explicit HttpBridgeFactory(
      net::URLRequestContextGetter* baseline_context_getter);
  virtual ~HttpBridgeFactory();

  
  virtual sync_api::HttpPostProviderInterface* Create() OVERRIDE;
  virtual void Destroy(sync_api::HttpPostProviderInterface* http) OVERRIDE;

 private:
  
  
  HttpBridge::RequestContextGetter* GetRequestContextGetter();

  scoped_refptr<HttpBridge::RequestContextGetter> request_context_getter_;

  DISALLOW_COPY_AND_ASSIGN(HttpBridgeFactory);
};

}  

#endif  
