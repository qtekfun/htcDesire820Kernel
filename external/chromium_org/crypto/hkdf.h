// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_HKDF_H_
#define CRYPTO_HKDF_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "build/build_config.h"
#include "crypto/crypto_export.h"

namespace crypto {

class CRYPTO_EXPORT HKDF {
 public:
  
  
  
  
  
  
  
  
  
  
  HKDF(const base::StringPiece& secret,
       const base::StringPiece& salt,
       const base::StringPiece& info,
       size_t key_bytes_to_generate,
       size_t iv_bytes_to_generate);
  ~HKDF();

  base::StringPiece client_write_key() const {
    return client_write_key_;
  }
  base::StringPiece client_write_iv() const {
    return client_write_iv_;
  }
  base::StringPiece server_write_key() const {
    return server_write_key_;
  }
  base::StringPiece server_write_iv() const {
    return server_write_iv_;
  }

 private:
  std::vector<uint8> output_;

  base::StringPiece client_write_key_;
  base::StringPiece server_write_key_;
  base::StringPiece client_write_iv_;
  base::StringPiece server_write_iv_;
};

}  

#endif  
