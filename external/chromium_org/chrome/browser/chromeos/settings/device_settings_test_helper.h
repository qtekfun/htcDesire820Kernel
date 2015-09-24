// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SETTINGS_DEVICE_SETTINGS_TEST_HELPER_H_
#define CHROME_BROWSER_CHROMEOS_SETTINGS_DEVICE_SETTINGS_TEST_HELPER_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop.h"
#include "base/strings/string_util.h"
#include "chrome/browser/chromeos/policy/device_policy_builder.h"
#include "chrome/browser/chromeos/settings/device_settings_service.h"
#include "chrome/browser/chromeos/settings/device_settings_test_helper.h"
#include "chrome/browser/chromeos/settings/mock_owner_key_util.h"
#include "chromeos/dbus/session_manager_client.h"
#include "content/public/test/test_browser_thread.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace chromeos {

class DeviceSettingsTestHelper : public SessionManagerClient {
 public:
  
  DeviceSettingsTestHelper();
  virtual ~DeviceSettingsTestHelper();

  
  void FlushLoops();

  
  void FlushStore();

  
  void FlushRetrieve();

  
  void Flush();

  
  bool HasPendingOperations() const;

  bool store_result() {
    return device_policy_.store_result_;
  }
  void set_store_result(bool store_result) {
    device_policy_.store_result_ = store_result;
  }

  const std::string& policy_blob() {
    return device_policy_.policy_blob_;
  }
  void set_policy_blob(const std::string& policy_blob) {
    device_policy_.policy_blob_ = policy_blob;
  }

  const std::string& device_local_account_policy_blob(
      const std::string& id) const {
    const std::map<std::string, PolicyState>::const_iterator entry =
        device_local_account_policy_.find(id);
    return entry == device_local_account_policy_.end() ?
        base::EmptyString() : entry->second.policy_blob_;
  }

  void set_device_local_account_policy_blob(const std::string& id,
                                            const std::string& policy_blob) {
    device_local_account_policy_[id].policy_blob_ = policy_blob;
  }

  
  virtual void Init(dbus::Bus* bus) OVERRIDE;
  virtual void AddObserver(Observer* observer) OVERRIDE;
  virtual void RemoveObserver(Observer* observer) OVERRIDE;
  virtual bool HasObserver(Observer* observer) OVERRIDE;
  virtual void EmitLoginPromptReady() OVERRIDE;
  virtual void EmitLoginPromptVisible() OVERRIDE;
  virtual void RestartJob(int pid, const std::string& command_line) OVERRIDE;
  virtual void StartSession(const std::string& user_email) OVERRIDE;
  virtual void StopSession() OVERRIDE;
  virtual void StartDeviceWipe() OVERRIDE;
  virtual void RequestLockScreen() OVERRIDE;
  virtual void NotifyLockScreenShown() OVERRIDE;
  virtual void NotifyLockScreenDismissed() OVERRIDE;
  virtual void RetrieveActiveSessions(
      const ActiveSessionsCallback& callback) OVERRIDE;
  virtual void RetrieveDevicePolicy(
      const RetrievePolicyCallback& callback) OVERRIDE;
  virtual void RetrievePolicyForUser(
      const std::string& username,
      const RetrievePolicyCallback& callback) OVERRIDE;
  virtual std::string BlockingRetrievePolicyForUser(
      const std::string& username) OVERRIDE;
  virtual void RetrieveDeviceLocalAccountPolicy(
      const std::string& account_id,
      const RetrievePolicyCallback& callback) OVERRIDE;
  virtual void StoreDevicePolicy(const std::string& policy_blob,
                                 const StorePolicyCallback& callback) OVERRIDE;
  virtual void StorePolicyForUser(const std::string& username,
                                  const std::string& policy_blob,
                                  const std::string& policy_key,
                                  const StorePolicyCallback& callback) OVERRIDE;
  virtual void StoreDeviceLocalAccountPolicy(
      const std::string& account_id,
      const std::string& policy_blob,
      const StorePolicyCallback& callback) OVERRIDE;
  virtual void SetFlagsForUser(
      const std::string& account_id,
      const std::vector<std::string>& flags) OVERRIDE;

 private:
  struct PolicyState {
    bool store_result_;
    std::string policy_blob_;
    std::vector<StorePolicyCallback> store_callbacks_;
    std::vector<RetrievePolicyCallback> retrieve_callbacks_;

    PolicyState();
    ~PolicyState();

    bool HasPendingOperations() const {
      return !store_callbacks_.empty() || !retrieve_callbacks_.empty();
    }
  };

  PolicyState device_policy_;
  std::map<std::string, PolicyState> device_local_account_policy_;

  DISALLOW_COPY_AND_ASSIGN(DeviceSettingsTestHelper);
};

class ScopedDeviceSettingsTestHelper : public DeviceSettingsTestHelper {
 public:
  ScopedDeviceSettingsTestHelper();
  virtual ~ScopedDeviceSettingsTestHelper();

 private:
  DISALLOW_COPY_AND_ASSIGN(ScopedDeviceSettingsTestHelper);
};

class DeviceSettingsTestBase : public testing::Test {
 protected:
  DeviceSettingsTestBase();
  virtual ~DeviceSettingsTestBase();

  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  
  void FlushDeviceSettings();

  
  
  void ReloadDeviceSettings();

  base::MessageLoop loop_;
  content::TestBrowserThread ui_thread_;
  content::TestBrowserThread file_thread_;

  policy::DevicePolicyBuilder device_policy_;

  DeviceSettingsTestHelper device_settings_test_helper_;
  scoped_refptr<MockOwnerKeyUtil> owner_key_util_;
  
  
  DeviceSettingsService device_settings_service_;

 private:
  DISALLOW_COPY_AND_ASSIGN(DeviceSettingsTestBase);
};

}  

#endif  
