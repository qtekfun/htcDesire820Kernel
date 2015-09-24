// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_DEVICE_POLICY_CROS_BROWSER_TEST_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_DEVICE_POLICY_CROS_BROWSER_TEST_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/scoped_temp_dir.h"
#include "chrome/browser/chromeos/policy/device_policy_builder.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chromeos/dbus/fake_dbus_thread_manager.h"

namespace chromeos {
class FakeSessionManagerClient;
}

namespace policy {

class DevicePolicyCrosTestHelper {
 public:
  DevicePolicyCrosTestHelper();
  ~DevicePolicyCrosTestHelper();

  
  
  
  void MarkAsEnterpriseOwned();

  
  void InstallOwnerKey();

  DevicePolicyBuilder* device_policy() { return &device_policy_; }

 private:
  
  base::ScopedTempDir temp_dir_;

  
  DevicePolicyBuilder device_policy_;

  DISALLOW_COPY_AND_ASSIGN(DevicePolicyCrosTestHelper);
};

class DevicePolicyCrosBrowserTest : public InProcessBrowserTest {
 protected:
  DevicePolicyCrosBrowserTest();
  virtual ~DevicePolicyCrosBrowserTest();

  virtual void SetUpInProcessBrowserTestFixture() OVERRIDE;
  virtual void TearDownInProcessBrowserTestFixture() OVERRIDE;

  
  
  
  void MarkAsEnterpriseOwned();

  
  void InstallOwnerKey();

  
  
  void RefreshDevicePolicy();

  chromeos::FakeDBusThreadManager* fake_dbus_thread_manager() {
    return fake_dbus_thread_manager_;
  }

  chromeos::FakeSessionManagerClient* session_manager_client() {
    return fake_session_manager_client_;
  }

  DevicePolicyBuilder* device_policy() { return test_helper_.device_policy(); }

 private:
  DevicePolicyCrosTestHelper test_helper_;

  
  chromeos::FakeDBusThreadManager* fake_dbus_thread_manager_;
  chromeos::FakeSessionManagerClient* fake_session_manager_client_;

  DISALLOW_COPY_AND_ASSIGN(DevicePolicyCrosBrowserTest);
};

}  

#endif  