// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_THROTTLER_HEADER_INTERFACE_H_
#define NET_URL_REQUEST_URL_REQUEST_THROTTLER_HEADER_INTERFACE_H_

#include <string>

namespace net {

class URLRequestThrottlerHeaderInterface {
 public:
  virtual ~URLRequestThrottlerHeaderInterface() {}

  
  
  
  virtual std::string GetNormalizedValue(const std::string& key) const = 0;

  
  virtual int GetResponseCode() const = 0;
};

}  

#endif  
