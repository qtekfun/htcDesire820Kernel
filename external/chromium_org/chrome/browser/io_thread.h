// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IO_THREAD_H_
#define CHROME_BROWSER_IO_THREAD_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/net/ssl_config_service_manager.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/browser_thread_delegate.h"
#include "net/base/network_change_notifier.h"
#include "net/http/http_network_session.h"
#include "net/socket/next_proto.h"

class ChromeNetLog;
class CommandLine;
class PrefProxyConfigTracker;
class PrefService;
class PrefRegistrySimple;
class SystemURLRequestContextGetter;

namespace chrome_browser_net {
class DnsProbeService;
class HttpPipeliningCompatibilityClient;
class LoadTimeStats;
}

namespace extensions {
class EventRouterForwarder;
}

namespace net {
class CertVerifier;
class CookieStore;
class CTVerifier;
class FtpTransactionFactory;
class HostMappingRules;
class HostResolver;
class HttpAuthHandlerFactory;
class HttpServerProperties;
class HttpTransactionFactory;
class HttpUserAgentSettings;
class NetworkDelegate;
class NetworkTimeNotifier;
class ServerBoundCertService;
class ProxyConfigService;
class ProxyService;
class SdchManager;
class SSLConfigService;
class TransportSecurityState;
class URLRequestContext;
class URLRequestContextGetter;
class URLRequestJobFactory;
class URLRequestThrottlerManager;
class URLSecurityManager;
}  

namespace policy {
class PolicyService;
}  

class IOThread : public content::BrowserThreadDelegate {
 public:
  struct Globals {
    template <typename T>
    class Optional {
     public:
      Optional() : set_(false) {}

      void set(T value) {
        set_ = true;
        value_ = value;
      }
      void CopyToIfSet(T* value) {
        if (set_) {
          *value = value_;
        }
      }

     private:
      bool set_;
      T value_;
    };

    class SystemRequestContextLeakChecker {
     public:
      explicit SystemRequestContextLeakChecker(Globals* globals);
      ~SystemRequestContextLeakChecker();

     private:
      Globals* const globals_;
    };

    Globals();
    ~Globals();

    
    scoped_ptr<net::NetworkDelegate> system_network_delegate;
    scoped_ptr<net::HostResolver> host_resolver;
    scoped_ptr<net::CertVerifier> cert_verifier;
    
    scoped_ptr<net::ServerBoundCertService> system_server_bound_cert_service;
    
    
    
    scoped_ptr<net::TransportSecurityState> transport_security_state;
    scoped_ptr<net::CTVerifier> cert_transparency_verifier;
    scoped_refptr<net::SSLConfigService> ssl_config_service;
    scoped_ptr<net::HttpAuthHandlerFactory> http_auth_handler_factory;
    scoped_ptr<net::HttpServerProperties> http_server_properties;
    scoped_ptr<net::ProxyService> proxy_script_fetcher_proxy_service;
    scoped_ptr<net::HttpTransactionFactory>
        proxy_script_fetcher_http_transaction_factory;
    scoped_ptr<net::FtpTransactionFactory>
        proxy_script_fetcher_ftp_transaction_factory;
    scoped_ptr<net::URLRequestJobFactory>
        proxy_script_fetcher_url_request_job_factory;
    scoped_ptr<net::URLRequestThrottlerManager> throttler_manager;
    scoped_ptr<net::URLSecurityManager> url_security_manager;
    
    
    
    
    
    
    scoped_ptr<net::URLRequestContext> proxy_script_fetcher_context;
    scoped_ptr<net::ProxyService> system_proxy_service;
    scoped_ptr<net::HttpTransactionFactory> system_http_transaction_factory;
    scoped_ptr<net::URLRequestContext> system_request_context;
    SystemRequestContextLeakChecker system_request_context_leak_checker;
    
    
    scoped_refptr<net::CookieStore> system_cookie_store;
    scoped_refptr<extensions::EventRouterForwarder>
        extension_event_router_forwarder;
    scoped_ptr<chrome_browser_net::HttpPipeliningCompatibilityClient>
        http_pipelining_compatibility_client;
    scoped_ptr<chrome_browser_net::LoadTimeStats> load_time_stats;
    scoped_ptr<net::HostMappingRules> host_mapping_rules;
    scoped_ptr<net::HttpUserAgentSettings> http_user_agent_settings;
    bool ignore_certificate_errors;
    bool http_pipelining_enabled;
    uint16 testing_fixed_http_port;
    uint16 testing_fixed_https_port;
    Optional<size_t> initial_max_spdy_concurrent_streams;
    Optional<size_t> max_spdy_concurrent_streams_limit;
    Optional<bool> force_spdy_single_domain;
    Optional<bool> enable_spdy_ip_pooling;
    Optional<bool> enable_spdy_compression;
    Optional<bool> enable_spdy_ping_based_connection_checking;
    Optional<net::NextProto> spdy_default_protocol;
    Optional<string> trusted_spdy_proxy;
    Optional<bool> enable_quic;
    Optional<bool> enable_quic_https;
    Optional<size_t> quic_max_packet_length;
    Optional<net::HostPortPair> origin_to_force_quic_on;
    bool enable_user_alternate_protocol_ports;
    
    
    
    scoped_ptr<chrome_browser_net::DnsProbeService> dns_probe_service;
    scoped_ptr<net::NetworkTimeNotifier> network_time_notifier;
  };

  
  IOThread(PrefService* local_state,
           policy::PolicyService* policy_service,
           ChromeNetLog* net_log,
           extensions::EventRouterForwarder* extension_event_router_forwarder);

  virtual ~IOThread();

  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  Globals* globals();

  
  
  
  void SetGlobalsForTesting(Globals* globals);

  ChromeNetLog* net_log();

  
  void ChangedToOnTheRecord();

  
  net::URLRequestContextGetter* system_url_request_context_getter();

  
  
  
  void ClearHostCache();

  void InitializeNetworkSessionParams(net::HttpNetworkSession::Params* params);

 private:
  
  
  friend class SystemURLRequestContextGetter;

  
  
  
  virtual void Init() OVERRIDE;
  virtual void InitAsync() OVERRIDE;
  virtual void CleanUp() OVERRIDE;

  void InitializeNetworkOptions(const CommandLine& parsed_command_line);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void EnableSpdy(const std::string& mode);

  
  
  void InitSystemRequestContext();

  
  
  
  
  void InitSystemRequestContextOnIOThread();

  net::HttpAuthHandlerFactory* CreateDefaultAuthHandlerFactory(
      net::HostResolver* resolver);

  
  net::SSLConfigService* GetSSLConfigService();

  void ChangedToOnTheRecordOnIOThread();

  void UpdateDnsClientEnabled();

  
  
  void ConfigureQuic(const CommandLine& command_line);

  
  
  bool ShouldEnableQuic(const CommandLine& command_line,
                        base::StringPiece quic_trial_group);

  
  
  bool ShouldEnableQuicHttps(const CommandLine& command_line,
                             base::StringPiece quic_trial_group);

  
  
  
  size_t GetQuicMaxPacketLength(const CommandLine& command_line,
                                base::StringPiece quic_trial_group);

  
  
  ChromeNetLog* net_log_;

  
  
  extensions::EventRouterForwarder* extension_event_router_forwarder_;

  
  
  
  

  
  

  Globals* globals_;

  
  class LoggingNetworkChangeObserver;
  scoped_ptr<LoggingNetworkChangeObserver> network_change_observer_;

  BooleanPrefMember system_enable_referrers_;

  BooleanPrefMember dns_client_enabled_;

  
  std::string auth_schemes_;
  bool negotiate_disable_cname_lookup_;
  bool negotiate_enable_port_;
  std::string auth_server_whitelist_;
  std::string auth_delegate_whitelist_;
  std::string gssapi_library_name_;
  std::vector<GURL> spdyproxy_auth_origins_;

  
  
  scoped_ptr<SSLConfigServiceManager> ssl_config_service_manager_;

  
  
  scoped_ptr<net::ProxyConfigService> system_proxy_config_service_;

  scoped_ptr<PrefProxyConfigTracker> pref_proxy_config_tracker_;

  scoped_refptr<net::URLRequestContextGetter>
      system_url_request_context_getter_;

  net::SdchManager* sdch_manager_;

  
  bool is_spdy_disabled_by_policy_;

  base::WeakPtrFactory<IOThread> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(IOThread);
};

#endif  
