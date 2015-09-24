// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SSL_SSL_CLIENT_CERT_TYPE_H_
#define NET_SSL_SSL_CLIENT_CERT_TYPE_H_

namespace net {

enum SSLClientCertType {
  CLIENT_CERT_RSA_SIGN = 1,
  CLIENT_CERT_DSS_SIGN = 2,
  CLIENT_CERT_ECDSA_SIGN = 64,
  
  CLIENT_CERT_INVALID_TYPE = 255,
};

}  

#endif  
