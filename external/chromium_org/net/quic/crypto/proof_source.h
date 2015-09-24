// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_PROOF_SOURCE_H_
#define NET_QUIC_CRYPTO_PROOF_SOURCE_H_

#include <string>
#include <vector>

#include "net/base/net_export.h"

namespace net {

class NET_EXPORT_PRIVATE ProofSource {
 public:
  virtual ~ProofSource() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool GetProof(const std::string& hostname,
                        const std::string& server_config,
                        bool ecdsa_ok,
                        const std::vector<std::string>** out_certs,
                        std::string* out_signature) = 0;
};

}  

#endif  
