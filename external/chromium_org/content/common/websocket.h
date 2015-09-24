// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_WEBSOCKET_H_
#define CONTENT_COMMON_WEBSOCKET_H_

#include <string>
#include <utility>
#include <vector>

#include "base/time/time.h"
#include "url/gurl.h"

namespace content {

enum WebSocketMessageType {
  WEB_SOCKET_MESSAGE_TYPE_CONTINUATION = 0x0,
  WEB_SOCKET_MESSAGE_TYPE_TEXT = 0x1,
  WEB_SOCKET_MESSAGE_TYPE_BINARY = 0x2
};

struct WebSocketHandshakeRequest {
  WebSocketHandshakeRequest();
  ~WebSocketHandshakeRequest();

  
  GURL url;
  
  std::vector<std::pair<std::string, std::string> > headers;
  
  base::Time request_time;
};

struct WebSocketHandshakeResponse {
  WebSocketHandshakeResponse();
  ~WebSocketHandshakeResponse();

  
  GURL url;
  
  int status_code;
  
  std::string status_text;
  
  std::vector<std::pair<std::string, std::string> > headers;
  
  base::Time response_time;
};

}  

#endif  
