// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DNS_HOST_RESOLVER_WRAPPER_H_
#define CHROME_BROWSER_EXTENSIONS_API_DNS_HOST_RESOLVER_WRAPPER_H_

#include "base/memory/singleton.h"
#include "net/dns/host_resolver.h"

namespace extensions {

class HostResolverWrapper {
 public:
  static HostResolverWrapper* GetInstance();

  
  
  
  
  net::HostResolver* GetHostResolver(net::HostResolver* real_resolver);

  
  void SetHostResolverForTesting(net::HostResolver* mock_resolver);

 private:
  HostResolverWrapper();
  friend struct DefaultSingletonTraits<HostResolverWrapper>;

  net::HostResolver* resolver_;

  DISALLOW_COPY_AND_ASSIGN(HostResolverWrapper);
};

}  

#endif  
