// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_RETRY_INFO_H_
#define NET_PROXY_PROXY_RETRY_INFO_H_

#include <map>

#include "base/time/time.h"

namespace net {

struct ProxyRetryInfo {
  
  base::TimeTicks bad_until;

  
  
  base::TimeDelta current_delay;
};

typedef std::map<std::string, ProxyRetryInfo> ProxyRetryInfoMap;

}  

#endif  
