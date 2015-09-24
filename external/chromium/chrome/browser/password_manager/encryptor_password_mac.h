// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_ENCRYPTOR_PASSWORD_H__
#define CHROME_BROWSER_PASSWORD_MANAGER_ENCRYPTOR_PASSWORD_H__
#pragma once

#include <string>

#include "base/basictypes.h"

class MacKeychain;

class EncryptorPassword {
 public:
  explicit EncryptorPassword(const MacKeychain& keychain)
      : keychain_(keychain) {
  }

  
  
  
  
  
  
  std::string GetEncryptorPassword() const;

 private:
  DISALLOW_COPY_AND_ASSIGN(EncryptorPassword);
  const MacKeychain& keychain_;
};

#endif  
