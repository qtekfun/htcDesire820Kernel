// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_BASE_COOKIE_STORE_H_
#define NET_BASE_COOKIE_STORE_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/time.h"
#include "net/base/cookie_options.h"
#include "net/base/net_export.h"

class GURL;

namespace net {

class CookieMonster;

class NET_EXPORT CookieStore : public base::RefCountedThreadSafe<CookieStore> {
 public:
  
  virtual bool SetCookieWithOptions(const GURL& url,
                                    const std::string& cookie_line,
                                    const CookieOptions& options) = 0;

  
  
  
  
  virtual std::string GetCookiesWithOptions(const GURL& url,
                                            const CookieOptions& options) = 0;

  
  virtual void DeleteCookie(const GURL& url,
                            const std::string& cookie_name) = 0;

  
  virtual CookieMonster* GetCookieMonster() = 0;


  
  

  
  bool SetCookie(const GURL& url, const std::string& cookie_line);

  
  std::string GetCookies(const GURL& url);

  
  void SetCookiesWithOptions(const GURL& url,
                             const std::vector<std::string>& cookie_lines,
                             const CookieOptions& options);
  void SetCookies(const GURL& url,
                  const std::vector<std::string>& cookie_lines);

 protected:
  friend class base::RefCountedThreadSafe<CookieStore>;
  CookieStore();
  virtual ~CookieStore();
};

}  

#endif  
