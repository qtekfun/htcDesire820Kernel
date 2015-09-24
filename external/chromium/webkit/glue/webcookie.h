// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_WEBCOOKIE_H_
#define WEBKIT_GLUE_WEBCOOKIE_H_

#include "net/base/cookie_monster.h"

namespace webkit_glue {

struct WebCookie {
  WebCookie();
  explicit WebCookie(const net::CookieMonster::CanonicalCookie& c);
  WebCookie(const std::string& name, const std::string& value,
            const std::string& domain, const std::string& path, double expires,
            bool http_only, bool secure, bool session);
  ~WebCookie();

  
  std::string name;

  
  std::string value;

  
  std::string domain;

  
  std::string path;

  
  double expires;

  
  bool http_only;

  
  bool secure;

  
  bool session;
};

}  

#endif  
