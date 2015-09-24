// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SIGNED_SETTINGS_HELPER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SIGNED_SETTINGS_HELPER_H_
#pragma once

#include <string>

#include "chrome/browser/chromeos/login/signed_settings.h"

namespace enterprise_management {
class PolicyFetchResponse;
}  
namespace em = enterprise_management;
namespace chromeos {

class SignedSettings;

class SignedSettingsHelper {
 public:
  class Callback {
   public:
    
    
    virtual void OnCheckWhitelistCompleted(
        SignedSettings::ReturnCode code,
        const std::string& email) {}

    
    virtual void OnWhitelistCompleted(
        SignedSettings::ReturnCode code, const std::string& email) {}

    
    virtual void OnUnwhitelistCompleted(
        SignedSettings::ReturnCode code, const std::string& email) {}

    
    virtual void OnStorePropertyCompleted(
        SignedSettings::ReturnCode code,
        const std::string& name,
        const std::string& value) {}

    
    virtual void OnRetrievePropertyCompleted(
        SignedSettings::ReturnCode code,
        const std::string& name,
        const std::string& value) {}

    
    virtual void OnStorePolicyCompleted(
        SignedSettings::ReturnCode code) {}

    
    virtual void OnRetrievePolicyCompleted(
        SignedSettings::ReturnCode code,
        const em::PolicyFetchResponse& policy) {}
  };

  
  static SignedSettingsHelper* Get();

  
  virtual void StartCheckWhitelistOp(const std::string& email,
                                     Callback* callback) = 0;
  virtual void StartWhitelistOp(const std::string& email,
                                bool add_to_whitelist,
                                Callback* callback) = 0;
  virtual void StartStorePropertyOp(const std::string& name,
                                    const std::string& value,
                                    Callback* callback) = 0;
  virtual void StartRetrieveProperty(const std::string& name,
                                     Callback* callback) = 0;
  virtual void StartStorePolicyOp(const em::PolicyFetchResponse& policy,
                                  Callback* callback) = 0;
  virtual void StartRetrievePolicyOp(Callback* callback) = 0;

  
  virtual void CancelCallback(Callback* callback) = 0;

  class TestDelegate {
   public:
    virtual void OnOpCreated(SignedSettings* op) = 0;
    virtual void OnOpStarted(SignedSettings* op) = 0;
    virtual void OnOpCompleted(SignedSettings* op) = 0;
  };

#if defined(UNIT_TEST)
  void set_test_delegate(TestDelegate* test_delegate) {
    test_delegate_ = test_delegate;
  }
#endif  

 protected:
  SignedSettingsHelper() : test_delegate_(NULL) {
  }

  TestDelegate* test_delegate_;
};

}  

#endif  
