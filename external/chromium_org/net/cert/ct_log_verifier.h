// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_CT_LOG_VERIFIER_H_
#define NET_CERT_CT_LOG_VERIFIER_H_

#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/cert/signed_certificate_timestamp.h"

#if defined(USE_OPENSSL)
typedef struct evp_pkey_st EVP_PKEY;
#else
typedef struct SECKEYPublicKeyStr SECKEYPublicKey;
#endif

namespace net {

class NET_EXPORT CTLogVerifier {
 public:
  
  
  
  
  static scoped_ptr<CTLogVerifier> Create(
      const base::StringPiece& public_key,
      const base::StringPiece& description);

  ~CTLogVerifier();

  
  const std::string& key_id() const { return key_id_; }
  
  const std::string& description() const { return description_; }

  
  bool Verify(const ct::LogEntry& entry,
              const ct::SignedCertificateTimestamp& sct);

 private:
  FRIEND_TEST_ALL_PREFIXES(CTLogVerifierTest, VerifySignature);

  CTLogVerifier();

  
  bool Init(const base::StringPiece& public_key,
            const base::StringPiece& description);

  
  
  
  bool VerifySignature(const base::StringPiece& data_to_sign,
                       const base::StringPiece& signature);

  std::string key_id_;
  std::string description_;
  ct::DigitallySigned::HashAlgorithm hash_algorithm_;
  ct::DigitallySigned::SignatureAlgorithm signature_algorithm_;

#if defined(USE_OPENSSL)
  EVP_PKEY* public_key_;
#else
  SECKEYPublicKey* public_key_;
#endif
};

}  

#endif  
