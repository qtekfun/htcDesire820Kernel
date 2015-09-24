// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_SCRIPT_DECIDER_H_
#define NET_PROXY_PROXY_SCRIPT_DECIDER_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "net/base/address_list.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/dns/host_resolver.h"
#include "net/dns/single_request_host_resolver.h"
#include "net/proxy/proxy_config.h"
#include "net/proxy/proxy_resolver.h"
#include "url/gurl.h"

namespace net {

class DhcpProxyScriptFetcher;
class NetLogParameter;
class ProxyResolver;
class ProxyScriptFetcher;

class NET_EXPORT_PRIVATE ProxyScriptDecider {
 public:
  
  
  ProxyScriptDecider(ProxyScriptFetcher* proxy_script_fetcher,
                     DhcpProxyScriptFetcher* dhcp_proxy_script_fetcher,
                     NetLog* net_log);

  
  ~ProxyScriptDecider();

  
  
  
  
  
  
  
  
  
  
  int Start(const ProxyConfig& config,
            const base::TimeDelta wait_delay,
            bool fetch_pac_bytes,
            const net::CompletionCallback& callback);

  const ProxyConfig& effective_config() const;

  
  ProxyResolverScriptData* script_data() const;

 private:
  
  
  struct PacSource {
    enum Type {
      WPAD_DHCP,
      WPAD_DNS,
      CUSTOM
    };

    PacSource(Type type, const GURL& url)
        : type(type), url(url) {}

    
    
    
    base::Value* NetLogCallback(const GURL* effective_pac_url,
                                NetLog::LogLevel log_level) const;

    Type type;
    GURL url;  
  };

  typedef std::vector<PacSource> PacSourceList;

  enum State {
    STATE_NONE,
    STATE_WAIT,
    STATE_WAIT_COMPLETE,
    STATE_QUICK_CHECK,
    STATE_QUICK_CHECK_COMPLETE,
    STATE_FETCH_PAC_SCRIPT,
    STATE_FETCH_PAC_SCRIPT_COMPLETE,
    STATE_VERIFY_PAC_SCRIPT,
    STATE_VERIFY_PAC_SCRIPT_COMPLETE,
  };

  
  PacSourceList BuildPacSourcesFallbackList(const ProxyConfig& config) const;

  void OnIOCompletion(int result);
  int DoLoop(int result);
  void DoCallback(int result);

  int DoWait();
  int DoWaitComplete(int result);

  int DoQuickCheck();
  int DoQuickCheckComplete(int result);

  int DoFetchPacScript();
  int DoFetchPacScriptComplete(int result);

  int DoVerifyPacScript();
  int DoVerifyPacScriptComplete(int result);

  
  
  
  
  int TryToFallbackPacSource(int error);

  
  
  State GetStartState() const;

  void DetermineURL(const PacSource& pac_source, GURL* effective_pac_url);

  
  const PacSource& current_pac_source() const;

  void OnWaitTimerFired();
  void DidComplete();
  void Cancel();

  ProxyResolver* resolver_;
  ProxyScriptFetcher* proxy_script_fetcher_;
  DhcpProxyScriptFetcher* dhcp_proxy_script_fetcher_;

  net::CompletionCallback callback_;

  size_t current_pac_source_index_;

  
  base::string16 pac_script_;

  
  
  bool pac_mandatory_;

  
  bool have_custom_pac_url_;

  PacSourceList pac_sources_;
  State next_state_;

  BoundNetLog net_log_;

  bool fetch_pac_bytes_;

  base::TimeDelta wait_delay_;
  base::OneShotTimer<ProxyScriptDecider> wait_timer_;

  
  ProxyConfig effective_config_;
  scoped_refptr<ProxyResolverScriptData> script_data_;

  AddressList wpad_addresses_;
  base::OneShotTimer<ProxyScriptDecider> quick_check_timer_;
  scoped_ptr<SingleRequestHostResolver> host_resolver_;
  base::Time quick_check_start_time_;

  DISALLOW_COPY_AND_ASSIGN(ProxyScriptDecider);
};

}  

#endif  
