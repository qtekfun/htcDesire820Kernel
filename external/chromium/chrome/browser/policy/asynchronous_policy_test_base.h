// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_ASYNCHRONOUS_POLICY_TEST_BASE_H_
#define CHROME_BROWSER_POLICY_ASYNCHRONOUS_POLICY_TEST_BASE_H_
#pragma once

#include "base/message_loop.h"
#include "chrome/browser/policy/asynchronous_policy_provider.h"
#include "content/browser/browser_thread.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace policy {

class MockConfigurationPolicyStore;

class ProviderDelegateMock : public AsynchronousPolicyProvider::Delegate {
 public:
  ProviderDelegateMock();
  virtual ~ProviderDelegateMock();

  MOCK_METHOD0(Load, DictionaryValue*());

 private:
  DISALLOW_COPY_AND_ASSIGN(ProviderDelegateMock);
};

class AsynchronousPolicyTestBase : public testing::Test {
 public:
  AsynchronousPolicyTestBase();
  virtual ~AsynchronousPolicyTestBase();

  
  virtual void SetUp();
  virtual void TearDown();

 protected:
  MessageLoop loop_;

  
  
  scoped_ptr<MockConfigurationPolicyStore> store_;
  scoped_ptr<ProviderDelegateMock> delegate_;

 private:
  BrowserThread ui_thread_;
  BrowserThread file_thread_;

  DISALLOW_COPY_AND_ASSIGN(AsynchronousPolicyTestBase);
};

}  

#endif  
