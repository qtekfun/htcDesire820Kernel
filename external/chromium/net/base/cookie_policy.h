// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_COOKIE_POLICY_H_
#define NET_BASE_COOKIE_POLICY_H_
#pragma once

#include <string>

#include "net/base/completion_callback.h"

class GURL;

namespace net {

enum {
  OK_FOR_SESSION_ONLY = 1,  
};

class CookiePolicy {
 public:
  virtual ~CookiePolicy() {}

  
  
  
  
  
  virtual int CanGetCookies(const GURL& url,
                            const GURL& first_party_for_cookies) const = 0;

  // Determines if the URL's cookies may be written.
  
  
  
  
  
  
  virtual int CanSetCookie(const GURL& url,
                           const GURL& first_party_for_cookies,
                           const std::string& cookie_line) const = 0;
};

}  

#endif 
