// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_CONFIG_H_
#define NET_PROXY_PROXY_CONFIG_H_

#include <string>

#include "net/base/net_export.h"
#include "net/proxy/proxy_bypass_rules.h"
#include "net/proxy/proxy_config_source.h"
#include "net/proxy/proxy_list.h"
#include "net/proxy/proxy_server.h"
#include "url/gurl.h"

namespace base {
class Value;
}

namespace net {

class ProxyInfo;

class NET_EXPORT ProxyConfig {
 public:
  
  
  
  
  struct NET_EXPORT ProxyRules {
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

    
    
    void Apply(const GURL& url, ProxyInfo* result) const;

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    void ParseFromString(const std::string& proxy_rules);

    
    
    
    const ProxyList* MapUrlSchemeToProxyList(
        const std::string& url_scheme) const;

    
    bool Equals(const ProxyRules& other) const;

    
    ProxyBypassRules bypass_rules;

    
    bool reverse_bypass;

    Type type;

    
    ProxyList single_proxies;

    
    ProxyList proxies_for_http;
    ProxyList proxies_for_https;
    ProxyList proxies_for_ftp;

    
    
    ProxyList fallback_proxies;

   private:
    
    
    
    ProxyList* MapUrlSchemeToProxyListNoFallback(const std::string& scheme);
  };

  typedef int ID;

  
  static const ID kInvalidConfigID = 0;

  ProxyConfig();
  ProxyConfig(const ProxyConfig& config);
  ~ProxyConfig();
  ProxyConfig& operator=(const ProxyConfig& config);

  
  ID id() const { return id_; }
  void set_id(ID id) { id_ = id; }
  bool is_valid() const { return id_ != kInvalidConfigID; }

  
  
  bool Equals(const ProxyConfig& other) const;

  
  
  bool HasAutomaticSettings() const;

  void ClearAutomaticSettings();

  
  
  base::DictionaryValue* ToValue() const;

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

  void set_pac_mandatory(bool enable_pac_mandatory) {
    pac_mandatory_ = enable_pac_mandatory;
  }

  bool pac_mandatory() const {
    return pac_mandatory_;
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

  void set_source(ProxyConfigSource source) {
    source_ = source;
  }

  ProxyConfigSource source() const {
    return source_;
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
    
    config.set_pac_mandatory(false);
    return config;
  }

 private:
  
  bool auto_detect_;

  
  GURL pac_url_;

  
  
  bool pac_mandatory_;

  
  ProxyRules proxy_rules_;

  
  ProxyConfigSource source_;

  ID id_;
};

}  



#endif  
