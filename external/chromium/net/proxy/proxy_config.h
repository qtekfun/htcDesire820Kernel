// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_CONFIG_H_
#define NET_PROXY_PROXY_CONFIG_H_
#pragma once

#include <string>

#include "googleurl/src/gurl.h"
#include "net/base/net_export.h"
#include "net/proxy/proxy_bypass_rules.h"
#include "net/proxy/proxy_server.h"

class Value;

namespace net {

class ProxyInfo;

class NET_EXPORT ProxyConfig {
 public:
  
  
  struct ProxyRules {
    enum Type {
      TYPE_NO_RULES,
      TYPE_SINGLE_PROXY,
      TYPE_PROXY_PER_SCHEME,
    };

    
    
    ProxyRules();
    ~ProxyRules();

    bool empty() const {
      return type == TYPE_NO_RULES;
    }

    
    void Apply(const GURL& url, ProxyInfo* result);

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    void ParseFromString(const std::string& proxy_rules);

    
    
    
    const ProxyServer* MapUrlSchemeToProxy(const std::string& url_scheme) const;

    
    bool Equals(const ProxyRules& other) const;

    
    ProxyBypassRules bypass_rules;

    
    bool reverse_bypass;

    Type type;

    
    ProxyServer single_proxy;

    
    ProxyServer proxy_for_http;
    ProxyServer proxy_for_https;
    ProxyServer proxy_for_ftp;

    
    ProxyServer fallback_proxy;

   private:
    
    
    
    ProxyServer* MapUrlSchemeToProxyNoFallback(const std::string& scheme);
  };

  typedef int ID;

  
  enum { INVALID_ID = 0 };

  ProxyConfig();
  ProxyConfig(const ProxyConfig& config);
  ~ProxyConfig();
  ProxyConfig& operator=(const ProxyConfig& config);

  
  ID id() const { return id_; }
  void set_id(int id) { id_ = id; }
  bool is_valid() const { return id_ != INVALID_ID; }

  
  bool Equals(const ProxyConfig& other) const;

  
  
  bool HasAutomaticSettings() const;

  void ClearAutomaticSettings();

  
  
  Value* ToValue() const;

  ProxyRules& proxy_rules() {
    return proxy_rules_;
  }

  const ProxyRules& proxy_rules() const {
    return proxy_rules_;
  }

  void set_pac_url(const GURL& url) {
    pac_url_ = url;
  }

  const GURL& pac_url() const {
    return pac_url_;
  }

  bool has_pac_url() const {
    return pac_url_.is_valid();
  }

  void set_auto_detect(bool enable_auto_detect) {
    auto_detect_ = enable_auto_detect;
  }

  bool auto_detect() const {
    return auto_detect_;
  }

  

  static ProxyConfig CreateDirect() {
    return ProxyConfig();
  }

  static ProxyConfig CreateAutoDetect() {
    ProxyConfig config;
    config.set_auto_detect(true);
    return config;
  }

  static ProxyConfig CreateFromCustomPacURL(const GURL& pac_url) {
    ProxyConfig config;
    config.set_pac_url(pac_url);
    return config;
  }

 private:
  
  bool auto_detect_;

  
  GURL pac_url_;

  
  ProxyRules proxy_rules_;

  int id_;
};

}  



#endif  
