// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TEST_EMBEDDED_TEST_SERVER_HTTP_RESPONSE_H_
#define NET_TEST_EMBEDDED_TEST_SERVER_HTTP_RESPONSE_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "net/http/http_status_code.h"

namespace net {
namespace test_server {

class HttpResponse{
 public:
  virtual ~HttpResponse();

  // Returns raw contents to be written to the network socket
  
  
  virtual std::string ToResponseString() const = 0;
};

class BasicHttpResponse : public HttpResponse {
 public:
  BasicHttpResponse();
  virtual ~BasicHttpResponse();

  
  HttpStatusCode code() const { return code_; }
  void set_code(HttpStatusCode code) { code_ = code; }

  
  const std::string& content() const { return content_; }
  void set_content(const std::string& content) { content_ = content; }

  
  const std::string& content_type() const { return content_type_; }
  void set_content_type(const std::string& content_type) {
    content_type_ = content_type;
  }

  
  void AddCustomHeader(const std::string& key, const std::string& value) {
    custom_headers_.push_back(std::make_pair(key, value));
  }

  
  virtual std::string ToResponseString() const OVERRIDE;

 private:
  HttpStatusCode code_;
  std::string content_;
  std::string content_type_;
  std::vector<std::pair<std::string, std::string> > custom_headers_;

  DISALLOW_COPY_AND_ASSIGN(BasicHttpResponse);
};

}  
}  

#endif  
