// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_OPENPGP_SYMMETRIC_ENCRYPTION_H_
#define CRYPTO_OPENPGP_SYMMETRIC_ENCRYPTION_H_

#include <string>

#include "base/strings/string_piece.h"
#include "crypto/crypto_export.h"

namespace crypto {

class CRYPTO_EXPORT OpenPGPSymmetricEncrytion {
 public:
  enum Result {
    OK,
    UNKNOWN_CIPHER,  
    UNKNOWN_HASH,
    NOT_SYMMETRICALLY_ENCRYPTED,  
    COMPRESSED,  
    PARSE_ERROR,  
    INTERNAL_ERROR,
  };

  static Result Decrypt(base::StringPiece encrypted,
                        base::StringPiece passphrase,
                        std::string *out);

  static std::string Encrypt(base::StringPiece plaintext,
                             base::StringPiece passphrase);
};

}  

#endif  

