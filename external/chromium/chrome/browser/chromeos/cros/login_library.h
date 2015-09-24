// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CROS_LOGIN_LIBRARY_H_
#define CHROME_BROWSER_CHROMEOS_CROS_LOGIN_LIBRARY_H_
#pragma once

#include <string>

#include "base/memory/singleton.h"
#include "third_party/cros/chromeos_login.h"

namespace chromeos {

class LoginLibrary {
 public:
  class Delegate {
   public:
    virtual void OnComplete(bool value) = 0;
  };

  virtual ~LoginLibrary() {}
  
  virtual bool EmitLoginPromptReady() = 0;

  
  
  
  
  virtual bool CheckWhitelist(const std::string& email,
                              std::vector<uint8>* OUT_signature) = 0;

  virtual void RequestRetrievePolicy(RetrievePolicyCallback callback,
                                     void* delegate_string) = 0;

  
  
  
  
  
  virtual void RequestRetrieveProperty(const std::string& name,
                                       RetrievePropertyCallback callback,
                                       void* user_data) = 0;

  virtual void RequestStorePolicy(const std::string& policy,
                                  StorePolicyCallback callback,
                                  void* delegate_bool) = 0;

  
  
  
  
  
  virtual bool StorePropertyAsync(const std::string& name,
                                  const std::string& value,
                                  const std::vector<uint8>& signature,
                                  Delegate* callback) = 0;

  
  
  
  
  
  virtual bool WhitelistAsync(const std::string& email,
                              const std::vector<uint8>& signature,
                              Delegate* callback) = 0;

  
  
  
  
  
  
  virtual bool UnwhitelistAsync(const std::string& email,
                                const std::vector<uint8>& signature,
                                Delegate* callback) = 0;

  
  
  
  
  
  
  
  
  virtual bool EnumerateWhitelisted(std::vector<std::string>* whitelisted) = 0;

  
  
  
  
  virtual bool StartSession(const std::string& user_email,
                            const std::string& unique_id ) = 0;

  
  
  
  
  virtual bool StopSession(const std::string& unique_id ) = 0;

  
  virtual bool RestartEntd() = 0;

  
  virtual bool RestartJob(int pid, const std::string& command_line) = 0;

  
  
  static LoginLibrary* GetImpl(bool stub);
};

}  

#endif  
