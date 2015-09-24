// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_TOOLS_HTTP_SERVER_REQUEST_INFO_H_
#define NET_BASE_TOOLS_HTTP_SERVER_REQUEST_INFO_H_
#pragma once

#include <map>
#include <string>

#include "net/http/http_request_info.h"

class HttpServerRequestInfo : public net::HttpRequestInfo {
 public:
  HttpServerRequestInfo();
  virtual ~HttpServerRequestInfo();

  
  std::map<std::string, std::string> headers;
};

#endif  
