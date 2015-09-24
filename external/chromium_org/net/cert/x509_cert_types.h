// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_X509_CERT_TYPES_H_
#define NET_CERT_X509_CERT_TYPES_H_

#include <string.h>

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/logging.h"
#include "base/strings/string_piece.h"
#include "build/build_config.h"
#include "net/base/hash_value.h"
#include "net/base/net_export.h"
#include "net/cert/cert_status_flags.h"

#if defined(OS_MACOSX) && !defined(OS_IOS)
#include <Security/x509defs.h>
#endif

namespace base {
class Time;
}  

namespace net {

class X509Certificate;

struct NET_EXPORT CertPrincipal {
  CertPrincipal();
  explicit CertPrincipal(const std::string& name);
  ~CertPrincipal();

#if (defined(OS_MACOSX) && !defined(OS_IOS)) || defined(OS_WIN)
  
  bool ParseDistinguishedName(const void* ber_name_data, size_t length);
#endif

#if defined(OS_MACOSX) && !defined(OS_IOS)
  
  
  
  
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

class NET_EXPORT CertPolicy {
 public:
  
  enum Judgment {
    
    UNKNOWN,

    
    ALLOWED,

    
    DENIED,
  };

  CertPolicy();
  ~CertPolicy();

  
  
  
  
  
  Judgment Check(X509Certificate* cert, CertStatus error) const;

  
  void Allow(X509Certificate* cert, CertStatus error);

  
  void Deny(X509Certificate* cert, CertStatus error);

  
  bool HasAllowedCert() const;

  
  bool HasDeniedCert() const;

 private:
  
  std::map<SHA1HashValue, CertStatus, SHA1HashValueLessThan> allowed_;

  
  std::map<SHA1HashValue, CertStatus, SHA1HashValueLessThan> denied_;
};

#if defined(OS_MACOSX) && !defined(OS_IOS)
inline bool CSSMOIDEqual(const CSSM_OID* oid1, const CSSM_OID* oid2) {
  return oid1->Length == oid2->Length &&
  (memcmp(oid1->Data, oid2->Data, oid1->Length) == 0);
}
#endif

enum CertDateFormat {
  
  CERT_DATE_FORMAT_UTC_TIME,

  
  CERT_DATE_FORMAT_GENERALIZED_TIME,
};

NET_EXPORT_PRIVATE bool ParseCertificateDate(const base::StringPiece& raw_date,
                                             CertDateFormat format,
                                             base::Time* time);
}  

#endif  
