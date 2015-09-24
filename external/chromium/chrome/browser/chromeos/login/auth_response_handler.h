// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_AUTH_RESPONSE_HANDLER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_AUTH_RESPONSE_HANDLER_H_
#pragma once

#include <string>

#include "chrome/common/net/url_fetcher.h"

class GURL;

namespace chromeos {

extern const int kHttpSuccess;

class AuthResponseHandler {
 public:
  AuthResponseHandler() {}
  virtual ~AuthResponseHandler() {}

  
  virtual bool CanHandle(const GURL& url) = 0;

  
  
  
  
  
  virtual URLFetcher* Handle(const std::string& to_process,
                             URLFetcher::Delegate* catcher) = 0;

  
  static const char kClientLoginUrl[];
  static const char kIssueAuthTokenUrl[];
  static const char kTokenAuthUrl[];
};

}  

#endif  
