// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_MULTI_LOG_CT_VERIFIER_H_
#define NET_CERT_MULTI_LOG_CT_VERIFIER_H_

#include <map>
#include <string>

#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/cert/ct_verifier.h"
#include "net/cert/signed_certificate_timestamp.h"

namespace net {

namespace ct {
struct LogEntry;
}  

class CTLogVerifier;

class NET_EXPORT MultiLogCTVerifier : public CTVerifier {
 public:
  MultiLogCTVerifier();
  virtual ~MultiLogCTVerifier();

  void AddLog(scoped_ptr<CTLogVerifier> log_verifier);

  
  virtual int Verify(X509Certificate* cert,
                     const std::string& stapled_ocsp_response,
                     const std::string& sct_list_from_tls_extension,
                     ct::CTVerifyResult* result,
                     const BoundNetLog& net_log) OVERRIDE;

 private:
  
  
  
  typedef std::map<std::string, linked_ptr<CTLogVerifier> > IDToLogMap;

  
  
  
  bool VerifySCTs(const std::string& encoded_sct_list,
                  const ct::LogEntry& expected_entry,
                  ct::SignedCertificateTimestamp::Origin origin,
                  ct::CTVerifyResult* result);

  
  bool VerifySingleSCT(
      scoped_refptr<ct::SignedCertificateTimestamp> sct,
      const ct::LogEntry& expected_entry,
      ct::CTVerifyResult* result);

  IDToLogMap logs_;

  DISALLOW_COPY_AND_ASSIGN(MultiLogCTVerifier);
};

}  

#endif  
