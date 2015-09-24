// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_NET_AW_URL_REQUEST_CONTEXT_GETTER_H_
#define ANDROID_WEBVIEW_BROWSER_NET_AW_URL_REQUEST_CONTEXT_GETTER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/content_browser_client.h"
#include "net/http/http_network_session.h"
#include "net/url_request/url_request_context_getter.h"
#include "net/url_request/url_request_job_factory.h"

namespace net {
class CookieStore;
class HttpTransactionFactory;
class ProxyConfigService;
class URLRequestContext;
class URLRequestJobFactory;
}

namespace android_webview {

class AwNetworkDelegate;

class AwURLRequestContextGetter : public net::URLRequestContextGetter {
 public:
  AwURLRequestContextGetter(const base::FilePath& partition_path,
                            net::CookieStore* cookie_store);

  void InitializeOnNetworkThread();

  
  virtual net::URLRequestContext* GetURLRequestContext() OVERRIDE;
  virtual scoped_refptr<base::SingleThreadTaskRunner>
      GetNetworkTaskRunner() const OVERRIDE;

 private:
  friend class AwBrowserContext;
  virtual ~AwURLRequestContextGetter();

  
  
  
  
  
  
  void SetProtocolHandlers(content::ProtocolHandlerMap* protocol_handlers);

  void InitializeURLRequestContext();

  const base::FilePath partition_path_;
  scoped_refptr<net::CookieStore> cookie_store_;
  scoped_ptr<net::URLRequestContext> url_request_context_;
  scoped_ptr<net::ProxyConfigService> proxy_config_service_;
  scoped_ptr<net::URLRequestJobFactory> job_factory_;
  scoped_ptr<net::HttpTransactionFactory> main_http_factory_;

  
  
  content::ProtocolHandlerMap protocol_handlers_;

  DISALLOW_COPY_AND_ASSIGN(AwURLRequestContextGetter);
};

}  

#endif  
