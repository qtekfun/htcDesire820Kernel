// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SERVER_HTTP_SERVER_RESPONSE_INFO_H_
#define NET_SERVER_HTTP_SERVER_RESPONSE_INFO_H_

#include <string>
#include <utility>
#include <vector>

#include "net/http/http_status_code.h"

namespace net {

class HttpServerResponseInfo {
 public:
  
  HttpServerResponseInfo();
  explicit HttpServerResponseInfo(HttpStatusCode status_code);
  ~HttpServerResponseInfo();

  static HttpServerResponseInfo CreateFor404();
  static HttpServerResponseInfo CreateFor500(const std::string& body);

  void AddHeader(const std::string& name, const std::string& value);

  
  void SetBody(const std::string& body, const std::string& content_type);

  std::string Serialize() const;

  HttpStatusCode status_code() const;
  const std::string& body() const;

 private:
  typedef std::vector<std::pair<std::string, std::string> > Headers;

  HttpStatusCode status_code_;
  Headers headers_;
  std::string body_;
};

}  

#endif  
