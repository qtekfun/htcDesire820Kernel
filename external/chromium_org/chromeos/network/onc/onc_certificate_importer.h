// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_ONC_ONC_CERTIFICATE_IMPORTER_H_
#define CHROMEOS_NETWORK_ONC_ONC_CERTIFICATE_IMPORTER_H_

#include "base/basictypes.h"
#include "chromeos/chromeos_export.h"
#include "components/onc/onc_constants.h"
#include "net/cert/x509_certificate.h"

namespace base {
class ListValue;
}

namespace chromeos {
namespace onc {

class CHROMEOS_EXPORT CertificateImporter {
 public:
  CertificateImporter() {}
  virtual ~CertificateImporter() {}

  
  
  
  
  
  
  
  virtual bool ImportCertificates(
      const base::ListValue& certificates,
      ::onc::ONCSource source,
      net::CertificateList* onc_trusted_certificates) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(CertificateImporter);
};

}  
}  

#endif  
