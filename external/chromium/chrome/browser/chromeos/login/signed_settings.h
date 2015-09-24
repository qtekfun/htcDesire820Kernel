// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SIGNED_SETTINGS_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SIGNED_SETTINGS_H_
#pragma once

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "chrome/browser/chromeos/login/owner_manager.h"



namespace enterprise_management {
class PolicyFetchResponse;
class PolicyData;
}  
namespace em = enterprise_management;

namespace chromeos {
class OwnershipService;

class SignedSettings : public base::RefCountedThreadSafe<SignedSettings>,
                       public OwnerManager::Delegate {
 public:
  enum ReturnCode {
    SUCCESS,
    NOT_FOUND,         
    KEY_UNAVAILABLE,   
    OPERATION_FAILED,  
    BAD_SIGNATURE      
  };

  template <class T>
  class Delegate {
   public:
    
    virtual void OnSettingsOpCompleted(ReturnCode code, T value) {}
  };

  SignedSettings();
  virtual ~SignedSettings();

  
  
  static SignedSettings* CreateCheckWhitelistOp(
      const std::string& email,
      SignedSettings::Delegate<bool>* d);

  static SignedSettings* CreateWhitelistOp(const std::string& email,
                                           bool add_to_whitelist,
                                           SignedSettings::Delegate<bool>* d);

  
  
  static SignedSettings* CreateStorePropertyOp(
      const std::string& name,
      const std::string& value,
      SignedSettings::Delegate<bool>* d);

  static SignedSettings* CreateRetrievePropertyOp(
      const std::string& name,
      SignedSettings::Delegate<std::string>* d);

  
  
  static SignedSettings* CreateStorePolicyOp(
      em::PolicyFetchResponse* policy,
      SignedSettings::Delegate<bool>* d);

  static SignedSettings* CreateRetrievePolicyOp(
      SignedSettings::Delegate<const em::PolicyFetchResponse&>* d);

  static bool EnumerateWhitelist(std::vector<std::string>* whitelisted);

  static ReturnCode MapKeyOpCode(OwnerManager::KeyOpCode code);

  virtual void Execute() = 0;

  virtual void Fail(ReturnCode code) = 0;

  
  void OnKeyOpComplete(const OwnerManager::KeyOpCode return_code,
                       const std::vector<uint8>& payload) = 0;

 protected:
  static bool PolicyIsSane(const em::PolicyFetchResponse& value,
                           em::PolicyData* poldata);

  void set_service(OwnershipService* service) { service_ = service; }

  void TryToFetchPolicyAndCallBack();

  OwnershipService* service_;

 private:
  friend class SignedSettingsTest;
  friend class SignedSettingsHelperTest;

  class Relay
      : public SignedSettings::Delegate<const em::PolicyFetchResponse&> {
   public:
    
    explicit Relay(SignedSettings* s);
    virtual ~Relay();
    
    virtual void OnSettingsOpCompleted(SignedSettings::ReturnCode code,
                                       const em::PolicyFetchResponse& value);
   private:
    SignedSettings* settings_;
    DISALLOW_COPY_AND_ASSIGN(Relay);
  };

  
  static const char kDevicePolicyType[];

  scoped_ptr<Relay> relay_;
  scoped_refptr<SignedSettings> polfetcher_;
  DISALLOW_COPY_AND_ASSIGN(SignedSettings);
};

}  

#endif  
