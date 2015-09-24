// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_CRYPTO_MODULE_BLOCKING_PASSWORD_DELEGATE_H_
#define CRYPTO_CRYPTO_MODULE_BLOCKING_PASSWORD_DELEGATE_H_
#pragma once

#include <string>

namespace crypto {

class CryptoModuleBlockingPasswordDelegate {
 public:
  virtual ~CryptoModuleBlockingPasswordDelegate() {}

  
  
  
  
  
  
  
  virtual std::string RequestPassword(const std::string& slot_name, bool retry,
                                      bool* cancelled) = 0;
};

}  

#endif  
