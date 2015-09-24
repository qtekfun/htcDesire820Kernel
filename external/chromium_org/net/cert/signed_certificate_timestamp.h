// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_SIGNED_CERTIFICATE_TIMESTAMP_H_
#define NET_CERT_SIGNED_CERTIFICATE_TIMESTAMP_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "net/base/hash_value.h"
#include "net/base/net_export.h"

class Pickle;
class PickleIterator;

namespace net {

namespace ct {

struct NET_EXPORT LogEntry {
  
  enum Type {
    LOG_ENTRY_TYPE_X509 = 0,
    LOG_ENTRY_TYPE_PRECERT = 1
  };

  LogEntry();
  ~LogEntry();
  void Reset();

  Type type;

  
  std::string leaf_certificate;

  
  SHA256HashValue issuer_key_hash;
  std::string tbs_certificate;
};

struct NET_EXPORT_PRIVATE DigitallySigned {
  enum HashAlgorithm {
    HASH_ALGO_NONE = 0,
    HASH_ALGO_MD5 = 1,
    HASH_ALGO_SHA1 = 2,
    HASH_ALGO_SHA224 = 3,
    HASH_ALGO_SHA256 = 4,
    HASH_ALGO_SHA384 = 5,
    HASH_ALGO_SHA512 = 6,
  };

  enum SignatureAlgorithm {
    SIG_ALGO_ANONYMOUS = 0,
    SIG_ALGO_RSA = 1,
    SIG_ALGO_DSA = 2,
    SIG_ALGO_ECDSA = 3
  };

  DigitallySigned();
  ~DigitallySigned();

  HashAlgorithm hash_algorithm;
  SignatureAlgorithm signature_algorithm;
  
  std::string signature_data;
};

struct NET_EXPORT SignedCertificateTimestamp
    : public base::RefCountedThreadSafe<SignedCertificateTimestamp> {
  
  
  struct NET_EXPORT LessThan {
    bool operator()(const scoped_refptr<SignedCertificateTimestamp>& lhs,
                    const scoped_refptr<SignedCertificateTimestamp>& rhs) const;
  };

  
  enum Version {
    SCT_VERSION_1 = 0,
  };

  
  enum Origin {
    SCT_EMBEDDED = 0,
    SCT_FROM_TLS_EXTENSION = 1,
    SCT_FROM_OCSP_RESPONSE = 2,
  };

  SignedCertificateTimestamp();

  void Persist(Pickle* pickle);
  static scoped_refptr<SignedCertificateTimestamp> CreateFromPickle(
      PickleIterator* iter);

  Version version;
  std::string log_id;
  base::Time timestamp;
  std::string extensions;
  DigitallySigned signature;
  
  
  Origin origin;
  
  
  
  
  std::string log_description;

 private:
  friend class base::RefCountedThreadSafe<SignedCertificateTimestamp>;

  ~SignedCertificateTimestamp();

  DISALLOW_COPY_AND_ASSIGN(SignedCertificateTimestamp);
};

}  

}  

#endif  
