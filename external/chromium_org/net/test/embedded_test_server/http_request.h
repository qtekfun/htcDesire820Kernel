// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TEST_EMBEDDED_TEST_SERVER_HTTP_REQUEST_H_
#define NET_TEST_EMBEDDED_TEST_SERVER_HTTP_REQUEST_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"

namespace net {
namespace test_server {

enum HttpMethod {
  METHOD_UNKNOWN,
  METHOD_GET,
  METHOD_HEAD,
  METHOD_POST,
  METHOD_PUT,
  METHOD_DELETE,
  METHOD_PATCH,
};

struct HttpRequest {
  HttpRequest();
  ~HttpRequest();

  std::string relative_url;  
  HttpMethod method;
  std::map<std::string, std::string> headers;
  std::string content;
  bool has_content;
};

class HttpRequestParser {
 public:
  
  enum ParseResult {
    WAITING,  
    ACCEPTED,  
  };

  
  enum State {
    STATE_HEADERS,  
    STATE_CONTENT,  
    STATE_ACCEPTED,  
  };

  HttpRequestParser();
  ~HttpRequestParser();

  
  void ProcessChunk(const base::StringPiece& data);

  
  
  
  
  ParseResult ParseRequest();

  
  
  
  scoped_ptr<HttpRequest> GetRequest();

 private:
  HttpMethod GetMethodType(const std::string& token) const;

  
  
  ParseResult ParseHeaders();

  
  
  ParseResult ParseContent();

  
  
  
  std::string ShiftLine();

  scoped_ptr<HttpRequest> http_request_;
  std::string buffer_;
  size_t buffer_position_;  
  State state_;
  
  size_t declared_content_length_;

  DISALLOW_COPY_AND_ASSIGN(HttpRequestParser);
};

}  
}  

#endif  
