// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_INFO_H_
#define NET_PROXY_PROXY_INFO_H_
#pragma once

#include <string>

#include "net/proxy/proxy_config.h"
#include "net/proxy/proxy_list.h"
#include "net/proxy/proxy_retry_info.h"
#include "net/proxy/proxy_server.h"

namespace net {

class ProxyInfo {
 public:
  ProxyInfo();
  ~ProxyInfo();
  

  
  void Use(const ProxyInfo& proxy_info);

  
  void UseDirect();

  
  
  
  
  void UseNamedProxy(const std::string& proxy_uri_list);

  
  void UseProxyServer(const ProxyServer& proxy_server);

  
  void UsePacString(const std::string& pac_string) {
    proxy_list_.SetFromPacString(pac_string);
  }

  
  bool is_direct() const {
    
    if (is_empty())
      return false;
    return proxy_list_.Get().is_direct();
  }

  
  bool is_https() const {
    if (is_empty())
      return false;
    return proxy_server().is_https();
  }

  
  bool is_http() const {
    if (is_empty())
      return false;
    return proxy_server().is_http();
  }

  
  bool is_socks() const {
    if (is_empty())
      return false;
    return proxy_server().is_socks();
  }

  
  bool is_empty() const {
    return proxy_list_.IsEmpty();
  }

  
  
  const ProxyServer& proxy_server() const { return proxy_list_.Get(); }

  
  std::string ToPacString() const;

  
  
  bool Fallback(ProxyRetryInfoMap* proxy_retry_info);

  
  
  void DeprioritizeBadProxies(const ProxyRetryInfoMap& proxy_retry_info);

  
  void RemoveProxiesWithoutScheme(int scheme_bit_field);

 private:
  friend class ProxyService;

  
  
  ProxyList proxy_list_;

  
  ProxyConfig::ID config_id_;
};

}  

#endif  
