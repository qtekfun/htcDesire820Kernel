// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_INIT_PROXY_RESOLVER_H_
#define NET_PROXY_INIT_PROXY_RESOLVER_H_
#pragma once

#include <vector>

#include "base/string16.h"
#include "base/time.h"
#include "base/timer.h"
#include "googleurl/src/gurl.h"
#include "net/base/completion_callback.h"
#include "net/base/net_log.h"

namespace net {

class ProxyConfig;
class ProxyResolver;
class ProxyScriptFetcher;

class InitProxyResolver {
 public:
  
  
  InitProxyResolver(ProxyResolver* resolver,
                    ProxyScriptFetcher* proxy_script_fetcher,
                    NetLog* net_log);

  
  ~InitProxyResolver();

  
  
  
  
  
  // written out to |*effective_config|. Note that this may differ from
  
  
  
  int Init(const ProxyConfig& config,
           const base::TimeDelta wait_delay,
           ProxyConfig* effective_config,
           CompletionCallback* callback);

 private:
  struct PacURL {
    PacURL(bool auto_detect, const GURL& url)
        : auto_detect(auto_detect), url(url) {}
    bool auto_detect;
    GURL url;
  };

  typedef std::vector<PacURL> UrlList;

  enum State {
    STATE_NONE,
    STATE_WAIT,
    STATE_WAIT_COMPLETE,
    STATE_FETCH_PAC_SCRIPT,
    STATE_FETCH_PAC_SCRIPT_COMPLETE,
    STATE_SET_PAC_SCRIPT,
    STATE_SET_PAC_SCRIPT_COMPLETE,
  };

  
  UrlList BuildPacUrlsFallbackList(const ProxyConfig& config) const;

  void OnIOCompletion(int result);
  int DoLoop(int result);
  void DoCallback(int result);

  int DoWait();
  int DoWaitComplete(int result);

  int DoFetchPacScript();
  int DoFetchPacScriptComplete(int result);

  int DoSetPacScript();
  int DoSetPacScriptComplete(int result);

  
  
  
  
  int TryToFallbackPacUrl(int error);

  
  
  State GetStartState() const;

  
  const PacURL& current_pac_url() const;

  void OnWaitTimerFired();
  void DidCompleteInit();
  void Cancel();

  ProxyResolver* resolver_;
  ProxyScriptFetcher* proxy_script_fetcher_;

  CompletionCallbackImpl<InitProxyResolver> io_callback_;
  CompletionCallback* user_callback_;

  size_t current_pac_url_index_;

  
  string16 pac_script_;

  UrlList pac_urls_;
  State next_state_;

  BoundNetLog net_log_;

  base::TimeDelta wait_delay_;
  base::OneShotTimer<InitProxyResolver> wait_timer_;

  ProxyConfig* effective_config_;

  DISALLOW_COPY_AND_ASSIGN(InitProxyResolver);
};

}  

#endif  
