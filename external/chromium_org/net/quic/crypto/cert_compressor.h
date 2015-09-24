// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_CERT_COMPRESSOR_H_
#define NET_QUIC_CRYPTO_CERT_COMPRESSOR_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/quic/crypto/common_cert_set.h"
#include "net/quic/crypto/crypto_protocol.h"

namespace net {

class NET_EXPORT_PRIVATE CertCompressor {
 public:
  
  
  
  
  
  static std::string CompressChain(const std::vector<std::string>& certs,
                                   base::StringPiece client_common_set_hashes,
                                   base::StringPiece client_cached_cert_hashes,
                                   const CommonCertSets* common_sets);

  
  // into a series of certificates that are written to |out_certs|.
  
  
  static bool DecompressChain(base::StringPiece in,
                              const std::vector<std::string>& cached_certs,
                              const CommonCertSets* common_sets,
                              std::vector<std::string>* out_certs);
};

}  

#endif  
