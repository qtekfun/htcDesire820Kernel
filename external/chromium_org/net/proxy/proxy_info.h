// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_INFO_H_
#define NET_PROXY_PROXY_INFO_H_

#include <string>

#include "base/time/time.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/proxy/proxy_config.h"
#include "net/proxy/proxy_list.h"
#include "net/proxy/proxy_retry_info.h"
#include "net/proxy/proxy_server.h"

namespace net {

class NET_EXPORT ProxyInfo {
 public:
  ProxyInfo();
  ~ProxyInfo();
  

  
  void Use(const ProxyInfo& proxy_info);

  
  void UseDirect();

  
  
  void UseDirectWithBypassedProxy();

  
  
  
  
  void UseNamedProxy(const std::string& proxy_uri_list);

  
  void UseProxyServer(const ProxyServer& proxy_server);

  
  void UsePacString(const std::string& pac_string);

  
  void UseProxyList(const ProxyList& proxy_list);

  
  bool is_direct() const {
    
    if (is_empty())
      return false;
    return proxy_list_.Get().is_direct();
  }

  bool is_direct_only() const {
    return is_direct() && proxy_list_.size() == 1 && proxy_retry_info_.empty();
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

  
  
  bool did_bypass_proxy() const {
    return did_bypass_proxy_;
  }

  
  bool did_use_pac_script() const {
    return did_use_pac_script_;
  }

  
  
  const ProxyServer& proxy_server() const { return proxy_list_.Get(); }

  
  ProxyConfigSource config_source() const { return config_source_; }

  
  std::string ToPacString() const;

  
  
  bool Fallback(const BoundNetLog& net_log);

  
  
  void DeprioritizeBadProxies(const ProxyRetryInfoMap& proxy_retry_info);

  
  void RemoveProxiesWithoutScheme(int scheme_bit_field);

  ProxyConfig::ID config_id() const { return config_id_; }

  base::TimeTicks proxy_resolve_start_time() const {
    return proxy_resolve_start_time_;
  }

  base::TimeTicks proxy_resolve_end_time() const {
    return proxy_resolve_end_time_;
  }

 private:
  friend class ProxyService;

  const ProxyRetryInfoMap& proxy_retry_info() const {
    return proxy_retry_info_;
  }

  
  void Reset();

  
  
  ProxyList proxy_list_;

  
  ProxyRetryInfoMap proxy_retry_info_;

  
  ProxyConfig::ID config_id_;

  
  ProxyConfigSource config_source_;

  
  bool did_bypass_proxy_;

  
  bool did_use_pac_script_;

  
  
  base::TimeTicks proxy_resolve_start_time_;
  base::TimeTicks proxy_resolve_end_time_;
};

}  

#endif  
