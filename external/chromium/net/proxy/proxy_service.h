// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_SERVICE_H_
#define NET_PROXY_PROXY_SERVICE_H_
#pragma once

#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/base/network_change_notifier.h"
#include "net/proxy/proxy_config_service.h"
#include "net/proxy/proxy_info.h"
#include "net/proxy/proxy_server.h"

class GURL;
class MessageLoop;

namespace net {

class HostResolver;
class InitProxyResolver;
class ProxyResolver;
class ProxyScriptFetcher;
class URLRequestContext;

class NET_EXPORT ProxyService : public base::RefCounted<ProxyService>,
                     public NetworkChangeNotifier::IPAddressObserver,
                     public ProxyConfigService::Observer,
                     public base::NonThreadSafe {
 public:
  
  
  
  ProxyService(ProxyConfigService* config_service,
               ProxyResolver* resolver,
               NetLog* net_log);

  
  
  class PacRequest;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int ResolveProxy(const GURL& url,
                   ProxyInfo* results,
                   CompletionCallback* callback,
                   PacRequest** pac_request,
                   const BoundNetLog& net_log);

  
  
  
  
  
  
  
  
  
  
  
  
  int ReconsiderProxyAfterError(const GURL& url,
                                ProxyInfo* results,
                                CompletionCallback* callback,
                                PacRequest** pac_request,
                                const BoundNetLog& net_log);

  
  void CancelPacRequest(PacRequest* pac_request);

  
  
  
  void SetProxyScriptFetcher(ProxyScriptFetcher* proxy_script_fetcher);
  ProxyScriptFetcher* GetProxyScriptFetcher() const;

  
  
  
  
  
  void ResetConfigService(ProxyConfigService* new_proxy_config_service);

  
  void PurgeMemory();


  
  const ProxyConfig& fetched_config() {
    return fetched_config_;
  }

  
  const ProxyConfig& config() {
    return config_;
  }

  
  const ProxyRetryInfoMap& proxy_retry_info() const {
    return proxy_retry_info_;
  }

  
  void ClearBadProxiesCache() {
    proxy_retry_info_.clear();
  }

  
  
  
  void ForceReloadProxyConfig();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  
  
  
  
  
  
  
  
  
  
  static ProxyService* CreateUsingV8ProxyResolver(
      ProxyConfigService* proxy_config_service,
      size_t num_pac_threads,
      ProxyScriptFetcher* proxy_script_fetcher,
      HostResolver* host_resolver,
      NetLog* net_log);

  
  
  
  static ProxyService* CreateUsingSystemProxyResolver(
      ProxyConfigService* proxy_config_service,
      size_t num_pac_threads,
      NetLog* net_log);

  
  static ProxyService* CreateWithoutProxyResolver(
      ProxyConfigService* proxy_config_service,
      NetLog* net_log);

  
  
  static ProxyService* CreateFixed(const ProxyConfig& pc);
  static ProxyService* CreateFixed(const std::string& proxy);

  
  static ProxyService* CreateDirect();
  
  static ProxyService* CreateDirectWithNetLog(NetLog* net_log);

  
  
  
  
  
  static ProxyService* CreateFixedFromPacResult(const std::string& pac_string);

  
  
  static ProxyConfigService* CreateSystemProxyConfigService(
      MessageLoop* io_loop, MessageLoop* file_loop);

#if UNIT_TEST
  void set_stall_proxy_auto_config_delay(base::TimeDelta delay) {
    stall_proxy_auto_config_delay_ = delay;
  }
#endif

 private:
  friend class base::RefCounted<ProxyService>;
  FRIEND_TEST_ALL_PREFIXES(ProxyServiceTest, UpdateConfigAfterFailedAutodetect);
  FRIEND_TEST_ALL_PREFIXES(ProxyServiceTest, UpdateConfigFromPACToDirect);
  friend class PacRequest;

  
  
  
  
  typedef std::vector<scoped_refptr<PacRequest> > PendingRequests;

  enum State {
    STATE_NONE,
    STATE_WAITING_FOR_PROXY_CONFIG,
    STATE_WAITING_FOR_INIT_PROXY_RESOLVER,
    STATE_READY,
  };

  virtual ~ProxyService();

  
  
  
  
  
  
  State ResetProxyConfig(bool reset_fetched_config);

  
  
  void ApplyProxyConfigIfAvailable();

  
  
  void OnInitProxyResolverComplete(int result);

  
  
  
  int TryToCompleteSynchronously(const GURL& url, ProxyInfo* result);

  
  
  void SuspendAllPendingRequests();

  
  
  void SetReady();

  
  bool ContainsPendingRequest(PacRequest* req);

  
  void RemovePendingRequest(PacRequest* req);

  
  
  
  int DidFinishResolvingProxy(ProxyInfo* result,
                              int result_code,
                              const BoundNetLog& net_log);

  
  void InitializeUsingLastFetchedConfig();

  
  
  virtual void OnIPAddressChanged();

  
  virtual void OnProxyConfigChanged(
      const ProxyConfig& config,
      ProxyConfigService::ConfigAvailability availability);

  scoped_ptr<ProxyConfigService> config_service_;
  scoped_ptr<ProxyResolver> resolver_;

  
  
  
  
  
  ProxyConfig fetched_config_;
  ProxyConfig config_;

  
  int next_config_id_;

  
  base::TimeTicks config_last_update_time_;

  
  ProxyRetryInfoMap proxy_retry_info_;

  
  PendingRequests pending_requests_;

  
  
  
  scoped_ptr<ProxyScriptFetcher> proxy_script_fetcher_;

  
  CompletionCallbackImpl<ProxyService> init_proxy_resolver_callback_;

  
  
  
  
  scoped_ptr<InitProxyResolver> init_proxy_resolver_;

  State current_state_;

  
  
  NetLog* net_log_;

  
  
  base::TimeTicks stall_proxy_autoconfig_until_;

  
  base::TimeDelta stall_proxy_auto_config_delay_;

  DISALLOW_COPY_AND_ASSIGN(ProxyService);
};

class SyncProxyServiceHelper
    : public base::RefCountedThreadSafe<SyncProxyServiceHelper> {
 public:
  SyncProxyServiceHelper(MessageLoop* io_message_loop,
                         ProxyService* proxy_service);

  int ResolveProxy(const GURL& url,
                   ProxyInfo* proxy_info,
                   const BoundNetLog& net_log);
  int ReconsiderProxyAfterError(const GURL& url,
                                ProxyInfo* proxy_info,
                                const BoundNetLog& net_log);

 private:
  friend class base::RefCountedThreadSafe<SyncProxyServiceHelper>;

  virtual ~SyncProxyServiceHelper();

  void StartAsyncResolve(const GURL& url, const BoundNetLog& net_log);
  void StartAsyncReconsider(const GURL& url, const BoundNetLog& net_log);

  void OnCompletion(int result);

  MessageLoop* io_message_loop_;
  ProxyService* proxy_service_;

  base::WaitableEvent event_;
  CompletionCallbackImpl<SyncProxyServiceHelper> callback_;
  ProxyInfo proxy_info_;
  int result_;
};

}  

#endif  
