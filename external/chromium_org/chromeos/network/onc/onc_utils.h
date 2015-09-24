// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_ONC_ONC_UTILS_H_
#define CHROMEOS_NETWORK_ONC_ONC_UTILS_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chromeos/chromeos_export.h"
#include "components/onc/onc_constants.h"

namespace base {
class DictionaryValue;
class ListValue;
}

namespace net {
class X509Certificate;
}

namespace chromeos {
namespace onc {

struct OncValueSignature;

CHROMEOS_EXPORT extern const char kEmptyUnencryptedConfiguration[];

typedef std::map<std::string, std::string> CertPEMsByGUIDMap;

CHROMEOS_EXPORT scoped_ptr<base::DictionaryValue> ReadDictionaryFromJson(
    const std::string& json);

CHROMEOS_EXPORT scoped_ptr<base::DictionaryValue> Decrypt(
    const std::string& passphrase,
    const base::DictionaryValue& onc);

CHROMEOS_EXPORT std::string GetSourceAsString(::onc::ONCSource source);

class CHROMEOS_EXPORT StringSubstitution {
 public:
  StringSubstitution() {}
  virtual ~StringSubstitution() {}

  
  
  
  virtual bool GetSubstitute(const std::string& placeholder,
                             std::string* substitute) const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(StringSubstitution);
};

CHROMEOS_EXPORT void ExpandStringsInOncObject(
    const OncValueSignature& signature,
    const StringSubstitution& substitution,
    base::DictionaryValue* onc_object);

CHROMEOS_EXPORT void ExpandStringsInNetworks(
    const StringSubstitution& substitution,
    base::ListValue* network_configs);

CHROMEOS_EXPORT scoped_ptr<base::DictionaryValue> MaskCredentialsInOncObject(
    const OncValueSignature& signature,
    const base::DictionaryValue& onc_object,
    const std::string& mask);

CHROMEOS_EXPORT bool ParseAndValidateOncForImport(
    const std::string& onc_blob,
    ::onc::ONCSource onc_source,
    const std::string& passphrase,
    base::ListValue* network_configs,
    base::DictionaryValue* global_network_config,
    base::ListValue* certificates);

CHROMEOS_EXPORT scoped_refptr<net::X509Certificate> DecodePEMCertificate(
    const std::string& pem_encoded);

CHROMEOS_EXPORT bool ResolveServerCertRefsInNetworks(
    const CertPEMsByGUIDMap& certs_by_guid,
    base::ListValue* network_configs);

CHROMEOS_EXPORT bool ResolveServerCertRefsInNetwork(
    const CertPEMsByGUIDMap& certs_by_guid,
    base::DictionaryValue* network_config);

}  
}  

#endif  
