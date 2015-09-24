// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_SERVICE_H_
#define NET_PROXY_PROXY_SERVICE_H_

#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/completion_callback.h"
#include "net/base/load_states.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/base/network_change_notifier.h"
#include "net/proxy/proxy_config_service.h"
#include "net/proxy/proxy_info.h"
#include "net/proxy/proxy_server.h"

class GURL;

namespace base {
class MessageLoop;
class SingleThreadTaskRunner;
class TimeDelta;
}  

namespace net {

class DhcpProxyScriptFetcher;
class HostResolver;
class NetworkDelegate;
class ProxyResolver;
class ProxyResolverScriptData;
class ProxyScriptDecider;
class ProxyScriptFetcher;

class NET_EXPORT ProxyService : public NetworkChangeNotifier::IPAddressObserver,
                                public NetworkChangeNotifier::DNSObserver,
                                public ProxyConfigService::Observer,
                                NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  static const size_t kDefaultNumPacThreads = 4;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  class NET_EXPORT_PRIVATE PacPollPolicy {
   public:
    enum Mode {
      MODE_USE_TIMER,
      MODE_START_AFTER_ACTIVITY,
    };

    virtual ~PacPollPolicy() {}

    
    
    
    
    
    
    virtual Mode GetNextDelay(int initial_error,
                              base::TimeDelta current_delay,
                              base::TimeDelta* next_delay) const = 0;
  };

  
  
  
  ProxyService(ProxyConfigService* config_service,
               ProxyResolver* resolver,
               NetLog* net_log);

  virtual ~ProxyService();

  
  
  class PacRequest;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int ResolveProxy(const GURL& url,
                   ProxyInfo* results,
                   const net::CompletionCallback& callback,
                   PacRequest** pac_request,
                   const BoundNetLog& net_log);

  
  
  
  
  
  
  
  
  
  
  
  
  int ReconsiderProxyAfterError(const GURL& url,
                                ProxyInfo* results,
                                const CompletionCallback& callback,
                                PacRequest** pac_request,
                                const BoundNetLog& net_log);

  
  
  
  
  
  
  bool MarkProxiesAsBad(const ProxyInfo& results,
                        base::TimeDelta retry_delay,
                        const ProxyServer& another_bad_proxy,
                        const BoundNetLog& net_log);

  
  
  
  void ReportSuccess(const ProxyInfo& proxy_info);

  
  void CancelPacRequest(PacRequest* pac_request);

  
  LoadState GetLoadState(const PacRequest* pac_request) const;

  
  
  
  void SetProxyScriptFetchers(
      ProxyScriptFetcher* proxy_script_fetcher,
      DhcpProxyScriptFetcher* dhcp_proxy_script_fetcher);
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
      base::SingleThreadTaskRunner* io_thread_task_runner,
      base::MessageLoop* file_loop);

  
  void set_stall_proxy_auto_config_delay(base::TimeDelta delay) {
    stall_proxy_auto_config_delay_ = delay;
  }

  
  
  static const PacPollPolicy* set_pac_script_poll_policy(
      const PacPollPolicy* policy);

  
  
  static scoped_ptr<PacPollPolicy> CreateDefaultPacPollPolicy();

#if defined(SPDY_PROXY_AUTH_ORIGIN)
  
  
  
  enum DataReductionProxyBypassEventType {
    
    SHORT_BYPASS = 0,

    
    LONG_BYPASS,

    
    INTERNAL_SERVER_ERROR_BYPASS,

    
    ERROR_BYPASS,

    
    MISSING_VIA_HEADER,

    
    BYPASS_EVENT_TYPE_MAX
  };

  
  
  void RecordDataReductionProxyBypassInfo(
      bool is_primary,
      const ProxyServer& proxy_server,
      DataReductionProxyBypassEventType bypass_type) const;
#endif

 private:
  FRIEND_TEST_ALL_PREFIXES(ProxyServiceTest, UpdateConfigAfterFailedAutodetect);
  FRIEND_TEST_ALL_PREFIXES(ProxyServiceTest, UpdateConfigFromPACToDirect);
  friend class PacRequest;
  class InitProxyResolver;
  class ProxyScriptDeciderPoller;

  
  
  
  
  typedef std::vector<scoped_refptr<PacRequest> > PendingRequests;

  enum State {
    STATE_NONE,
    STATE_WAITING_FOR_PROXY_CONFIG,
    STATE_WAITING_FOR_INIT_PROXY_RESOLVER,
    STATE_READY,
  };

  
  
  
  
  
  
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

  
  void InitializeUsingDecidedConfig(
      int decider_result,
      ProxyResolverScriptData* script_data,
      const ProxyConfig& effective_config);

  
  
  virtual void OnIPAddressChanged() OVERRIDE;

  
  
  virtual void OnDNSChanged() OVERRIDE;

  
  virtual void OnProxyConfigChanged(
      const ProxyConfig& config,
      ProxyConfigService::ConfigAvailability availability) OVERRIDE;

  scoped_ptr<ProxyConfigService> config_service_;
  scoped_ptr<ProxyResolver> resolver_;

  
  
  
  
  
  ProxyConfig fetched_config_;
  ProxyConfig config_;

  
  int next_config_id_;

  
  base::TimeTicks config_last_update_time_;

  
  ProxyRetryInfoMap proxy_retry_info_;

  
  PendingRequests pending_requests_;

  
  
  
  scoped_ptr<ProxyScriptFetcher> proxy_script_fetcher_;

  
  
  
  scoped_ptr<DhcpProxyScriptFetcher> dhcp_proxy_script_fetcher_;

  
  
  
  
  scoped_ptr<InitProxyResolver> init_proxy_resolver_;

  
  scoped_ptr<ProxyScriptDeciderPoller> script_poller_;

  State current_state_;

  
  
  int permanent_error_;

  
  
  NetLog* net_log_;

  
  
  base::TimeTicks stall_proxy_autoconfig_until_;

  
  base::TimeDelta stall_proxy_auto_config_delay_;

  DISALLOW_COPY_AND_ASSIGN(ProxyService);
};

class NET_EXPORT SyncProxyServiceHelper
    : public base::RefCountedThreadSafe<SyncProxyServiceHelper> {
 public:
  SyncProxyServiceHelper(base::MessageLoop* io_message_loop,
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

  base::MessageLoop* io_message_loop_;
  ProxyService* proxy_service_;

  base::WaitableEvent event_;
  CompletionCallback callback_;
  ProxyInfo proxy_info_;
  int result_;
};

}  

#endif  
