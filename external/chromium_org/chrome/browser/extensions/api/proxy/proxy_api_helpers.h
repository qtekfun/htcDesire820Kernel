// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_API_PROXY_PROXY_API_HELPERS_H_
#define CHROME_BROWSER_EXTENSIONS_API_PROXY_PROXY_API_HELPERS_H_

#include <string>

#include "chrome/browser/prefs/proxy_prefs.h"
#include "net/proxy/proxy_config.h"

class ProxyConfigDictionary;

namespace base {
class DictionaryValue;
class ListValue;
}

namespace extensions {
namespace proxy_api_helpers {

bool CreateDataURLFromPACScript(const std::string& pac_script,
                                std::string* pac_script_url_base64_encoded);
bool CreatePACScriptFromDataURL(
    const std::string& pac_script_url_base64_encoded,
    std::string* pac_script);


bool GetProxyModeFromExtensionPref(const base::DictionaryValue* proxy_config,
                                   ProxyPrefs::ProxyMode* out,
                                   std::string* error,
                                   bool* bad_message);
bool GetPacMandatoryFromExtensionPref(const base::DictionaryValue* proxy_config,
                                      bool* out,
                                      std::string* error,
                                      bool* bad_message);
bool GetPacUrlFromExtensionPref(const base::DictionaryValue* proxy_config,
                                std::string* out,
                                std::string* error,
                                bool* bad_message);
bool GetPacDataFromExtensionPref(const base::DictionaryValue* proxy_config,
                                 std::string* out,
                                 std::string* error,
                                 bool* bad_message);
bool GetProxyRulesStringFromExtensionPref(
    const base::DictionaryValue* proxy_config,
    std::string* out,
    std::string* error,
    bool* bad_message);
bool GetBypassListFromExtensionPref(const base::DictionaryValue* proxy_config,
                                    std::string* out,
                                    std::string* error,
                                    bool* bad_message);

base::DictionaryValue* CreateProxyConfigDict(
    ProxyPrefs::ProxyMode mode_enum,
    bool pac_mandatory,
    const std::string& pac_url,
    const std::string& pac_data,
    const std::string& proxy_rules_string,
    const std::string& bypass_list,
    std::string* error);

bool GetProxyServer(const base::DictionaryValue* proxy_server,
                    net::ProxyServer::Scheme default_scheme,
                    net::ProxyServer* out,
                    std::string* error,
                    bool* bad_message);

bool JoinUrlList(const base::ListValue* list,
                 const std::string& joiner,
                 std::string* out,
                 std::string* error,
                 bool* bad_message);



base::DictionaryValue* CreateProxyRulesDict(
    const ProxyConfigDictionary& proxy_config);

base::DictionaryValue* CreateProxyServerDict(const net::ProxyServer& proxy);

base::DictionaryValue* CreatePacScriptDict(
    const ProxyConfigDictionary& proxy_config);

base::ListValue* TokenizeToStringList(const std::string& in,
                                      const std::string& delims);

}  
}  

#endif  
