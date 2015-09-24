// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_PROXY_UTILS_H_
#define NET_HTTP_HTTP_PROXY_UTILS_H_
#pragma once

#include <string>

namespace net {

struct HttpRequestInfo;
class HttpRequestHeaders;
class HostPortPair;

void BuildTunnelRequest(const HttpRequestInfo& request_info,
                        const HttpRequestHeaders& auth_headers,
                        const HostPortPair& endpoint,
                        std::string* request_line,
                        HttpRequestHeaders* request_headers);

}  

#endif  
