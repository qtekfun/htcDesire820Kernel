// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_CERT_TRUST_ANCHOR_PROVIDER_H_
#define NET_CERT_CERT_TRUST_ANCHOR_PROVIDER_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "net/base/net_export.h"

namespace net {

class X509Certificate;
typedef std::vector<scoped_refptr<X509Certificate> > CertificateList;

class NET_EXPORT CertTrustAnchorProvider {
 public:
  virtual ~CertTrustAnchorProvider() {}

  
  
  
  virtual const CertificateList& GetAdditionalTrustAnchors() = 0;
};

}  

#endif  
