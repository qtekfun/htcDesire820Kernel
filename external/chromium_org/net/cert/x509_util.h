// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_X509_UTIL_H_
#define NET_CERT_X509_UTIL_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "net/base/net_export.h"

namespace crypto {
class ECPrivateKey;
class RSAPrivateKey;
}

namespace net {

class X509Certificate;

namespace x509_util {

enum DigestAlgorithm {
  DIGEST_SHA1,
  DIGEST_SHA256
};

NET_EXPORT_PRIVATE bool IsSupportedValidityRange(base::Time not_valid_before,
                                                 base::Time not_valid_after);

NET_EXPORT_PRIVATE bool CreateKeyAndDomainBoundCertEC(
    const std::string& domain,
    uint32 serial_number,
    base::Time not_valid_before,
    base::Time not_valid_after,
    scoped_ptr<crypto::ECPrivateKey>* key,
    std::string* der_cert);

NET_EXPORT_PRIVATE bool CreateDomainBoundCertEC(crypto::ECPrivateKey* key,
                                                DigestAlgorithm alg,
                                                const std::string& domain,
                                                uint32 serial_number,
                                                base::Time not_valid_before,
                                                base::Time not_valid_after,
                                                std::string* der_cert);

NET_EXPORT bool CreateKeyAndSelfSignedCert(
    const std::string& subject,
    uint32 serial_number,
    base::Time not_valid_before,
    base::Time not_valid_after,
    scoped_ptr<crypto::RSAPrivateKey>* key,
    std::string* der_cert);

NET_EXPORT bool CreateSelfSignedCert(crypto::RSAPrivateKey* key,
                                     DigestAlgorithm alg,
                                     const std::string& subject,
                                     uint32 serial_number,
                                     base::Time not_valid_before,
                                     base::Time not_valid_after,
                                     std::string* der_cert);

class NET_EXPORT_PRIVATE ClientCertSorter {
 public:
  ClientCertSorter();

  bool operator()(
      const scoped_refptr<X509Certificate>& a,
      const scoped_refptr<X509Certificate>& b) const;

 private:
  base::Time now_;
};

} 

} 

#endif  
