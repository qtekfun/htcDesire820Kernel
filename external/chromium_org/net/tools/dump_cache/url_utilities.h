// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_DUMP_CACHE_URL_UTILITIES_H_
#define NET_TOOLS_DUMP_CACHE_URL_UTILITIES_H_

#include <string>

namespace net {

struct UrlUtilities {
  
  
  
  static std::string GetUrlHost(const std::string& url);

  
  
  
  static std::string GetUrlHostPath(const std::string& url);

  
  
  
  static std::string GetUrlPath(const std::string& url);

  
  
  static std::string Unescape(const std::string& escaped_url);
};

}  

#endif  
