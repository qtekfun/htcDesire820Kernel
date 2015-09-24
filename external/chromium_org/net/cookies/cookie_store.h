// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_COOKIES_COOKIE_STORE_H_
#define NET_COOKIES_COOKIE_STORE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "net/base/net_export.h"
#include "net/cookies/cookie_options.h"

class GURL;

namespace net {

class CookieMonster;

class NET_EXPORT CookieStore : public base::RefCountedThreadSafe<CookieStore> {
 public:
  
  typedef base::Callback<void(const std::string& cookie)>
      GetCookiesCallback;
  typedef base::Callback<void(bool success)> SetCookiesCallback;
  typedef base::Callback<void(int num_deleted)> DeleteCallback;


  
  
  
  
  
  virtual void SetCookieWithOptionsAsync(
      const GURL& url,
      const std::string& cookie_line,
      const CookieOptions& options,
      const SetCookiesCallback& callback) = 0;

  
  
  
  
  
  
  virtual void GetCookiesWithOptionsAsync(
      const GURL& url,
      const CookieOptions& options,
      const GetCookiesCallback& callback) = 0;

  
  virtual void DeleteCookieAsync(const GURL& url,
                                 const std::string& cookie_name,
                                 const base::Closure& callback) = 0;

  
  
  
  virtual void DeleteAllCreatedBetweenAsync(const base::Time& delete_begin,
                                            const base::Time& delete_end,
                                            const DeleteCallback& callback) = 0;

  virtual void DeleteSessionCookiesAsync(const DeleteCallback&) = 0;

  
  virtual CookieMonster* GetCookieMonster() = 0;

 protected:
  friend class base::RefCountedThreadSafe<CookieStore>;
  CookieStore();
  virtual ~CookieStore();
};

}  

#endif  
