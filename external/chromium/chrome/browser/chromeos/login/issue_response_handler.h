// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_ISSUE_RESPONSE_HANDLER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_ISSUE_RESPONSE_HANDLER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "chrome/browser/chromeos/login/auth_response_handler.h"

namespace net {
class URLRequestContextGetter;
}

namespace chromeos {

class IssueResponseHandler : public AuthResponseHandler {
 public:
  explicit IssueResponseHandler(net::URLRequestContextGetter* getter)
      : getter_(getter) {}
  virtual ~IssueResponseHandler() {}

  
  virtual bool CanHandle(const GURL& url);

  
  
  
  
  
  virtual URLFetcher* Handle(const std::string& to_process,
                             URLFetcher::Delegate* catcher);

  
  std::string token_url() { return token_url_; }

 private:
  std::string token_url_;
  net::URLRequestContextGetter* getter_;
  DISALLOW_COPY_AND_ASSIGN(IssueResponseHandler);
};

}  

#endif  
