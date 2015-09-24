// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_CLIENT_CERT_UTIL_H_
#define CHROMEOS_NETWORK_CLIENT_CERT_UTIL_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "chromeos/chromeos_export.h"

namespace base {
class DictionaryValue;
}

namespace net {
struct CertPrincipal;
class X509Certificate;
}

namespace chromeos {

class CertificatePattern;
class IssuerSubjectPattern;

namespace client_cert {

enum ConfigType {
  CONFIG_TYPE_NONE,
  CONFIG_TYPE_OPENVPN,
  CONFIG_TYPE_IPSEC,
  CONFIG_TYPE_EAP
};

bool CertPrincipalMatches(const IssuerSubjectPattern& pattern,
                          const net::CertPrincipal& principal);

CHROMEOS_EXPORT scoped_refptr<net::X509Certificate> GetCertificateMatch(
    const CertificatePattern& pattern);

void SetShillProperties(const ConfigType cert_config_type,
                        const std::string& tpm_slot,
                        const std::string& tpm_pin,
                        const std::string* pkcs11_id,
                        base::DictionaryValue* properties);

bool IsCertificateConfigured(const client_cert::ConfigType cert_config_type,
                             const base::DictionaryValue& service_properties);

}  

}  

#endif  
