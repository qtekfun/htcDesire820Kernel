// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_COOKIES_CANONICAL_COOKIE_H_
#define NET_COOKIES_CANONICAL_COOKIE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "net/base/net_export.h"
#include "net/cookies/cookie_constants.h"
#include "net/cookies/cookie_options.h"

class GURL;

namespace net {

class ParsedCookie;

class NET_EXPORT CanonicalCookie {
 public:
  
  
  
  
  CanonicalCookie();
  CanonicalCookie(const GURL& url,
                  const std::string& name,
                  const std::string& value,
                  const std::string& domain,
                  const std::string& path,
                  const base::Time& creation,
                  const base::Time& expiration,
                  const base::Time& last_access,
                  bool secure,
                  bool httponly,
                  CookiePriority priority);

  
  
  
  CanonicalCookie(const GURL& url, const ParsedCookie& pc);

  ~CanonicalCookie();

  

  
  
  
  static CanonicalCookie* Create(const GURL& url,
                                 const std::string& cookie_line,
                                 const base::Time& creation_time,
                                 const CookieOptions& options);

  
  
  
  static CanonicalCookie* Create(const GURL& url,
                                 const std::string& name,
                                 const std::string& value,
                                 const std::string& domain,
                                 const std::string& path,
                                 const base::Time& creation,
                                 const base::Time& expiration,
                                 bool secure,
                                 bool http_only,
                                 CookiePriority priority);

  const std::string& Source() const { return source_; }
  const std::string& Name() const { return name_; }
  const std::string& Value() const { return value_; }
  const std::string& Domain() const { return domain_; }
  const std::string& Path() const { return path_; }
  const base::Time& CreationDate() const { return creation_date_; }
  const base::Time& LastAccessDate() const { return last_access_date_; }
  bool IsPersistent() const { return !expiry_date_.is_null(); }
  const base::Time& ExpiryDate() const { return expiry_date_; }
  bool IsSecure() const { return secure_; }
  bool IsHttpOnly() const { return httponly_; }
  CookiePriority Priority() const { return priority_; }
  bool IsDomainCookie() const {
    return !domain_.empty() && domain_[0] == '.'; }
  bool IsHostCookie() const { return !IsDomainCookie(); }

  bool IsExpired(const base::Time& current) const {
    return !expiry_date_.is_null() && current >= expiry_date_;
  }

  
  
  
  
  
  
  bool IsEquivalent(const CanonicalCookie& ecc) const {
    
    
    
    return (name_ == ecc.Name() && domain_ == ecc.Domain()
            && path_ == ecc.Path());
  }

  void SetLastAccessDate(const base::Time& date) {
    last_access_date_ = date;
  }

  
  
  bool IsOnPath(const std::string& url_path) const;

  
  
  bool IsDomainMatch(const std::string& host) const;

  
  
  
  
  bool IncludeForRequestURL(const GURL& url,
                            const CookieOptions& options) const;

  std::string DebugString() const;

  
  
  static std::string GetCookieSourceFromURL(const GURL& url);
  static std::string CanonPath(const GURL& url, const ParsedCookie& pc);
  static base::Time CanonExpiration(const ParsedCookie& pc,
                                    const base::Time& current,
                                    const base::Time& server_time);

 private:
  
  
  
  
  
  
  
  
  std::string source_;
  std::string name_;
  std::string value_;
  std::string domain_;
  std::string path_;
  base::Time creation_date_;
  base::Time expiry_date_;
  base::Time last_access_date_;
  bool secure_;
  bool httponly_;
  CookiePriority priority_;
};

typedef std::vector<CanonicalCookie> CookieList;

}  

#endif  
