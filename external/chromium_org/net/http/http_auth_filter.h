// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_AUTH_FILTER_H_
#define NET_HTTP_HTTP_AUTH_FILTER_H_

#include <list>
#include <string>

#include "net/base/net_export.h"
#include "net/http/http_auth.h"
#include "net/proxy/proxy_bypass_rules.h"

class GURL;

namespace net {

class NET_EXPORT_PRIVATE HttpAuthFilter {
 public:
  virtual ~HttpAuthFilter() {}

  
  
  virtual bool IsValid(const GURL& url, HttpAuth::Target target) const = 0;
};

class NET_EXPORT HttpAuthFilterWhitelist : public HttpAuthFilter {
 public:
  explicit HttpAuthFilterWhitelist(const std::string& server_whitelist);
  virtual ~HttpAuthFilterWhitelist();

  
  bool AddFilter(const std::string& filter, HttpAuth::Target target);

  
  void AddRuleToBypassLocal();

  const ProxyBypassRules& rules() const { return rules_; }

  
  virtual bool IsValid(const GURL& url, HttpAuth::Target target) const OVERRIDE;

 private:
  
  
  void SetWhitelist(const std::string& server_whitelist);

  
  
  ProxyBypassRules rules_;

  DISALLOW_COPY_AND_ASSIGN(HttpAuthFilterWhitelist);
};

}   

#endif  
