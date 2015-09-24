// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_PROXY_CONFIG_SERVICE_IMPL_H_
#define CHROME_BROWSER_CHROMEOS_PROXY_CONFIG_SERVICE_IMPL_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/values.h"
#include "chrome/browser/chromeos/login/signed_settings.h"
#include "net/proxy/proxy_config.h"
#include "net/proxy/proxy_config_service.h"
#include "net/proxy/proxy_server.h"

namespace chromeos {

class ProxyConfigServiceImpl
    : public base::RefCountedThreadSafe<ProxyConfigServiceImpl>,
      public SignedSettings::Delegate<bool>,
      public SignedSettings::Delegate<std::string> {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  
  
  
  
  
  
  
  
  
  struct ProxyConfig {
    
    
    enum Mode {
      MODE_DIRECT,
      MODE_AUTO_DETECT,
      MODE_PAC_SCRIPT,
      MODE_SINGLE_PROXY,
      MODE_PROXY_PER_SCHEME,
    };

    
    enum Source {
      SOURCE_NONE,    
      SOURCE_POLICY,  
      SOURCE_OWNER,   
    };

    struct Setting {
      Setting() : source(SOURCE_NONE) {}
      bool CanBeWrittenByUser(bool user_is_owner);

      Source source;
    };

    
    struct AutomaticProxy : public Setting {
      GURL    pac_url;  
    };

    
    struct ManualProxy : public Setting {
      net::ProxyServer  server;
    };

    ProxyConfig() : mode(MODE_DIRECT) {}

    
    void ToNetProxyConfig(net::ProxyConfig* net_config);

    // Returns true if proxy config can be written by user.
    
    
    bool CanBeWrittenByUser(bool user_is_owner, const std::string& scheme);

    
    
    ManualProxy* MapSchemeToProxy(const std::string& scheme);

    
    
    bool Serialize(std::string* output);
    
    
    bool Deserialize(const std::string& input);

    
    std::string ToString() const;

    Mode mode;

    
    AutomaticProxy  automatic_proxy;
    
    ManualProxy     single_proxy;
    
    ManualProxy     http_proxy;
    
    ManualProxy     https_proxy;
    
    ManualProxy     ftp_proxy;
    
    ManualProxy     socks_proxy;

    
    net::ProxyBypassRules  bypass_rules;

   private:
    
    static void EncodeAndAppendProxyServer(const std::string& scheme,
                                           const net::ProxyServer& server,
                                           std::string* spec);
  };

  
  ProxyConfigServiceImpl();
  
  
  explicit ProxyConfigServiceImpl(const ProxyConfig& init_config);
  virtual ~ProxyConfigServiceImpl();

  
  
  void AddObserver(net::ProxyConfigService::Observer* observer);
  void RemoveObserver(net::ProxyConfigService::Observer* observer);
  
  net::ProxyConfigService::ConfigAvailability IOGetProxyConfig(
      net::ProxyConfig* config);

  
  void UIGetProxyConfig(ProxyConfig* config);

  
  
  
  void UISetPersistToDevice(bool persist) {
    persist_to_device_ = persist;
  }

  
  
  
  
  
  
  
  bool UISetProxyConfigToDirect();
  bool UISetProxyConfigToAutoDetect();
  bool UISetProxyConfigToPACScript(const GURL& pac_url);
  bool UISetProxyConfigToSingleProxy(const net::ProxyServer& server);
  
  bool UISetProxyConfigToProxyPerScheme(const std::string& scheme,
                                        const net::ProxyServer& server);
  
  bool UISetProxyConfigBypassRules(const net::ProxyBypassRules& bypass_rules);

  
  virtual void OnSettingsOpCompleted(SignedSettings::ReturnCode code,
                                     std::string value);
  virtual void OnSettingsOpCompleted(SignedSettings::ReturnCode code,
                                     bool value);

 private:
  friend class base::RefCountedThreadSafe<ProxyConfigServiceImpl>;

  
  void PersistConfigToDevice();

  
  
  void OnUISetProxyConfig(bool update_to_device);

  
  void IOSetProxyConfig(
      const ProxyConfig& new_config,
      net::ProxyConfigService::ConfigAvailability new_availability);

  
  void CheckCurrentlyOnIOThread();

  
  void CheckCurrentlyOnUIThread();

  

  
  
  
  bool can_post_task_;

  
  net::ProxyConfigService::ConfigAvailability config_availability_;

  
  bool persist_to_device_;

  
  bool persist_to_device_pending_;

  
  
  
  
  ProxyConfig cached_config_;

  
  
  
  ProxyConfig reference_config_;

  
  ObserverList<net::ProxyConfigService::Observer> observers_;

  
  
  scoped_refptr<SignedSettings> retrieve_property_op_;
  scoped_refptr<SignedSettings> store_property_op_;

  DISALLOW_COPY_AND_ASSIGN(ProxyConfigServiceImpl);
};

}  

#endif  
