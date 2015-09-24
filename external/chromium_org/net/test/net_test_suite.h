// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TEST_NET_TEST_SUITE_H_
#define NET_TEST_NET_TEST_SUITE_H_

#include "base/memory/ref_counted.h"
#include "base/test/test_suite.h"
#include "build/build_config.h"
#include "net/dns/mock_host_resolver.h"

namespace base {
class MessageLoop;
}

namespace net {
class NetworkChangeNotifier;
}

class NetTestSuite : public base::TestSuite {
 public:
  NetTestSuite(int argc, char** argv);
  virtual ~NetTestSuite();

  virtual void Initialize() OVERRIDE;

  virtual void Shutdown() OVERRIDE;

 protected:
  
  
  
  NetTestSuite(int argc, char** argv, bool create_at_exit_manager);

  
  
  
  
  void InitializeTestThread();

  
  
  
  
  void InitializeTestThreadNoNetworkChangeNotifier();

 private:
  scoped_ptr<net::NetworkChangeNotifier> network_change_notifier_;
  scoped_ptr<base::MessageLoop> message_loop_;
  scoped_refptr<net::RuleBasedHostResolverProc> host_resolver_proc_;
  net::ScopedDefaultHostResolverProc scoped_host_resolver_proc_;
};

#endif  
