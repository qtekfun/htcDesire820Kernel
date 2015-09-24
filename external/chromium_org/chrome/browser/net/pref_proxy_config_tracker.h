// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_PREF_PROXY_CONFIG_TRACKER_H_
#define CHROME_BROWSER_NET_PREF_PROXY_CONFIG_TRACKER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace net {
class ProxyConfigService;
}

class PrefProxyConfigTracker {
 public:
  PrefProxyConfigTracker();
  virtual ~PrefProxyConfigTracker();

  
  
  
  
  
  
  
  virtual scoped_ptr<net::ProxyConfigService> CreateTrackingProxyConfigService(
      scoped_ptr<net::ProxyConfigService> base_service) = 0;

  
  
  
  virtual void DetachFromPrefService() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(PrefProxyConfigTracker);
};

#endif  
