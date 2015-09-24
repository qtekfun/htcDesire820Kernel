// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SSL_OPENSSL_CLIENT_KEY_STORE_H_
#define NET_SSL_OPENSSL_CLIENT_KEY_STORE_H_

#include <openssl/evp.h>

#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "crypto/openssl_util.h"
#include "net/base/net_export.h"

namespace net {

class X509Certificate;

class NET_EXPORT OpenSSLClientKeyStore {
 public:
  
  static OpenSSLClientKeyStore* GetInstance();

  struct EVP_PKEY_Deleter {
    inline void operator()(EVP_PKEY* ptr) const {
      EVP_PKEY_free(ptr);
    }
  };

  typedef scoped_ptr<EVP_PKEY, EVP_PKEY_Deleter> ScopedEVP_PKEY;

  
  
  
  
  
  
  
  
  
  
  
  NET_EXPORT bool RecordClientCertPrivateKey(const X509Certificate* cert,
                                             EVP_PKEY* private_key);

  
  
  
  
  
  
  bool FetchClientCertPrivateKey(const X509Certificate* cert,
                                 ScopedEVP_PKEY* private_key);

  
  void Flush();

 protected:
  OpenSSLClientKeyStore();

  ~OpenSSLClientKeyStore();

  
  
  
  
  void AddKeyPair(EVP_PKEY* pub_key, EVP_PKEY* private_key);

 private:
  
  
  class KeyPair {
   public:
    explicit KeyPair(EVP_PKEY* pub_key, EVP_PKEY* priv_key);
    KeyPair(const KeyPair& other);
    void operator=(const KeyPair& other);
    ~KeyPair();

    EVP_PKEY* public_key;
    EVP_PKEY* private_key;

   private:
    KeyPair();  
  };

  
  int FindKeyPairIndex(EVP_PKEY* public_key);

  std::vector<KeyPair> pairs_;

  friend struct DefaultSingletonTraits<OpenSSLClientKeyStore>;

  DISALLOW_COPY_AND_ASSIGN(OpenSSLClientKeyStore);
};

}  

#endif  
