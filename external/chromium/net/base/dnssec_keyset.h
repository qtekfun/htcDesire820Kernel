// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_DNSSEC_KEYSET_H_
#define NET_BASE_DNSSEC_KEYSET_H_

#include <string>
#include <vector>

#include "base/string_piece.h"

namespace net {

class DNSSECKeySet {
 public:
  DNSSECKeySet();
  ~DNSSECKeySet();

  
  
  bool AddKey(const base::StringPiece& dnskey);

  
  
  
  
  
  
  bool CheckSignature(const base::StringPiece& name,
                      const base::StringPiece& zone,
                      const base::StringPiece& signature,
                      uint16 rrtype,
                      const std::vector<base::StringPiece>& rrdatas);

  
  
  static uint16 DNSKEYToKeyID(const base::StringPiece& dnskey);

  
  
  void IgnoreTimestamps();

 private:
  bool VerifySignature(
      base::StringPiece signature_algorithm,
      base::StringPiece signature,
      base::StringPiece public_key,
      base::StringPiece signed_data);

  std::string ASN1WrapDNSKEY(const base::StringPiece& dnskey);

  bool ignore_timestamps_;
  std::vector<uint16> keyids_;
  std::vector<std::string> public_keys_;
};

}  

#endif  
