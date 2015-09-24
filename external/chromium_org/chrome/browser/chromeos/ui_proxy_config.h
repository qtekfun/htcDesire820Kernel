// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_UI_PROXY_CONFIG_H_
#define CHROME_BROWSER_CHROMEOS_UI_PROXY_CONFIG_H_

#include <string>

#include "chrome/browser/prefs/proxy_prefs.h"
#include "net/proxy/proxy_bypass_rules.h"
#include "net/proxy/proxy_server.h"
#include "url/gurl.h"

namespace base {
class DictionaryValue;
}

namespace net {
class ProxyConfig;
}

namespace chromeos {

struct UIProxyConfig {
  
  
  enum Mode {
    MODE_DIRECT,
    MODE_AUTO_DETECT,
    MODE_PAC_SCRIPT,
    MODE_SINGLE_PROXY,
    MODE_PROXY_PER_SCHEME,
  };

  
  struct AutomaticProxy {
    GURL pac_url;  
  };

  
  struct ManualProxy {
    net::ProxyServer server;
  };

  UIProxyConfig();
  ~UIProxyConfig();

  void SetPacUrl(const GURL& pac_url);
  void SetSingleProxy(const net::ProxyServer& server);

  
  void SetProxyForScheme(const std::string& scheme,
                         const net::ProxyServer& server);

  
  void SetBypassRules(const net::ProxyBypassRules& rules);

  
  bool FromNetProxyConfig(const net::ProxyConfig& net_config);

  
  
  base::DictionaryValue* ToPrefProxyConfig() const;

  
  
  ManualProxy* MapSchemeToProxy(const std::string& scheme);

  
  static void EncodeAndAppendProxyServer(const std::string& url_scheme,
                                         const net::ProxyServer& server,
                                         std::string* spec);

  Mode mode;

  ProxyPrefs::ConfigState state;

  
  
  
  
  bool user_modifiable;

  
  AutomaticProxy  automatic_proxy;
  
  ManualProxy     single_proxy;
  
  ManualProxy     http_proxy;
  
  ManualProxy     https_proxy;
  
  ManualProxy     ftp_proxy;
  
  ManualProxy     socks_proxy;

  
  net::ProxyBypassRules bypass_rules;
};

}  

#endif  
