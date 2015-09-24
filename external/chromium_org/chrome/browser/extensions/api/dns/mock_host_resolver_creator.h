// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DNS_MOCK_HOST_RESOLVER_CREATOR_H_
#define CHROME_BROWSER_EXTENSIONS_API_DNS_MOCK_HOST_RESOLVER_CREATOR_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/synchronization/waitable_event.h"

namespace net {
class MockHostResolver;
}

namespace extensions {

class MockHostResolverCreator
    : public base::RefCountedThreadSafe<MockHostResolverCreator> {
 public:
  static const std::string kHostname;
  static const std::string kAddress;

  MockHostResolverCreator();

  net::MockHostResolver* CreateMockHostResolver();
  void DeleteMockHostResolver();

 private:
  friend class base::RefCountedThreadSafe<MockHostResolverCreator>;
  virtual ~MockHostResolverCreator();

  void CreateMockHostResolverOnIOThread();
  void DeleteMockHostResolverOnIOThread();

  base::WaitableEvent resolver_event_;

  
  
  
  
  
  net::MockHostResolver* mock_host_resolver_;
};

}  

#endif  
