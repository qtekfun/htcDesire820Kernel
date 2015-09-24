// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_UTIL_NIGORI_H_
#define SYNC_UTIL_NIGORI_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "sync/base/sync_export.h"

namespace crypto {
class SymmetricKey;
}  

namespace syncer {

class SYNC_EXPORT Nigori {
 public:
  enum Type {
    Password = 1,
  };

  Nigori();
  virtual ~Nigori();

  
  bool InitByDerivation(const std::string& hostname,
                        const std::string& username,
                        const std::string& password);

  
  
  bool InitByImport(const std::string& user_key,
                    const std::string& encryption_key,
                    const std::string& mac_key);

  
  
  
  
  bool Permute(Type type, const std::string& name, std::string* permuted) const;

  
  
  bool Encrypt(const std::string& value, std::string* encrypted) const;

  
  
  bool Decrypt(const std::string& value, std::string* decrypted) const;

  
  bool ExportKeys(std::string* user_key,
                  std::string* encryption_key,
                  std::string* mac_key) const;

  static const char kSaltSalt[];  
  static const size_t kSaltKeySizeInBits = 128;
  static const size_t kDerivedKeySizeInBits = 128;
  static const size_t kIvSize = 16;
  static const size_t kHashSize = 32;

  static const size_t kSaltIterations = 1001;
  static const size_t kUserIterations = 1002;
  static const size_t kEncryptionIterations = 1003;
  static const size_t kSigningIterations = 1004;

 private:
  scoped_ptr<crypto::SymmetricKey> user_key_;
  scoped_ptr<crypto::SymmetricKey> encryption_key_;
  scoped_ptr<crypto::SymmetricKey> mac_key_;
};

}  

#endif  
