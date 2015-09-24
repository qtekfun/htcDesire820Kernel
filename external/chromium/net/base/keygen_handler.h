// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_KEYGEN_HANDLER_H_
#define NET_BASE_KEYGEN_HANDLER_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "build/build_config.h"
#include "googleurl/src/gurl.h"

#if defined(USE_NSS)
#include "crypto/crypto_module_blocking_password_delegate.h"
#endif  

namespace net {


class KeygenHandler {
 public:
  
  
  
  KeygenHandler(int key_size_in_bits,
                const std::string& challenge,
                const GURL& url);
  ~KeygenHandler();

  
  
  std::string GenKeyAndSignChallenge();

  
  void set_stores_key(bool store) { stores_key_ = store;}

#if defined(USE_NSS)
  
  
  
  
  void set_crypto_module_password_delegate(
      crypto::CryptoModuleBlockingPasswordDelegate* delegate);
#endif  

 private:
  int key_size_in_bits_;  
  std::string challenge_;  
  GURL url_;  
  bool stores_key_;  
#if defined(USE_NSS)
  
  scoped_ptr<crypto::CryptoModuleBlockingPasswordDelegate>
      crypto_module_password_delegate_;
#endif  
};

}  

#endif  
