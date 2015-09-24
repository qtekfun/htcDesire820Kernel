// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_KEYGEN_HANDLER_H_
#define NET_BASE_KEYGEN_HANDLER_H_

#include <string>

#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "build/build_config.h"
#include "net/base/net_export.h"
#include "url/gurl.h"

namespace crypto {
class NSSCryptoModuleDelegate;
}

namespace net {


class NET_EXPORT KeygenHandler {
 public:
  
  
  
  KeygenHandler(int key_size_in_bits,
                const std::string& challenge,
                const GURL& url);
  ~KeygenHandler();

  
  
  std::string GenKeyAndSignChallenge();

  
  void set_stores_key(bool store) { stores_key_ = store;}

#if defined(USE_NSS)
  
  
  
  
  void set_crypto_module_delegate(
      scoped_ptr<crypto::NSSCryptoModuleDelegate> delegate);
#endif  

 private:
  int key_size_in_bits_;  
  std::string challenge_;  
  GURL url_;  
  bool stores_key_;  
#if defined(USE_NSS)
  
  scoped_ptr<crypto::NSSCryptoModuleDelegate> crypto_module_delegate_;
#endif  
};

}  

#endif  
