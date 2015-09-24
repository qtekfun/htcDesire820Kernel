// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CROS_CRYPTOHOME_LIBRARY_H_
#define CHROME_BROWSER_CHROMEOS_CROS_CRYPTOHOME_LIBRARY_H_
#pragma once

#include <string>

#include "base/memory/singleton.h"
#include "chrome/browser/chromeos/cros/cros_library.h"
#include "third_party/cros/chromeos_cryptohome.h"

namespace chromeos {

class CryptohomeLibrary {
 public:
  class Delegate {
   public:
    
    
    virtual void OnComplete(bool success, int return_code) = 0;
  };

  CryptohomeLibrary();
  virtual ~CryptohomeLibrary();

  
  
  virtual bool CheckKey(const std::string& user_email,
                        const std::string& passhash) = 0;

  
  
  
  
  virtual bool AsyncCheckKey(const std::string& user_email,
                             const std::string& passhash,
                             Delegate* callback) = 0;

  
  
  virtual bool MigrateKey(const std::string& user_email,
                          const std::string& old_hash,
                          const std::string& new_hash) = 0;

  
  
  
  
  
  virtual bool AsyncMigrateKey(const std::string& user_email,
                               const std::string& old_hash,
                               const std::string& new_hash,
                               Delegate* callback) = 0;

  
  
  virtual bool Mount(const std::string& user_email,
                     const std::string& passhash,
                     int* error_code) = 0;

  
  
  
  
  
  
  
  
  
  virtual bool AsyncMount(const std::string& user_email,
                          const std::string& passhash,
                          const bool create_if_missing,
                          Delegate* callback) = 0;

  
  virtual bool MountForBwsi(int* error_code) = 0;

  
  
  
  virtual bool AsyncMountForBwsi(Delegate* callback) = 0;

  
  
  virtual bool Unmount() = 0;

  
  
  virtual bool Remove(const std::string& user_email) = 0;

  
  
  virtual bool AsyncRemove(const std::string& user_email,
                           Delegate* callback) = 0;

  
  virtual bool IsMounted() = 0;

  
  virtual CryptohomeBlob GetSystemSalt() = 0;

  
  
  virtual bool AsyncDoAutomaticFreeDiskSpaceControl(Delegate* callback) = 0;

  

  
  virtual bool TpmIsReady() = 0;

  
  virtual bool TpmIsEnabled() = 0;

  
  virtual bool TpmIsOwned() = 0;

  
  virtual bool TpmIsBeingOwned() = 0;

  
  
  virtual bool TpmGetPassword(std::string* password) = 0;

  
  virtual void TpmCanAttemptOwnership() = 0;

  
  
  virtual void TpmClearStoredPassword() = 0;

  virtual bool InstallAttributesGet(const std::string& name,
                                    std::string* value) = 0;
  virtual bool InstallAttributesSet(const std::string& name,
                                    const std::string& value) = 0;
  virtual int InstallAttributesCount() = 0;
  virtual bool InstallAttributesFinalize() = 0;
  virtual bool InstallAttributesIsReady() = 0;
  virtual bool InstallAttributesIsSecure() = 0;
  virtual bool InstallAttributesIsInvalid() = 0;
  virtual bool InstallAttributesIsFirstInstall() = 0;

  
  
  
  virtual void Pkcs11GetTpmTokenInfo(std::string* label,
                                     std::string* user_pin) = 0;

  
  
  
  virtual bool Pkcs11IsTpmTokenReady() = 0;

  
  
  static CryptohomeLibrary* GetImpl(bool stub);
};

}  

#endif  
