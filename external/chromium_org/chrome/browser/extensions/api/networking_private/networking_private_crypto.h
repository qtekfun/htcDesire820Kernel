// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_NETWORKING_PRIVATE_NETWORKING_PRIVATE_CRYPTO_H_
#define CHROME_BROWSER_EXTENSIONS_API_NETWORKING_PRIVATE_NETWORKING_PRIVATE_CRYPTO_H_

#include <string>
#include "base/basictypes.h"

class NetworkingPrivateCrypto {
 public:
  NetworkingPrivateCrypto();
  ~NetworkingPrivateCrypto();

  
  
  
  
  
  
  
  bool VerifyCredentials(const std::string& certificate,
                         const std::string& signature,
                         const std::string& data,
                         const std::string& connected_mac);

  
  
  
  
  
  
  bool EncryptByteString(const std::string& public_key,
                         const std::string& data,
                         std::string* encrypted_output);

 private:
  friend class NetworkingPrivateCryptoTest;

  
  
  
  
  
  
  bool DecryptByteString(const std::string& private_key_pem,
                         const std::string& encrypted_data,
                         std::string* decrypted_output);

  DISALLOW_COPY_AND_ASSIGN(NetworkingPrivateCrypto);
};

#endif  

