// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_NSS_CRYPTO_MODULE_DELEGATE_H_
#define CRYPTO_NSS_CRYPTO_MODULE_DELEGATE_H_

#include <string>

#include "base/callback_forward.h"
#include "crypto/scoped_nss_types.h"

namespace crypto {

class CryptoModuleBlockingPasswordDelegate {
 public:
  virtual ~CryptoModuleBlockingPasswordDelegate() {}

  
  
  
  
  void* wincx() { return this; }

  
  
  
  
  
  
  virtual std::string RequestPassword(const std::string& slot_name, bool retry,
                                      bool* cancelled) = 0;

};

class NSSCryptoModuleDelegate : public CryptoModuleBlockingPasswordDelegate {
 public:
  virtual ~NSSCryptoModuleDelegate() {}

  
  virtual ScopedPK11Slot RequestSlot() = 0;
};

}  

#endif  
