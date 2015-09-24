// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SETTINGS_TOKEN_ENCRYPTOR_H_
#define CHROME_BROWSER_CHROMEOS_SETTINGS_TOKEN_ENCRYPTOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace crypto {
class SymmetricKey;
}

namespace chromeos {

class TokenEncryptor {
 public:
  virtual ~TokenEncryptor() {}

  
  
  
  virtual std::string EncryptWithSystemSalt(const std::string& token) = 0;

  
  
  virtual std::string DecryptWithSystemSalt(
      const std::string& encrypted_token_hex) = 0;
};

class CryptohomeTokenEncryptor : public TokenEncryptor {
 public:
  explicit CryptohomeTokenEncryptor(const std::string& system_salt);
  virtual ~CryptohomeTokenEncryptor();

  
  virtual std::string EncryptWithSystemSalt(const std::string& token) OVERRIDE;
  virtual std::string DecryptWithSystemSalt(
      const std::string& encrypted_token_hex) OVERRIDE;

 private:
  
  crypto::SymmetricKey* PassphraseToKey(const std::string& passphrase,
                                        const std::string& salt);

  
  std::string EncryptTokenWithKey(crypto::SymmetricKey* key,
                                  const std::string& salt,
                                  const std::string& token);

  
  std::string DecryptTokenWithKey(crypto::SymmetricKey* key,
                                  const std::string& salt,
                                  const std::string& encrypted_token_hex);

  
  
  std::string system_salt_;

  
  
  scoped_ptr<crypto::SymmetricKey> system_salt_key_;

  DISALLOW_COPY_AND_ASSIGN(CryptohomeTokenEncryptor);
};

}  

#endif  
