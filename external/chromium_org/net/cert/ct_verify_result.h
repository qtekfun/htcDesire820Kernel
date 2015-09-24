// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_CT_VERIFY_RESULT_H_
#define NET_CERT_CT_VERIFY_RESULT_H_

#include <vector>

#include "net/cert/signed_certificate_timestamp.h"

namespace net {

namespace ct {

typedef std::vector<scoped_refptr<SignedCertificateTimestamp> > SCTList;

struct NET_EXPORT CTVerifyResult {
  CTVerifyResult();
  ~CTVerifyResult();

  
  SCTList verified_scts;
  
  SCTList invalid_scts;
  
  SCTList unknown_logs_scts;
};

}  

}  

#endif  
