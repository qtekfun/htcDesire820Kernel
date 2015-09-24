// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_CERT_VERIFY_RESULT_H_
#define NET_BASE_CERT_VERIFY_RESULT_H_
#pragma once

#include <vector>

#include "net/base/x509_cert_types.h"

namespace net {

class CertVerifyResult {
 public:
  CertVerifyResult();
  ~CertVerifyResult();

  void Reset();

  
  int cert_status;

  
  bool has_md5;
  bool has_md2;
  bool has_md4;
  bool has_md5_ca;
  bool has_md2_ca;

  
  
  
  std::vector<SHA1Fingerprint> public_key_hashes;

  
  
  
  
  bool is_issued_by_known_root;
};

}  

#endif  
