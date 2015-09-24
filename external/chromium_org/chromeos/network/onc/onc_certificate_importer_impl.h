// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_ONC_ONC_CERTIFICATE_IMPORTER_IMPL_H_
#define CHROMEOS_NETWORK_ONC_ONC_CERTIFICATE_IMPORTER_IMPL_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/network/onc/onc_certificate_importer.h"
#include "components/onc/onc_constants.h"

namespace base {
class DictionaryValue;
class ListValue;
}

namespace net {
class X509Certificate;
typedef std::vector<scoped_refptr<X509Certificate> > CertificateList;
}

namespace chromeos {
namespace onc {

class CHROMEOS_EXPORT CertificateImporterImpl : public CertificateImporter {
 public:
  typedef std::map<std::string, scoped_refptr<net::X509Certificate> >
      CertsByGUID;

  CertificateImporterImpl();

  
  virtual bool ImportCertificates(
      const base::ListValue& certificates,
      ::onc::ONCSource source,
      net::CertificateList* onc_trusted_certificates) OVERRIDE;

  
  
  
  
  bool ParseAndStoreCertificates(bool allow_trust_imports,
                                 const base::ListValue& onc_certificates,
                                 net::CertificateList* onc_trusted_certificates,
                                 CertsByGUID* imported_server_and_ca_certs);

  
  
  static void ListCertsWithNickname(const std::string& label,
                                    net::CertificateList* result);

 private:
  
  
  static bool DeleteCertAndKeyByNickname(const std::string& label);

  
  
  bool ParseAndStoreCertificate(
      bool allow_trust_imports,
      const base::DictionaryValue& certificate,
      net::CertificateList* onc_trusted_certificates,
      CertsByGUID* imported_server_and_ca_certs);

  
  
  
  
  bool ParseServerOrCaCertificate(
      bool allow_trust_imports,
      const std::string& cert_type,
      const std::string& guid,
      const base::DictionaryValue& certificate,
      net::CertificateList* onc_trusted_certificates,
      CertsByGUID* imported_server_and_ca_certs);

  bool ParseClientCertificate(const std::string& guid,
                              const base::DictionaryValue& certificate);

  DISALLOW_COPY_AND_ASSIGN(CertificateImporterImpl);
};

}  
}  

#endif  
