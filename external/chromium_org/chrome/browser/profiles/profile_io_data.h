// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILE_IO_DATA_H_
#define CHROME_BROWSER_PROFILES_PROFILE_IO_DATA_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_member.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/custom_handlers/protocol_handler_registry.h"
#include "chrome/browser/io_thread.h"
#include "chrome/browser/net/chrome_url_request_context.h"
#include "chrome/browser/profiles/storage_partition_descriptor.h"
#include "chrome/common/content_settings_types.h"
#include "content/public/browser/content_browser_client.h"
#include "content/public/browser/resource_context.h"
#include "net/cookies/cookie_monster.h"
#include "net/http/http_network_session.h"
#include "net/url_request/url_request_job_factory.h"

class ChromeHttpUserAgentSettings;
class ChromeNetworkDelegate;
class CookieSettings;
class HostContentSettingsMap;
class ManagedModeURLFilter;
class MediaDeviceIDSalt;
class Profile;
class ProtocolHandlerRegistry;
class SigninNamesOnIOThread;

namespace chrome_browser_net {
class LoadTimeStats;
class ResourcePrefetchPredictorObserver;
}

namespace extensions {
class InfoMap;
}

namespace net {
class CookieStore;
class FraudulentCertificateReporter;
class FtpTransactionFactory;
class HttpServerProperties;
class HttpTransactionFactory;
class ServerBoundCertService;
class ProxyConfigService;
class ProxyService;
class SSLConfigService;
class TransportSecurityPersister;
class TransportSecurityState;
class URLRequestJobFactoryImpl;
}  

namespace policy {
class PolicyCertVerifier;
class PolicyHeaderIOHelper;
class URLBlacklistManager;
}  

class ProfileIOData {
 public:
  virtual ~ProfileIOData();

  static ProfileIOData* FromResourceContext(content::ResourceContext* rc);

  
  
  static bool IsHandledProtocol(const std::string& scheme);

  
  
  static bool IsHandledURL(const GURL& url);

  
  
  
  static void InstallProtocolHandlers(
      net::URLRequestJobFactoryImpl* job_factory,
      content::ProtocolHandlerMap* protocol_handlers);

  
  content::ResourceContext* GetResourceContext() const;

  
  
  
  void Init(content::ProtocolHandlerMap* protocol_handlers) const;

  ChromeURLRequestContext* GetMainRequestContext() const;
  ChromeURLRequestContext* GetMediaRequestContext() const;
  ChromeURLRequestContext* GetExtensionsRequestContext() const;
  ChromeURLRequestContext* GetIsolatedAppRequestContext(
      ChromeURLRequestContext* main_context,
      const StoragePartitionDescriptor& partition_descriptor,
      scoped_ptr<ProtocolHandlerRegistry::JobInterceptorFactory>
          protocol_handler_interceptor,
      content::ProtocolHandlerMap* protocol_handlers) const;
  ChromeURLRequestContext* GetIsolatedMediaRequestContext(
      ChromeURLRequestContext* app_context,
      const StoragePartitionDescriptor& partition_descriptor) const;

  
  
  
  extensions::InfoMap* GetExtensionInfoMap() const;
  CookieSettings* GetCookieSettings() const;
  HostContentSettingsMap* GetHostContentSettingsMap() const;

  IntegerPrefMember* session_startup_pref() const {
    return &session_startup_pref_;
  }

  SigninNamesOnIOThread* signin_names() const {
    return signin_names_.get();
  }

  StringPrefMember* google_services_account_id() const {
    return &google_services_user_account_id_;
  }

  StringPrefMember* google_services_username() const {
    return &google_services_username_;
  }

  StringPrefMember* google_services_username_pattern() const {
    return &google_services_username_pattern_;
  }

  BooleanPrefMember* reverse_autologin_enabled() const {
    return &reverse_autologin_enabled_;
  }

  const std::string& reverse_autologin_pending_email() const {
    return reverse_autologin_pending_email_;
  }

  void set_reverse_autologin_pending_email(const std::string& email) {
    reverse_autologin_pending_email_ = email;
  }

  StringListPrefMember* one_click_signin_rejected_email_list() const {
    return &one_click_signin_rejected_email_list_;
  }

  ChromeURLRequestContext* extensions_request_context() const {
    return extensions_request_context_.get();
  }

  BooleanPrefMember* safe_browsing_enabled() const {
    return &safe_browsing_enabled_;
  }

  BooleanPrefMember* printing_enabled() const {
    return &printing_enabled_;
  }

  BooleanPrefMember* sync_disabled() const {
    return &sync_disabled_;
  }

  BooleanPrefMember* signin_allowed() const {
    return &signin_allowed_;
  }

  content::ResourceContext::SaltCallback GetMediaDeviceIDSalt() const;

  net::TransportSecurityState* transport_security_state() const {
    return transport_security_state_.get();
  }

#if defined(OS_CHROMEOS)
  std::string username_hash() const {
    return username_hash_;
  }
#endif

  bool is_incognito() const {
    return is_incognito_;
  }

  chrome_browser_net::ResourcePrefetchPredictorObserver*
      resource_prefetch_predictor_observer() const {
    return resource_prefetch_predictor_observer_.get();
  }

#if defined(ENABLE_CONFIGURATION_POLICY)
  policy::PolicyHeaderIOHelper* policy_header_helper() const {
    return policy_header_helper_.get();
  }
#endif

#if defined(ENABLE_MANAGED_USERS)
  const ManagedModeURLFilter* managed_mode_url_filter() const {
    return managed_mode_url_filter_.get();
  }
#endif

  
  
  void InitializeMetricsEnabledStateOnUIThread();

  
  
  
  bool GetMetricsEnabledStateOnIOThread() const;

 protected:
  
  
  class MediaRequestContext : public ChromeURLRequestContext {
   public:
    explicit MediaRequestContext(
        chrome_browser_net::LoadTimeStats* load_time_stats);

    void SetHttpTransactionFactory(
        scoped_ptr<net::HttpTransactionFactory> http_factory);

   private:
    virtual ~MediaRequestContext();

    scoped_ptr<net::HttpTransactionFactory> http_factory_;
  };

  
  
  class AppRequestContext : public ChromeURLRequestContext {
   public:
    explicit AppRequestContext(
        chrome_browser_net::LoadTimeStats* load_time_stats);

    void SetCookieStore(net::CookieStore* cookie_store);
    void SetHttpTransactionFactory(
        scoped_ptr<net::HttpTransactionFactory> http_factory);
    void SetJobFactory(scoped_ptr<net::URLRequestJobFactory> job_factory);

   private:
    virtual ~AppRequestContext();

    scoped_refptr<net::CookieStore> cookie_store_;
    scoped_ptr<net::HttpTransactionFactory> http_factory_;
    scoped_ptr<net::URLRequestJobFactory> job_factory_;
  };

  
  
  struct ProfileParams {
    ProfileParams();
    ~ProfileParams();

    base::FilePath path;
    IOThread* io_thread;
    scoped_refptr<CookieSettings> cookie_settings;
    scoped_refptr<HostContentSettingsMap> host_content_settings_map;
    scoped_refptr<net::SSLConfigService> ssl_config_service;
    scoped_refptr<net::CookieMonster::Delegate> cookie_monster_delegate;
    scoped_refptr<extensions::InfoMap> extension_info_map;
    scoped_ptr<chrome_browser_net::ResourcePrefetchPredictorObserver>
        resource_prefetch_predictor_observer_;

    
    
    
    
    scoped_ptr<ProtocolHandlerRegistry::JobInterceptorFactory>
        protocol_handler_interceptor;

    
    
    
    scoped_ptr<net::ProxyConfigService> proxy_config_service;

#if defined(ENABLE_MANAGED_USERS)
    scoped_refptr<const ManagedModeURLFilter> managed_mode_url_filter;
#endif

#if defined(OS_CHROMEOS)
    std::string username_hash;
#endif

    
    
    
    void* profile;
  };

  explicit ProfileIOData(bool is_incognito);

  static std::string GetSSLSessionCacheShard();

  void InitializeOnUIThread(Profile* profile);
  void ApplyProfileParamsToContext(ChromeURLRequestContext* context) const;

  scoped_ptr<net::URLRequestJobFactory> SetUpJobFactoryDefaults(
      scoped_ptr<net::URLRequestJobFactoryImpl> job_factory,
      scoped_ptr<ProtocolHandlerRegistry::JobInterceptorFactory>
          protocol_handler_interceptor,
      net::NetworkDelegate* network_delegate,
      net::FtpTransactionFactory* ftp_transaction_factory) const;

  
  void ShutdownOnUIThread();

  
  
  
  
  void set_server_bound_cert_service(
      net::ServerBoundCertService* server_bound_cert_service) const;

  ChromeNetworkDelegate* network_delegate() const {
    return network_delegate_.get();
  }

  net::FraudulentCertificateReporter* fraudulent_certificate_reporter() const {
    return fraudulent_certificate_reporter_.get();
  }

  net::ProxyService* proxy_service() const {
    return proxy_service_.get();
  }

  base::WeakPtr<net::HttpServerProperties> http_server_properties() const;

  void set_http_server_properties(
      scoped_ptr<net::HttpServerProperties> http_server_properties) const;

  ChromeURLRequestContext* main_request_context() const {
    return main_request_context_.get();
  }

  chrome_browser_net::LoadTimeStats* load_time_stats() const {
    return load_time_stats_;
  }

  bool initialized() const {
    return initialized_;
  }

  
  
  
  void DestroyResourceContext();

  
  
  void PopulateNetworkSessionParams(
      const ProfileParams* profile_params,
      net::HttpNetworkSession::Params* params) const;

  void SetCookieSettingsForTesting(CookieSettings* cookie_settings);

  void set_signin_names_for_testing(SigninNamesOnIOThread* signin_names);

 private:
  class ResourceContext : public content::ResourceContext {
   public:
    explicit ResourceContext(ProfileIOData* io_data);
    virtual ~ResourceContext();

    
    virtual net::HostResolver* GetHostResolver() OVERRIDE;
    virtual net::URLRequestContext* GetRequestContext() OVERRIDE;
    virtual scoped_ptr<net::ClientCertStore> CreateClientCertStore() OVERRIDE;
    virtual void CreateKeygenHandler(
        uint32 key_size_in_bits,
        const std::string& challenge_string,
        const GURL& url,
        const base::Callback<void(scoped_ptr<net::KeygenHandler>)>& callback)
        OVERRIDE;
    virtual bool AllowMicAccess(const GURL& origin) OVERRIDE;
    virtual bool AllowCameraAccess(const GURL& origin) OVERRIDE;
    virtual SaltCallback GetMediaDeviceIDSalt() OVERRIDE;

   private:
    friend class ProfileIOData;

    
    
    bool AllowContentAccess(const GURL& origin, ContentSettingsType type);

    ProfileIOData* const io_data_;

    net::HostResolver* host_resolver_;
    net::URLRequestContext* request_context_;
  };

  typedef std::map<StoragePartitionDescriptor,
                   ChromeURLRequestContext*,
                   StoragePartitionDescriptorLess>
      URLRequestContextMap;

  
  
  

  
  
  virtual void InitializeInternal(
      ProfileParams* profile_params,
      content::ProtocolHandlerMap* protocol_handlers) const = 0;

  
  virtual void InitializeExtensionsRequestContext(
      ProfileParams* profile_params) const = 0;
  
  
  virtual ChromeURLRequestContext* InitializeAppRequestContext(
      ChromeURLRequestContext* main_context,
      const StoragePartitionDescriptor& details,
      scoped_ptr<ProtocolHandlerRegistry::JobInterceptorFactory>
          protocol_handler_interceptor,
      content::ProtocolHandlerMap* protocol_handlers) const = 0;

  
  
  virtual ChromeURLRequestContext* InitializeMediaRequestContext(
      ChromeURLRequestContext* original_context,
      const StoragePartitionDescriptor& details) const = 0;

  
  
  virtual ChromeURLRequestContext*
      AcquireMediaRequestContext() const = 0;
  virtual ChromeURLRequestContext*
      AcquireIsolatedAppRequestContext(
          ChromeURLRequestContext* main_context,
          const StoragePartitionDescriptor& partition_descriptor,
          scoped_ptr<ProtocolHandlerRegistry::JobInterceptorFactory>
              protocol_handler_interceptor,
          content::ProtocolHandlerMap* protocol_handlers) const = 0;
  virtual ChromeURLRequestContext*
      AcquireIsolatedMediaRequestContext(
          ChromeURLRequestContext* app_context,
          const StoragePartitionDescriptor& partition_descriptor) const = 0;

  
  virtual chrome_browser_net::LoadTimeStats* GetLoadTimeStats(
      IOThread::Globals* io_thread_globals) const = 0;

  
  
  
  
  
  
  
  
  
  
  
  

  
  mutable bool initialized_;

  
  
  mutable scoped_ptr<ProfileParams> profile_params_;

  
  mutable scoped_ptr<SigninNamesOnIOThread> signin_names_;

  mutable StringPrefMember google_services_user_account_id_;
  mutable StringPrefMember google_services_username_;
  mutable StringPrefMember google_services_username_pattern_;
  mutable BooleanPrefMember reverse_autologin_enabled_;

  
  
  std::string reverse_autologin_pending_email_;

  mutable StringListPrefMember one_click_signin_rejected_email_list_;

  mutable scoped_refptr<MediaDeviceIDSalt> media_device_id_salt_;

  
  mutable BooleanPrefMember enable_referrers_;
  mutable BooleanPrefMember enable_do_not_track_;
  mutable BooleanPrefMember force_safesearch_;
  mutable BooleanPrefMember safe_browsing_enabled_;
  mutable BooleanPrefMember printing_enabled_;
  mutable BooleanPrefMember sync_disabled_;
  mutable BooleanPrefMember signin_allowed_;
  
  mutable IntegerPrefMember session_startup_pref_;

  
  
  
#if defined(OS_CHROMEOS)
  bool enable_metrics_;
#else
  BooleanPrefMember enable_metrics_;
#endif

  
  mutable scoped_ptr<policy::URLBlacklistManager> url_blacklist_manager_;

#if defined(ENABLE_CONFIGURATION_POLICY)
  mutable scoped_ptr<policy::PolicyHeaderIOHelper> policy_header_helper_;
#endif

  
  mutable scoped_refptr<extensions::InfoMap> extension_info_map_;
  mutable scoped_ptr<net::ServerBoundCertService> server_bound_cert_service_;
  mutable scoped_ptr<ChromeNetworkDelegate> network_delegate_;
  mutable scoped_ptr<net::FraudulentCertificateReporter>
      fraudulent_certificate_reporter_;
  mutable scoped_ptr<net::ProxyService> proxy_service_;
  mutable scoped_ptr<net::TransportSecurityState> transport_security_state_;
  mutable scoped_ptr<net::HttpServerProperties>
      http_server_properties_;
#if defined(OS_CHROMEOS)
  mutable scoped_ptr<policy::PolicyCertVerifier> cert_verifier_;
  mutable std::string username_hash_;
#endif

  mutable scoped_ptr<net::TransportSecurityPersister>
      transport_security_persister_;

  
  
  mutable scoped_ptr<ChromeURLRequestContext> main_request_context_;
  mutable scoped_ptr<ChromeURLRequestContext> extensions_request_context_;
  
  mutable URLRequestContextMap app_request_context_map_;
  mutable URLRequestContextMap isolated_media_request_context_map_;

  mutable scoped_ptr<ResourceContext> resource_context_;

  mutable scoped_refptr<CookieSettings> cookie_settings_;

  mutable scoped_refptr<HostContentSettingsMap> host_content_settings_map_;

  mutable scoped_ptr<chrome_browser_net::ResourcePrefetchPredictorObserver>
      resource_prefetch_predictor_observer_;

  mutable scoped_ptr<ChromeHttpUserAgentSettings>
      chrome_http_user_agent_settings_;

  mutable chrome_browser_net::LoadTimeStats* load_time_stats_;

#if defined(ENABLE_MANAGED_USERS)
  mutable scoped_refptr<const ManagedModeURLFilter> managed_mode_url_filter_;
#endif

  
  bool initialized_on_UI_thread_;

  bool is_incognito_;

  DISALLOW_COPY_AND_ASSIGN(ProfileIOData);
};

#endif  
