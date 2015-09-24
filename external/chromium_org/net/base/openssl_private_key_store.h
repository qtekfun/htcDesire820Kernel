// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_OPENSSL_PRIVATE_KEY_STORE_H_
#define NET_BASE_OPENSSL_PRIVATE_KEY_STORE_H_

#include <vector>

typedef struct evp_pkey_st EVP_PKEY;

#include "base/basictypes.h"
#include "net/base/net_export.h"

class GURL;

namespace net {

class X509Certificate;

class NET_EXPORT OpenSSLPrivateKeyStore {
 public:
  
  
  
  
  
  
  
  static bool StoreKeyPair(const GURL& url, EVP_PKEY* pkey);

  
  
  
  
  
  
  static bool HasPrivateKey(EVP_PKEY* pub_key);

 private:
  OpenSSLPrivateKeyStore();  
  ~OpenSSLPrivateKeyStore();  
  DISALLOW_COPY_AND_ASSIGN(OpenSSLPrivateKeyStore);
};

} 

#endif  
