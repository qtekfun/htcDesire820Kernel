// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_OPENSSL_PRIVATE_KEY_STORE_H_
#define NET_BASE_OPENSSL_PRIVATE_KEY_STORE_H_
#pragma once

#include "base/basictypes.h"
#include "net/base/net_export.h"

typedef struct evp_pkey_st EVP_PKEY;

class GURL;

namespace net {

class
#ifdef ANDROID
NET_EXPORT
#endif
OpenSSLPrivateKeyStore {
 public:
  
  static OpenSSLPrivateKeyStore* GetInstance();

  virtual ~OpenSSLPrivateKeyStore() {}

  
  
  
  
  
  virtual bool StorePrivateKey(const GURL& url, EVP_PKEY* pkey) = 0;

  
  
  virtual EVP_PKEY* FetchPrivateKey(EVP_PKEY* public_key) = 0;

 protected:
  OpenSSLPrivateKeyStore() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(OpenSSLPrivateKeyStore);
};

} 

#endif  
