// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEBDATA_ENCRYPTOR_ENCRYPTOR_PASSWORD_MAC_H_
#define COMPONENTS_WEBDATA_ENCRYPTOR_ENCRYPTOR_PASSWORD_MAC_H_

#include <string>

#include "base/basictypes.h"

namespace crypto {
class AppleKeychain;
}  

class EncryptorPassword {
 public:
  explicit EncryptorPassword(const crypto::AppleKeychain& keychain)
      : keychain_(keychain) {
  }

  
  
  
  
  
  
  std::string GetEncryptorPassword() const;

 private:
  DISALLOW_COPY_AND_ASSIGN(EncryptorPassword);
  const crypto::AppleKeychain& keychain_;
};

#endif  
