// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_SSL_INFO_H_
#define NET_BASE_SSL_INFO_H_
#pragma once

#include <vector>

#include "base/memory/ref_counted.h"
#include "net/base/net_export.h"
#include "net/base/x509_cert_types.h"

namespace net {

class X509Certificate;

class NET_EXPORT SSLInfo {
 public:
  SSLInfo();
  SSLInfo(const SSLInfo& info);
  ~SSLInfo();
  SSLInfo& operator=(const SSLInfo& info);

  void Reset();

  bool is_valid() const { return cert != NULL; }

  
  void SetCertError(int error);

  
  scoped_refptr<X509Certificate> cert;

  
  
  
  int cert_status;

  
  
  
  int security_bits;

  
  
  
  int connection_status;

  
  
  bool is_issued_by_known_root;

  
  std::vector<SHA1Fingerprint> public_key_hashes;
};

}  

#endif  
