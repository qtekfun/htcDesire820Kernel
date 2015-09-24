// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_CERT_X509_UTIL_IOS_H_
#define NET_CERT_X509_UTIL_IOS_H_

#include <Security/Security.h>
#include <vector>

#include "net/cert/x509_cert_types.h"

typedef struct CERTCertificateStr CERTCertificate;

namespace net {

class X509Certificate;

namespace x509_util_ios {

CERTCertificate* CreateNSSCertHandleFromOSHandle(SecCertificateRef cert_handle);

SecCertificateRef CreateOSCertHandleFromNSSHandle(
    CERTCertificate* nss_cert_handle);

X509Certificate* CreateCertFromNSSHandles(
    CERTCertificate* cert_handle,
    const std::vector<CERTCertificate*>& intermediates);

SHA1HashValue CalculateFingerprintNSS(CERTCertificate* cert);

class NSSCertificate {
 public:
  explicit NSSCertificate(SecCertificateRef cert_handle);
  ~NSSCertificate();
  CERTCertificate* cert_handle() const;
 private:
  CERTCertificate* nss_cert_handle_;
};

class NSSCertChain {
 public:
  explicit NSSCertChain(X509Certificate* certificate);
  ~NSSCertChain();
  CERTCertificate* cert_handle() const;
  const std::vector<CERTCertificate*>& cert_chain() const;
 private:
  std::vector<CERTCertificate*> certs_;
};

}  
}  

#endif  
