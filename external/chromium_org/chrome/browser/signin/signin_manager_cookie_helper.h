// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_SIGNIN_MANAGER_COOKIE_HELPER_H_
#define CHROME_BROWSER_SIGNIN_SIGNIN_MANAGER_COOKIE_HELPER_H_

#include "base/callback_helpers.h"
#include "base/memory/ref_counted.h"
#include "net/cookies/canonical_cookie.h"
#include "net/url_request/url_request_context_getter.h"

namespace net {
class URLRequestContextGetter;
}

class SigninManagerCookieHelper
    : public base::RefCountedThreadSafe<SigninManagerCookieHelper> {
 public:
  explicit SigninManagerCookieHelper(
      net::URLRequestContextGetter* request_context_getter);

  
  
  void StartFetchingGaiaCookiesOnUIThread(
      const base::Callback<void(const net::CookieList& cookies)>& callback);

  
  void StartFetchingCookiesOnUIThread(
      const GURL& url,
      const base::Callback<void(const net::CookieList& cookies)>& callback);

 private:
  friend class base::RefCountedThreadSafe<SigninManagerCookieHelper>;
  ~SigninManagerCookieHelper();

  
  void FetchCookiesOnIOThread(const GURL& url);

  
  void OnCookiesFetched(const net::CookieList& cookies);

  
  void NotifyOnUIThread(const net::CookieList& cookies);

  scoped_refptr<net::URLRequestContextGetter> request_context_getter_;
  
  base::Callback<void(const net::CookieList& cookies)> completion_callback_;

  DISALLOW_COPY_AND_ASSIGN(SigninManagerCookieHelper);
};

#endif  
