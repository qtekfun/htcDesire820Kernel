// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_TEST_BROWSER_TEST_BASE_H_
#define CONTENT_PUBLIC_TEST_BROWSER_TEST_BASE_H_

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/threading/thread.h"
#include "net/test/spawned_test_server/spawned_test_server.h"
#include "testing/gtest/include/gtest/gtest.h"

class CommandLine;

namespace base {
class FilePath;
}

namespace net {
namespace test_server {
class EmbeddedTestServer;
}

class RuleBasedHostResolverProc;
}  

namespace content {

class BrowserTestBase : public testing::Test {
 public:
  BrowserTestBase();
  virtual ~BrowserTestBase();

  
  void AddRef() {}
  void Release() {}

  
  
  virtual void SetUp() OVERRIDE;

  
  virtual void TearDown() OVERRIDE;

  
  
  
  virtual void SetUpOnMainThread() {}

  
  
  virtual void TearDownOnMainThread() {}

  
  virtual void SetUpCommandLine(CommandLine* command_line) {}

  
  
  net::RuleBasedHostResolverProc* host_resolver() {
    return rule_based_resolver_.get();
  }

 protected:
  
  
  
  
  
  
  
  virtual void SetUpInProcessBrowserTestFixture() {}

  
  virtual void TearDownInProcessBrowserTestFixture() {}

  
  virtual void RunTestOnMainThread() = 0;

  
  
  
  virtual void RunTestOnMainThreadLoop() = 0;

  
  
  const net::SpawnedTestServer* test_server() const {
    return test_server_.get();
  }
  net::SpawnedTestServer* test_server() { return test_server_.get(); }

  
  const net::test_server::EmbeddedTestServer* embedded_test_server() const {
    return embedded_test_server_.get();
  }
  net::test_server::EmbeddedTestServer* embedded_test_server() {
    return embedded_test_server_.get();
  }

#if defined(OS_POSIX)
  
  
  void DisableSIGTERMHandling() {
    handle_sigterm_ = false;
  }
#endif

  
  
  
  
  
  
  void CreateTestServer(const base::FilePath& test_server_base);

  
  
  
  void PostTaskToInProcessRendererAndWait(const base::Closure& task);

  
  
  void UseRealGLContexts() { allow_test_contexts_ = false; }

  
  
  void UseRealGLBindings() { allow_osmesa_ = false; }

 private:
  void ProxyRunTestOnMainThreadLoop();

  
  scoped_ptr<net::SpawnedTestServer> test_server_;

  
  scoped_ptr<net::test_server::EmbeddedTestServer> embedded_test_server_;

  
  scoped_refptr<net::RuleBasedHostResolverProc> rule_based_resolver_;

  
  
  bool allow_test_contexts_;

  
  
  bool allow_osmesa_;

#if defined(OS_POSIX)
  bool handle_sigterm_;
#endif
};

}  

#endif  
