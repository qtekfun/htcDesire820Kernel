// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_FRAUDULENT_CERTIFICATE_REPORTER_H_
#define NET_URL_REQUEST_FRAUDULENT_CERTIFICATE_REPORTER_H_

#include <string>

namespace net {

class SSLInfo;

class FraudulentCertificateReporter {
 public:
  virtual ~FraudulentCertificateReporter() {}

  
  
  
  
  virtual void SendReport(const std::string& hostname,
                          const SSLInfo& ssl_info,
                          bool sni_available) = 0;
};

}  

#endif  

