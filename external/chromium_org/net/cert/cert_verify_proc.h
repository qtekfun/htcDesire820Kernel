// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_CERT_VERIFY_PROC_H_
#define NET_CERT_CERT_VERIFY_PROC_H_

#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "net/base/net_export.h"
#include "net/cert/x509_cert_types.h"

namespace net {

class CertVerifyResult;
class CRLSet;
class X509Certificate;
typedef std::vector<scoped_refptr<X509Certificate> > CertificateList;

class NET_EXPORT CertVerifyProc
    : public base::RefCountedThreadSafe<CertVerifyProc> {
 public:
  
  static CertVerifyProc* CreateDefault();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int Verify(X509Certificate* cert,
             const std::string& hostname,
             int flags,
             CRLSet* crl_set,
             const CertificateList& additional_trust_anchors,
             CertVerifyResult* verify_result);

  
  
  
  virtual bool SupportsAdditionalTrustAnchors() const = 0;

 protected:
  CertVerifyProc();
  virtual ~CertVerifyProc();

 private:
  friend class base::RefCountedThreadSafe<CertVerifyProc>;
  FRIEND_TEST_ALL_PREFIXES(CertVerifyProcTest, DigiNotarCerts);

  
  
  virtual int VerifyInternal(X509Certificate* cert,
                             const std::string& hostname,
                             int flags,
                             CRLSet* crl_set,
                             const CertificateList& additional_trust_anchors,
                             CertVerifyResult* verify_result) = 0;

  
  static bool IsBlacklisted(X509Certificate* cert);

  
  
  static bool IsPublicKeyBlacklisted(const HashValueVector& public_key_hashes);

  
  
  
  static bool HasNameConstraintsViolation(
      const HashValueVector& public_key_hashes,
      const std::string& common_name,
      const std::vector<std::string>& dns_names,
      const std::vector<std::string>& ip_addrs);

  DISALLOW_COPY_AND_ASSIGN(CertVerifyProc);
};

}  

#endif  
