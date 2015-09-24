// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILE_IO_DATA_H_
#define CHROME_BROWSER_PROFILES_PROFILE_IO_DATA_H_
#pragma once

#include <set>
#include "base/basictypes.h"
#include "base/debug/stack_trace.h"
#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/net/chrome_url_request_context.h"
#include "chrome/browser/prefs/pref_member.h"
#include "chrome/browser/profiles/profile.h"
#include "content/browser/resource_context.h"
#include "net/base/cookie_monster.h"

class CommandLine;
class ChromeAppCacheService;
class ChromeBlobStorageContext;
class ExtensionInfoMap;
namespace fileapi {
class FileSystemContext;
}  
class HostContentSettingsMap;
class HostZoomMap;
class IOThread;
namespace net {
class DnsCertProvenanceChecker;
class NetLog;
class ProxyConfigService;
class ProxyService;
class SSLConfigService;
class TransportSecurityState;
}  
namespace prerender {
class PrerenderManager;
};  
class ProtocolHandlerRegistry;
namespace webkit_database {
class DatabaseTracker;
}  

class ProfileIOData : public base::RefCountedThreadSafe<ProfileIOData> {
 public:
  
  
  
  scoped_refptr<ChromeURLRequestContext> GetMainRequestContext() const;
  scoped_refptr<ChromeURLRequestContext> GetMediaRequestContext() const;
  scoped_refptr<ChromeURLRequestContext> GetExtensionsRequestContext() const;
  scoped_refptr<ChromeURLRequestContext> GetIsolatedAppRequestContext(
      scoped_refptr<ChromeURLRequestContext> main_context,
      const std::string& app_id) const;
  const content::ResourceContext& GetResourceContext() const;

 protected:
  friend class base::RefCountedThreadSafe<ProfileIOData>;

  class RequestContext : public ChromeURLRequestContext {
   public:
    RequestContext();
    ~RequestContext();

    
    void set_profile_io_data(const ProfileIOData* profile_io_data) {
      profile_io_data_ = profile_io_data;
    }

   private:
    scoped_refptr<const ProfileIOData> profile_io_data_;
  };

  
  
  struct ProfileParams {
    ProfileParams();
    ~ProfileParams();

    bool is_incognito;
    bool clear_local_state_on_exit;
    std::string accept_language;
    std::string accept_charset;
    std::string referrer_charset;
    FilePath user_script_dir_path;
    IOThread* io_thread;
    scoped_refptr<HostContentSettingsMap> host_content_settings_map;
    scoped_refptr<HostZoomMap> host_zoom_map;
    scoped_refptr<net::TransportSecurityState> transport_security_state;
    scoped_refptr<net::SSLConfigService> ssl_config_service;
    scoped_refptr<net::CookieMonster::Delegate> cookie_monster_delegate;
    scoped_refptr<webkit_database::DatabaseTracker> database_tracker;
    scoped_refptr<ChromeAppCacheService> appcache_service;
    scoped_refptr<ChromeBlobStorageContext> blob_storage_context;
    scoped_refptr<fileapi::FileSystemContext> file_system_context;
    scoped_refptr<ExtensionInfoMap> extension_info_map;
    scoped_refptr<prerender::PrerenderManager> prerender_manager;
    scoped_refptr<ProtocolHandlerRegistry> protocol_handler_registry;
    
    
    
    scoped_ptr<net::ProxyConfigService> proxy_config_service;
    
    ProfileId profile_id;
  };

  explicit ProfileIOData(bool is_incognito);
  virtual ~ProfileIOData();

  void InitializeProfileParams(Profile* profile);
  void ApplyProfileParamsToContext(ChromeURLRequestContext* context) const;

  
  
  
  
  void LazyInitialize() const;

  
  void ShutdownOnUIThread();

  BooleanPrefMember* enable_referrers() const {
    return &enable_referrers_;
  }

  net::NetworkDelegate* network_delegate() const {
    return network_delegate_.get();
  }

  net::DnsCertProvenanceChecker* dns_cert_checker() const {
    return dns_cert_checker_.get();
  }

  net::ProxyService* proxy_service() const {
    return proxy_service_.get();
  }

  net::CookiePolicy* cookie_policy() const {
    return cookie_policy_.get();
  }

  ChromeURLRequestContext* main_request_context() const {
    return main_request_context_;
  }

  ChromeURLRequestContext* extensions_request_context() const {
    return extensions_request_context_;
  }

 private:
  class ResourceContext : public content::ResourceContext {
   public:
    explicit ResourceContext(const ProfileIOData* io_data);
    virtual ~ResourceContext();

   private:
    virtual void EnsureInitialized() const;

    const ProfileIOData* const io_data_;
  };

  
  
  

  
  
  virtual void LazyInitializeInternal(ProfileParams* profile_params) const = 0;

  
  
  virtual scoped_refptr<RequestContext> InitializeAppRequestContext(
      scoped_refptr<ChromeURLRequestContext> main_context,
      const std::string& app_id) const = 0;

  
  
  virtual scoped_refptr<ChromeURLRequestContext>
      AcquireMediaRequestContext() const = 0;
  virtual scoped_refptr<ChromeURLRequestContext>
      AcquireIsolatedAppRequestContext(
          scoped_refptr<ChromeURLRequestContext> main_context,
          const std::string& app_id) const = 0;

  
  mutable bool initialized_;

  
  
  mutable scoped_ptr<ProfileParams> profile_params_;

  
  mutable BooleanPrefMember enable_referrers_;

  
  mutable scoped_ptr<net::NetworkDelegate> network_delegate_;
  mutable scoped_ptr<net::DnsCertProvenanceChecker> dns_cert_checker_;
  mutable scoped_refptr<net::ProxyService> proxy_service_;
  mutable scoped_ptr<net::CookiePolicy> cookie_policy_;

  
  mutable scoped_refptr<webkit_database::DatabaseTracker> database_tracker_;
  mutable scoped_refptr<ChromeAppCacheService> appcache_service_;
  mutable scoped_refptr<ChromeBlobStorageContext> blob_storage_context_;
  mutable scoped_refptr<fileapi::FileSystemContext> file_system_context_;

  mutable ResourceContext resource_context_;

  
  
  mutable scoped_refptr<RequestContext> main_request_context_;
  mutable scoped_refptr<RequestContext> extensions_request_context_;

  DISALLOW_COPY_AND_ASSIGN(ProfileIOData);
};

#endif  
