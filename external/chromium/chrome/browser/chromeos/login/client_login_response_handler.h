// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_CLIENT_LOGIN_RESPONSE_HANDLER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_CLIENT_LOGIN_RESPONSE_HANDLER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "chrome/browser/chromeos/login/auth_response_handler.h"

namespace net {
class URLRequestContextGetter;
}

namespace chromeos {

class ClientLoginResponseHandler : public AuthResponseHandler {
 public:
  explicit ClientLoginResponseHandler(net::URLRequestContextGetter* getter)
      : getter_(getter) {}
  ~ClientLoginResponseHandler() {}

  
  virtual bool CanHandle(const GURL& url);

  
  
  
  
  virtual URLFetcher* Handle(const std::string& to_process,
                             URLFetcher::Delegate* catcher);

  
  std::string payload() { return payload_; }

  
  
  static const char kService[];
 private:
  std::string payload_;
  net::URLRequestContextGetter* getter_;

  DISALLOW_COPY_AND_ASSIGN(ClientLoginResponseHandler);
};

}  

#endif  
