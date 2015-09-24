// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_LIST_H_
#define NET_PROXY_PROXY_LIST_H_

#include <string>
#include <vector>

#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/proxy/proxy_retry_info.h"

namespace base {
class ListValue;
class TimeDelta;
}

namespace net {

class ProxyServer;

class NET_EXPORT_PRIVATE ProxyList {
 public:
  ProxyList();
  ~ProxyList();

  
  
  void Set(const std::string& proxy_uri_list);

  
  void SetSingleProxyServer(const ProxyServer& proxy_server);

  
  void AddProxyServer(const ProxyServer& proxy_server);

  
  
  void DeprioritizeBadProxies(const ProxyRetryInfoMap& proxy_retry_info);

  
  
  bool HasUntriedProxies(const ProxyRetryInfoMap& proxy_retry_info) const;

  
  
  void RemoveProxiesWithoutScheme(int scheme_bit_field);

  
  void Clear();

  
  bool IsEmpty() const;

  
  size_t size() const;

  
  bool Equals(const ProxyList& other) const;

  
  
  const ProxyServer& Get() const;

  
  
  
  
  
  
  void SetFromPacString(const std::string& pac_string);

  
  
  std::string ToPacString() const;

  
  base::ListValue* ToValue() const;

  
  
  
  bool Fallback(ProxyRetryInfoMap* proxy_retry_info,
                const BoundNetLog& net_log);

  
  
  
  
  
  
  void UpdateRetryInfoOnFallback(
      ProxyRetryInfoMap* proxy_retry_info,
      base::TimeDelta retry_delay,
      const ProxyServer& another_proxy_to_bypass,
      const BoundNetLog& net_log) const;

 private:
  
  
  
  
  
  
  void AddProxyToRetryList(ProxyRetryInfoMap* proxy_retry_info,
                           base::TimeDelta retry_delay,
                           const std::string& proxy_key,
                           const BoundNetLog& net_log) const;

  
  std::vector<ProxyServer> proxies_;
};

}  

#endif  
