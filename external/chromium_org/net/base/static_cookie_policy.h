// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_STATIC_COOKIE_POLICY_H_
#define NET_BASE_STATIC_COOKIE_POLICY_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"

class GURL;

namespace net {

class NET_EXPORT StaticCookiePolicy {
 public:
  
  
  enum Type {
    
    ALLOW_ALL_COOKIES = 0,
    
    BLOCK_SETTING_THIRD_PARTY_COOKIES,
    
    BLOCK_ALL_COOKIES,
    
    BLOCK_ALL_THIRD_PARTY_COOKIES
  };

  StaticCookiePolicy()
      : type_(StaticCookiePolicy::ALLOW_ALL_COOKIES) {
  }

  explicit StaticCookiePolicy(Type type)
      : type_(type) {
  }

  
  
  void set_type(Type type) { type_ = type; }
  Type type() const { return type_; }

  
  
  int CanGetCookies(const GURL& url, const GURL& first_party_for_cookies) const;

  
  
  int CanSetCookie(const GURL& url,
                   const GURL& first_party_for_cookies) const;

 private:
  Type type_;

  DISALLOW_COPY_AND_ASSIGN(StaticCookiePolicy);
};

}  

#endif  
