// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_CERT_VERIFY_RESULT_H_
#define NET_CERT_CERT_VERIFY_RESULT_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "net/base/net_export.h"
#include "net/cert/cert_status_flags.h"
#include "net/cert/x509_cert_types.h"

namespace net {

class X509Certificate;

class NET_EXPORT CertVerifyResult {
 public:
  CertVerifyResult();
  ~CertVerifyResult();

  void Reset();

  
  void CopyFrom(const CertVerifyResult& other) {
    *this = other;
  }

  
  
  
  
  
  
  scoped_refptr<X509Certificate> verified_cert;

  
  
  
  
  CertStatus cert_status;

  
  bool has_md5;
  bool has_md2;
  bool has_md4;

  
  
  
  HashValueVector public_key_hashes;

  
  
  
  
  bool is_issued_by_known_root;

  
  
  bool is_issued_by_additional_trust_anchor;

  
  
  bool common_name_fallback_used;
};

}  

#endif  
