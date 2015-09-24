// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_FLIP_SERVER_SPDY_UTIL_H_
#define NET_TOOLS_FLIP_SERVER_SPDY_UTIL_H_

#include <string>

namespace net {

extern bool g_need_to_encode_url;

std::string EncodeURL(std::string uri, std::string host, std::string method);

}  

#endif  
