// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_HANDSHAKE_HANDLER_H_
#define NET_WEBSOCKETS_WEBSOCKET_HANDSHAKE_HANDLER_H_
#pragma once

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "net/http/http_request_info.h"
#include "net/http/http_response_info.h"
#include "net/spdy/spdy_framer.h"

namespace net {

class WebSocketHandshakeRequestHandler {
 public:
  WebSocketHandshakeRequestHandler();
  ~WebSocketHandshakeRequestHandler() {}

  
  
  
  bool ParseRequest(const char* data, int length);

  size_t original_length() const;

  
  
  void AppendHeaderIfMissing(const std::string& name,
                             const std::string& value);
  
  void RemoveHeaders(const char* const headers_to_remove[],
                     size_t headers_to_remove_len);

  
  
  HttpRequestInfo GetRequestInfo(const GURL& url, std::string* challenge);
  
  
  bool GetRequestHeaderBlock(const GURL& url,
                             spdy::SpdyHeaderBlock* headers,
                             std::string* challenge);
  
  
  std::string GetRawRequest();
  
  size_t raw_length() const;

 private:
  std::string status_line_;
  std::string headers_;
  std::string key3_;
  int original_length_;
  int raw_length_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketHandshakeRequestHandler);
};

class WebSocketHandshakeResponseHandler {
 public:
  WebSocketHandshakeResponseHandler();
  ~WebSocketHandshakeResponseHandler();

  
  
  
  
  
  
  
  size_t ParseRawResponse(const char* data, int length);
  
  bool HasResponse() const;
  
  bool ParseResponseInfo(const HttpResponseInfo& response_info,
                         const std::string& challenge);
  
  bool ParseResponseHeaderBlock(const spdy::SpdyHeaderBlock& headers,
                                const std::string& challenge);

  
  void GetHeaders(const char* const headers_to_get[],
                  size_t headers_to_get_len,
                  std::vector<std::string>* values);
  
  void RemoveHeaders(const char* const headers_to_remove[],
                     size_t headers_to_remove_len);

  
  std::string GetRawResponse() const;

  
  std::string GetResponse();

 private:
  std::string original_;
  int original_header_length_;
  std::string status_line_;
  std::string headers_;
  std::string header_separator_;
  std::string key_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketHandshakeResponseHandler);
};

}  

#endif  
