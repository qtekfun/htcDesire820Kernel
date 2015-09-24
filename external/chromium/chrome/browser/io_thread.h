// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IO_THREAD_H_
#define CHROME_BROWSER_IO_THREAD_H_
#pragma once

#include <list>
#include <string>
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/browser_process_sub_thread.h"
#include "chrome/browser/prefs/pref_member.h"
#include "chrome/common/net/predictor_common.h"
#include "net/base/network_change_notifier.h"

class ChromeNetLog;
class ChromeURLRequestContextGetter;
class ExtensionEventRouterForwarder;
class ListValue;
class PrefProxyConfigTracker;
class PrefService;
class SystemURLRequestContextGetter;

namespace chrome_browser_net {
class ConnectInterceptor;
class Predictor;
}  

namespace net {
class CertVerifier;
class DnsRRResolver;
class FtpTransactionFactory;
class HostResolver;
class HttpAuthHandlerFactory;
class HttpTransactionFactory;
class NetworkDelegate;
class ProxyConfigService;
class ProxyScriptFetcher;
class ProxyService;
class SSLConfigService;
class URLRequestContext;
class URLRequestContextGetter;
class URLSecurityManager;
}  

class IOThread : public BrowserProcessSubThread {
 public:
  struct Globals {
    Globals();
    ~Globals();

    
    scoped_ptr<net::NetworkDelegate> system_network_delegate;
    scoped_ptr<net::HostResolver> host_resolver;
    scoped_ptr<net::CertVerifier> cert_verifier;
    scoped_ptr<net::DnsRRResolver> dnsrr_resolver;
    scoped_refptr<net::SSLConfigService> ssl_config_service;
    scoped_ptr<net::HttpAuthHandlerFactory> http_auth_handler_factory;
    scoped_refptr<net::ProxyService> proxy_script_fetcher_proxy_service;
    scoped_ptr<net::HttpTransactionFactory>
        proxy_script_fetcher_http_transaction_factory;
    scoped_ptr<net::FtpTransactionFactory>
        proxy_script_fetcher_ftp_transaction_factory;
    scoped_ptr<net::URLSecurityManager> url_security_manager;
    scoped_refptr<net::URLRequestContext> proxy_script_fetcher_context;
    scoped_ptr<net::HttpTransactionFactory> system_http_transaction_factory;
    scoped_ptr<net::FtpTransactionFactory> system_ftp_transaction_factory;
    scoped_refptr<net::ProxyService> system_proxy_service;
    
    
    
    scoped_refptr<net::URLRequestContext> system_request_context;
    scoped_refptr<ExtensionEventRouterForwarder>
        extension_event_router_forwarder;
  };

  
  IOThread(PrefService* local_state,
           ChromeNetLog* net_log,
           ExtensionEventRouterForwarder* extension_event_router_forwarder);

  virtual ~IOThread();

  
  Globals* globals();

  ChromeNetLog* net_log();

  
  
  
  
  
  void InitNetworkPredictor(bool prefetching_enabled,
                            base::TimeDelta max_dns_queue_delay,
                            size_t max_speculative_parallel_resolves,
                            const chrome_common_net::UrlList& startup_urls,
                            ListValue* referral_list,
                            bool preconnect_enabled);

  
  
  
  
  
  
  
  void RegisterURLRequestContextGetter(
      ChromeURLRequestContextGetter* url_request_context_getter);

  
  
  void UnregisterURLRequestContextGetter(
      ChromeURLRequestContextGetter* url_request_context_getter);

  
  void ChangedToOnTheRecord();

  
  net::URLRequestContextGetter* system_url_request_context_getter();

  
  
  
  void ClearNetworkingHistory();

 protected:
  virtual void Init();
  virtual void CleanUp();

 private:
  
  
  friend class SystemURLRequestContextGetter;

  static void RegisterPrefs(PrefService* local_state);

  net::HttpAuthHandlerFactory* CreateDefaultAuthHandlerFactory(
      net::HostResolver* resolver);

  
  
  void InitSystemRequestContext();

  void InitNetworkPredictorOnIOThread(
      bool prefetching_enabled,
      base::TimeDelta max_dns_queue_delay,
      size_t max_speculative_parallel_resolves,
      const chrome_common_net::UrlList& startup_urls,
      ListValue* referral_list,
      bool preconnect_enabled);

  void ChangedToOnTheRecordOnIOThread();

  
  
  
  void ClearHostCache();

  
  
  ChromeNetLog* net_log_;

  
  
  ExtensionEventRouterForwarder* extension_event_router_forwarder_;

  
  
  
  

  
  

  Globals* globals_;

  
  scoped_ptr<net::NetworkChangeNotifier::IPAddressObserver>
      network_change_observer_;

  BooleanPrefMember system_enable_referrers_;

  
  std::string auth_schemes_;
  bool negotiate_disable_cname_lookup_;
  bool negotiate_enable_port_;
  std::string auth_server_whitelist_;
  std::string auth_delegate_whitelist_;
  std::string gssapi_library_name_;

  
  

  
  
  
  
  chrome_browser_net::ConnectInterceptor* speculative_interceptor_;
  chrome_browser_net::Predictor* predictor_;

  scoped_ptr<net::ProxyConfigService> system_proxy_config_service_;

  scoped_refptr<PrefProxyConfigTracker> pref_proxy_config_tracker_;

  scoped_refptr<net::URLRequestContextGetter>
      system_url_request_context_getter_;

  
  
  
  std::list<ChromeURLRequestContextGetter*> url_request_context_getters_;

  DISALLOW_COPY_AND_ASSIGN(IOThread);
};

#endif  
