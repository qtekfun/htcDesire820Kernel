// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_PROXY_API_HELPERS_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_PROXY_API_HELPERS_H_
#pragma once

#include <string>

#include "chrome/browser/prefs/proxy_prefs.h"
#include "net/proxy/proxy_config.h"

class DictionaryValue;
class ListValue;
class ProxyConfigDictionary;

namespace extension_proxy_api_helpers {

bool CreateDataURLFromPACScript(const std::string& pac_script,
                                std::string* pac_script_url_base64_encoded);
bool CreatePACScriptFromDataURL(
    const std::string& pac_script_url_base64_encoded,
    std::string* pac_script);


bool GetProxyModeFromExtensionPref(const DictionaryValue* proxy_config,
                                   ProxyPrefs::ProxyMode* out,
                                   std::string* error);
bool GetPacUrlFromExtensionPref(const DictionaryValue* proxy_config,
                                std::string* out,
                                std::string* error);
bool GetPacDataFromExtensionPref(const DictionaryValue* proxy_config,
                                 std::string* out,
                                 std::string* error);
bool GetProxyRulesStringFromExtensionPref(const DictionaryValue* proxy_config,
                                          std::string* out,
                                          std::string* error);
bool GetBypassListFromExtensionPref(const DictionaryValue* proxy_config,
                                    std::string* out,
                                    std::string* error);

DictionaryValue* CreateProxyConfigDict(ProxyPrefs::ProxyMode mode_enum,
                                       const std::string& pac_url,
                                       const std::string& pac_data,
                                       const std::string& proxy_rules_string,
                                       const std::string& bypass_list,
                                       std::string* error);

bool GetProxyServer(const DictionaryValue* proxy_server,
                    net::ProxyServer::Scheme default_scheme,
                    net::ProxyServer* out,
                    std::string* error);

bool JoinUrlList(ListValue* list,
                 const std::string& joiner,
                 std::string* out,
                 std::string* error);



DictionaryValue* CreateProxyRulesDict(
    const ProxyConfigDictionary& proxy_config);

DictionaryValue* CreateProxyServerDict(const net::ProxyServer& proxy);

DictionaryValue* CreatePacScriptDict(const ProxyConfigDictionary& proxy_config);

ListValue* TokenizeToStringList(const std::string& in,
                                const std::string& delims);

}  

#endif  
