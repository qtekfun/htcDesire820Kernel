// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_API_PROXY_PROXY_API_CONSTANTS_H_
#define CHROME_BROWSER_EXTENSIONS_API_PROXY_PROXY_API_CONSTANTS_H_

namespace extensions {
namespace proxy_api_constants {

extern const char kProxyConfigMode[];
extern const char kProxyConfigPacScript[];
extern const char kProxyConfigPacScriptMandatory[];
extern const char kProxyConfigPacScriptUrl[];
extern const char kProxyConfigPacScriptData[];
extern const char kProxyConfigRules[];
extern const char kProxyConfigRuleHost[];
extern const char kProxyConfigRulePort[];
extern const char kProxyConfigRuleScheme[];
extern const char kProxyConfigBypassList[];

extern const char kProxyEventFatal[];
extern const char kProxyEventError[];
extern const char kProxyEventDetails[];
extern const char kProxyEventOnProxyError[];

extern const char kPACDataUrlPrefix[];

enum {
  SCHEME_ALL = 0,
  SCHEME_HTTP,
  SCHEME_HTTPS,
  SCHEME_FTP,
  SCHEME_FALLBACK,
  SCHEME_MAX = SCHEME_FALLBACK  
};

extern const char* field_name[];

extern const char* scheme_name[];

}  
}  

#endif  
