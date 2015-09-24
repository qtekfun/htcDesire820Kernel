// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_X509_CERT_TYPES_H_
#define NET_BASE_X509_CERT_TYPES_H_
#pragma once

#include <string.h>

#include <set>
#include <string>
#include <vector>

#include "build/build_config.h"

#if defined(OS_MACOSX)
#include <Security/x509defs.h>
#endif

namespace base {
class Time;
class StringPiece;
}  

namespace net {

class X509Certificate;

struct SHA1Fingerprint {
  bool Equals(const SHA1Fingerprint& other) const {
    return memcmp(data, other.data, sizeof(data)) == 0;
  }

  unsigned char data[20];
};

class SHA1FingerprintLessThan {
 public:
  bool operator() (const SHA1Fingerprint& lhs,
                   const SHA1Fingerprint& rhs) const {
    return memcmp(lhs.data, rhs.data, sizeof(lhs.data)) < 0;
  }
};

struct CertPrincipal {
  CertPrincipal();
  explicit CertPrincipal(const std::string& name);
  ~CertPrincipal();

#if defined(OS_MACOSX)
  
  bool ParseDistinguishedName(const void* ber_name_data, size_t length);

  
  void Parse(const CSSM_X509_NAME* name);

  
  
  
  
  bool Matches(const CertPrincipal& against) const;
#endif

  
  
  std::string GetDisplayName() const;

  
  

  std::string common_name;
  std::string locality_name;
  std::string state_or_province_name;
  std::string country_name;

  std::vector<std::string> street_addresses;
  std::vector<std::string> organization_names;
  std::vector<std::string> organization_unit_names;
  std::vector<std::string> domain_components;
};

class CertPolicy {
 public:
  
  enum Judgment {
    
    UNKNOWN,

    
    ALLOWED,

    
    DENIED,
  };

  CertPolicy();
  ~CertPolicy();

  
  Judgment Check(X509Certificate* cert) const;

  
  void Allow(X509Certificate* cert);

  
  void Deny(X509Certificate* cert);

  
  bool HasAllowedCert() const;

  
  bool HasDeniedCert() const;

 private:
  
  std::set<SHA1Fingerprint, SHA1FingerprintLessThan> allowed_;

  
  std::set<SHA1Fingerprint, SHA1FingerprintLessThan> denied_;
};

#if defined(OS_MACOSX)
inline bool CSSMOIDEqual(const CSSM_OID* oid1, const CSSM_OID* oid2) {
  return oid1->Length == oid2->Length &&
  (memcmp(oid1->Data, oid2->Data, oid1->Length) == 0);
}
#endif

enum CertDateFormat {
  
  CERT_DATE_FORMAT_UTC_TIME,

  
  CERT_DATE_FORMAT_GENERALIZED_TIME,
};

bool ParseCertificateDate(const base::StringPiece& raw_date,
                          CertDateFormat format,
                          base::Time* time);
}  

#endif  
