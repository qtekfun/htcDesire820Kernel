// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_ENTERPRISE_INSTALL_ATTRIBUTES_H_
#define CHROME_BROWSER_POLICY_ENTERPRISE_INSTALL_ATTRIBUTES_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"

namespace chromeos {
class CryptohomeLibrary;
}

namespace policy {

class EnterpriseInstallAttributes {
 public:
  
  enum LockResult {
    LOCK_SUCCESS,
    LOCK_NOT_READY,
    LOCK_BACKEND_ERROR,
    LOCK_WRONG_USER,
  };

  explicit EnterpriseInstallAttributes(chromeos::CryptohomeLibrary* cryptohome);

  
  
  
  LockResult LockDevice(const std::string& user) WARN_UNUSED_RESULT;

  
  bool IsEnterpriseDevice();

  
  
  std::string GetDomain();

  
  
  std::string GetRegistrationUser();

 private:
  
  
  void ReadImmutableAttributes();

  chromeos::CryptohomeLibrary* cryptohome_;

  bool device_locked_;
  std::string registration_user_;

  DISALLOW_COPY_AND_ASSIGN(EnterpriseInstallAttributes);
};

}  

#endif  
