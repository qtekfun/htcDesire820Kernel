// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_COOKIE_FETCHER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_COOKIE_FETCHER_H_
#pragma once

#include <string>
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/login/auth_response_handler.h"
#include "chrome/browser/chromeos/login/client_login_response_handler.h"
#include "chrome/browser/chromeos/login/issue_response_handler.h"
#include "chrome/common/net/url_fetcher.h"

class Profile;

namespace chromeos {

class CookieFetcher : public URLFetcher::Delegate {
 public:
  
  explicit CookieFetcher(Profile* profile);

  
  
  CookieFetcher(Profile* profile,
                AuthResponseHandler* cl_handler,
                AuthResponseHandler* i_handler)
      : profile_(profile),
        client_login_handler_(cl_handler),
        issue_handler_(i_handler) {
  }

  
  
  
  
  
  void AttemptFetch(const std::string& credentials);

  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

 private:
  virtual ~CookieFetcher() {}

  scoped_ptr<URLFetcher> fetcher_;
  Profile* profile_;
  scoped_ptr<AuthResponseHandler> client_login_handler_;
  scoped_ptr<AuthResponseHandler> issue_handler_;

  DISALLOW_COPY_AND_ASSIGN(CookieFetcher);
};

}  

#endif  
