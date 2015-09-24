// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_HTTP_USER_AGENT_SETTINGS_H_
#define NET_URL_REQUEST_HTTP_USER_AGENT_SETTINGS_H_

#include <string>

#include "base/basictypes.h"
#include "net/base/net_export.h"

class GURL;

namespace net {

class NET_EXPORT HttpUserAgentSettings {
 public:
  HttpUserAgentSettings() {}
  virtual ~HttpUserAgentSettings() {}

  
  virtual std::string GetAcceptLanguage() const = 0;

  
  
  virtual std::string GetUserAgent(const GURL& url) const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(HttpUserAgentSettings);
};

}  

#endif  

