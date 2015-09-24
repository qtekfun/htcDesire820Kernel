// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CHROME_FRAME_TEST_NET_TEST_AUTOMATION_PROVIDER_H_
#define CHROME_FRAME_TEST_NET_TEST_AUTOMATION_PROVIDER_H_

#include <string>
#include "chrome/browser/automation/automation_provider.h"

namespace net {
class NetworkDelegate;
class URLRequest;
class URLRequestJob;
}  

class TestAutomationResourceMessageFilter;

class TestAutomationProviderDelegate {
 public:
  virtual void OnInitialTabLoaded() = 0;
  virtual void OnProviderDestroyed() = 0;
};

class TestAutomationProvider
    : public AutomationProvider {
 public:
  TestAutomationProvider(Profile* profile,
                         TestAutomationProviderDelegate* delegate);

  virtual ~TestAutomationProvider();

  
  virtual bool OnMessageReceived(const IPC::Message& msg);
  virtual bool Send(IPC::Message* msg);

  
  static net::URLRequestJob* Factory(net::URLRequest* request,
                                     net::NetworkDelegate* network_delegate,
                                     const std::string& scheme);

  
  
  static TestAutomationProvider* NewAutomationProvider(
      Profile* p,
      const std::string& channel,
      TestAutomationProviderDelegate* delegate);

 protected:
  virtual std::string GetProtocolVersion();

  int tab_handle_;
  TestAutomationProviderDelegate* delegate_;

  static TestAutomationProvider* g_provider_instance_;
};

#endif  
