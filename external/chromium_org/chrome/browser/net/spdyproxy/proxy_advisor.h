// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_SPDYPROXY_PROXY_ADVISOR_H_
#define CHROME_BROWSER_NET_SPDYPROXY_PROXY_ADVISOR_H_

#include <set>

#include "base/prefs/pref_member.h"
#include "chrome/browser/net/url_info.h"
#include "net/url_request/url_request.h"

namespace net {
class URLRequestContextGetter;
}

class PrefService;

class ProxyAdvisor : public net::URLRequest::Delegate {
 public:
  ProxyAdvisor(PrefService* pref_service,
               net::URLRequestContextGetter* context_getter);
  virtual ~ProxyAdvisor();

  
  virtual void OnResponseStarted(net::URLRequest* request) OVERRIDE;
  virtual void OnReadCompleted(net::URLRequest* request,
                               int bytes_read) OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  
  virtual void Advise(const GURL& url,
              chrome_browser_net::UrlInfo::ResolutionMotivation motivation,
              bool is_preconnect);

  
  
  
  
  virtual bool WouldProxyURL(const GURL& url);

 private:
  
  void RequestComplete(net::URLRequest* request);

  
  
  void UpdateProxyState();

  scoped_refptr<net::URLRequestContextGetter> context_getter_;

  BooleanPrefMember proxy_pref_member_;

  std::set<net::URLRequest*> inflight_requests_;
};

#endif  

