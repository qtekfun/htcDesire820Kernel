// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_AUTH_FILTER_H_
#define NET_HTTP_HTTP_AUTH_FILTER_H_
#pragma once

#include <list>
#include <string>

#include "net/http/http_auth.h"
#include "net/proxy/proxy_bypass_rules.h"

class GURL;

namespace net {

class HttpAuthFilter {
 public:
  virtual ~HttpAuthFilter() {}

  
  
  virtual bool IsValid(const GURL& url, HttpAuth::Target target) const = 0;
};

class HttpAuthFilterWhitelist : public HttpAuthFilter {
 public:
  explicit HttpAuthFilterWhitelist(const std::string& server_whitelist);
  virtual ~HttpAuthFilterWhitelist();

  
  bool AddFilter(const std::string& filter, HttpAuth::Target target);

  
  void AddRuleToBypassLocal();

  const ProxyBypassRules& rules() const { return rules_; }

  
  virtual bool IsValid(const GURL& url, HttpAuth::Target target) const;

 private:
  
  
  void SetWhitelist(const std::string& server_whitelist);

  
  
  ProxyBypassRules rules_;

  DISALLOW_COPY_AND_ASSIGN(HttpAuthFilterWhitelist);
};

}   

#endif  
