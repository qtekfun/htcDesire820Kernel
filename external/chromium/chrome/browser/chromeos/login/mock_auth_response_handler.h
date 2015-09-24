// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_MOCK_AUTH_RESPONSE_HANDLER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_MOCK_AUTH_RESPONSE_HANDLER_H_
#pragma once

#include "chrome/browser/chromeos/login/auth_response_handler.h"

#include <string>

#include "googleurl/src/gurl.h"
#include "net/url_request/url_request_status.h"
#include "testing/gmock/include/gmock/gmock.h"

class URLFetcher;

namespace chromeos {

class MockAuthResponseHandler : public AuthResponseHandler {
 public:
  MockAuthResponseHandler(const GURL& url,
                          const net::URLRequestStatus& status,
                          const int code,
                          const std::string& data);
  virtual ~MockAuthResponseHandler() {}

  MOCK_METHOD1(CanHandle, bool(const GURL& url));
  MOCK_METHOD2(Handle, URLFetcher*(const std::string& to_process,
                                   URLFetcher::Delegate* catcher));

  URLFetcher* MockNetwork(std::string data, URLFetcher::Delegate* delegate);

 private:
  const GURL remote_;
  const net::URLRequestStatus status_;
  const int http_response_code_;
  const std::string data_;

  static void CompleteFetch(URLFetcher::Delegate* delegate,
                            const GURL remote,
                            const net::URLRequestStatus status,
                            const int http_response_code,
                            const std::string data);

  DISALLOW_COPY_AND_ASSIGN(MockAuthResponseHandler);
};

}  

#endif  
