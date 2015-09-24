// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_BASE_RSA_KEY_PAIR_H_
#define REMOTING_BASE_RSA_KEY_PAIR_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"

namespace crypto {
class RSAPrivateKey;
}  

namespace remoting {

class RsaKeyPair : public base::RefCountedThreadSafe<RsaKeyPair> {
 public:
  
  static scoped_refptr<RsaKeyPair> Generate();

  
  static scoped_refptr<RsaKeyPair> FromString(const std::string& key_base64);

  
  std::string ToString() const;

  
  
  
  std::string GenerateCertificate() const;

  
  std::string GetPublicKey() const;

  
  std::string SignMessage(const std::string& message) const;

  crypto::RSAPrivateKey* private_key() { return key_.get(); }

 private:
  friend class base::RefCountedThreadSafe<RsaKeyPair>;
  RsaKeyPair(scoped_ptr<crypto::RSAPrivateKey> key);
  virtual ~RsaKeyPair();

  scoped_ptr<crypto::RSAPrivateKey> key_;

  DISALLOW_COPY_AND_ASSIGN(RsaKeyPair);
};

}  

#endif  
