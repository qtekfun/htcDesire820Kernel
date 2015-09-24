// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_CHROME_HTTP_USER_AGENT_SETTINGS_H_
#define CHROME_BROWSER_NET_CHROME_HTTP_USER_AGENT_SETTINGS_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/prefs/pref_member.h"
#include "net/url_request/http_user_agent_settings.h"

class PrefService;

class ChromeHttpUserAgentSettings : public net::HttpUserAgentSettings {
 public:
  
  explicit ChromeHttpUserAgentSettings(PrefService* prefs);
  
  virtual ~ChromeHttpUserAgentSettings();

  void CleanupOnUIThread();

  
  virtual std::string GetAcceptLanguage() const OVERRIDE;
  virtual std::string GetUserAgent(const GURL& url) const OVERRIDE;

 private:
  StringPrefMember pref_accept_language_;

  
  
  mutable std::string last_pref_accept_language_;
  mutable std::string last_http_accept_language_;

  DISALLOW_COPY_AND_ASSIGN(ChromeHttpUserAgentSettings);
};

#endif  

