// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TEST_CERT_TEST_UTIL_H_
#define NET_TEST_CERT_TEST_UTIL_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "net/cert/x509_cert_types.h"
#include "net/cert/x509_certificate.h"

namespace base {
class FilePath;
}

namespace net {

class EVRootCAMetadata;

CertificateList CreateCertificateListFromFile(const base::FilePath& certs_dir,
                                              const std::string& cert_file,
                                              int format);

scoped_refptr<X509Certificate> CreateCertificateChainFromFile(
    const base::FilePath& certs_dir,
    const std::string& cert_file,
    int format);

scoped_refptr<X509Certificate> ImportCertFromFile(const base::FilePath& certs_dir,
                                                  const std::string& cert_file);

class ScopedTestEVPolicy {
 public:
  ScopedTestEVPolicy(EVRootCAMetadata* ev_root_ca_metadata,
                     const SHA1HashValue& fingerprint,
                     const char* policy);
  ~ScopedTestEVPolicy();

 private:
  SHA1HashValue fingerprint_;
  EVRootCAMetadata* const ev_root_ca_metadata_;
};

}  

#endif  
