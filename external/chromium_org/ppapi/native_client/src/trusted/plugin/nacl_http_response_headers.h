// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.




#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_NACL_HTTP_RESPONSE_HEADERS_H_
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_NACL_HTTP_RESPONSE_HEADERS_H_

#include <string>
#include <utility>
#include <vector>

#include "native_client/src/include/nacl_macros.h"

namespace plugin {

class NaClHttpResponseHeaders {
 public:
  NaClHttpResponseHeaders();
  ~NaClHttpResponseHeaders();

  typedef std::pair<std::string, std::string> Entry;

  
  
  
  void Parse(const std::string& headers_str);

  
  std::string GetHeader(const std::string& name);

  
  
  std::string GetCacheValidators();

  
  bool CacheControlNoStore();

 private:
  std::vector<Entry> header_entries_;
  NACL_DISALLOW_COPY_AND_ASSIGN(NaClHttpResponseHeaders);
};

}  

#endif  
