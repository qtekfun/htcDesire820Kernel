// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_CT_VERIFIER_H_
#define NET_CERT_CT_VERIFIER_H_

#include "net/base/net_export.h"

namespace net {

namespace ct {
struct CTVerifyResult;
}  

class BoundNetLog;
class X509Certificate;

class NET_EXPORT CTVerifier {
 public:
  virtual ~CTVerifier() {}

  
  
  
  
  
  
  
  
  
  
  virtual int Verify(X509Certificate* cert,
                     const std::string& stapled_ocsp_response,
                     const std::string& sct_list_from_tls_extension,
                     ct::CTVerifyResult* result,
                     const BoundNetLog& net_log) = 0;
};

}  

#endif  
