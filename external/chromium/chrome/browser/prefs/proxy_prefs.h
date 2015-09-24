// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_PROXY_PREFS_H_
#define CHROME_BROWSER_PREFS_PROXY_PREFS_H_
#pragma once

#include <string>

namespace ProxyPrefs {

enum ProxyMode {
  
  MODE_DIRECT = 0,

  
  
  MODE_AUTO_DETECT = 1,

  
  
  MODE_PAC_SCRIPT = 2,

  
  MODE_FIXED_SERVERS = 3,

  
  
  MODE_SYSTEM = 4,

  kModeCount
};

extern const char kDirectProxyModeName[];
extern const char kAutoDetectProxyModeName[];
extern const char kPacScriptProxyModeName[];
extern const char kFixedServersProxyModeName[];
extern const char kSystemProxyModeName[];

bool IntToProxyMode(int in_value, ProxyMode* out_value);
bool StringToProxyMode(const std::string& in_value,
                       ProxyMode* out_value);
const char* ProxyModeToString(ProxyMode mode);

}  

#endif  
