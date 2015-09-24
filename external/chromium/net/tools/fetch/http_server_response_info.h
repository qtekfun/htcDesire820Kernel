// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_RESPONSE_INFO_H_
#define NET_HTTP_HTTP_RESPONSE_INFO_H_
#pragma once

#include <map>
#include <string>

class HttpServerResponseInfo {
 public:
  HttpServerResponseInfo();
  ~HttpServerResponseInfo();

  
  std::string protocol;

  
  int status;

  
  std::string server_name;

  
  std::string content_type;

  
  int content_length;

  
  bool connection_close;

  
  std::map<std::string, std::string> headers;
};

#endif  
