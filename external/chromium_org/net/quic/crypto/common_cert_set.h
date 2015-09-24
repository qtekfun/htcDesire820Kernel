// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_COMMON_CERT_SET_H_
#define NET_QUIC_CRYPTO_COMMON_CERT_SET_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/quic/crypto/crypto_protocol.h"

namespace net {

class NET_EXPORT_PRIVATE CommonCertSets {
 public:
  virtual ~CommonCertSets();

  
  static const CommonCertSets* GetInstanceQUIC();

  
  
  
  virtual base::StringPiece GetCommonHashes() const = 0;

  
  
  
  virtual base::StringPiece GetCert(uint64 hash, uint32 index) const = 0;

  
  
  
  
  virtual bool MatchCert(base::StringPiece cert,
                         base::StringPiece common_set_hashes,
                         uint64* out_hash,
                         uint32* out_index) const = 0;
};

}  

#endif  
