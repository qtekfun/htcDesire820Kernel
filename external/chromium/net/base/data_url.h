// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_DATA_URL_H_
#define NET_BASE_DATA_URL_H_
#pragma once

#include <string>

class GURL;

#include "net/base/net_export.h"

namespace net {

class NET_EXPORT DataURL {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  static bool Parse(const GURL& url,
                    std::string* mime_type,
                    std::string* charset,
                    std::string* data);
};

}  

#endif  
