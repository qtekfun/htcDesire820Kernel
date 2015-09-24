// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_RESOLVER_ERROR_OBSERVER_H_
#define NET_PROXY_PROXY_RESOLVER_ERROR_OBSERVER_H_

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT_PRIVATE ProxyResolverErrorObserver {
 public:
  ProxyResolverErrorObserver() {}
  virtual ~ProxyResolverErrorObserver() {}

  
  
  
  
  
  
  
  
  virtual void OnPACScriptError(int line_number,
                                const base::string16& error) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ProxyResolverErrorObserver);
};

}  

#endif  
