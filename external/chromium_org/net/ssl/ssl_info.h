// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SSL_SSL_INFO_H_
#define NET_SSL_SSL_INFO_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "net/base/net_export.h"
#include "net/cert/cert_status_flags.h"
#include "net/cert/sct_status_flags.h"
#include "net/cert/x509_cert_types.h"
#include "net/ssl/signed_certificate_timestamp_and_status.h"

class Pickle;
class PickleIterator;

namespace net {

class X509Certificate;

class NET_EXPORT SSLInfo {
 public:
  
  
  enum HandshakeType {
    HANDSHAKE_UNKNOWN = 0,
    HANDSHAKE_RESUME,  
    HANDSHAKE_FULL,  
  };

  SSLInfo();
  SSLInfo(const SSLInfo& info);
  ~SSLInfo();
  SSLInfo& operator=(const SSLInfo& info);

  void Reset();

  bool is_valid() const { return cert.get() != NULL; }

  
  void SetCertError(int error);

  
  scoped_refptr<X509Certificate> cert;

  
  
  
  CertStatus cert_status;

  
  
  
  int security_bits;

  
  
  
  int connection_status;

  
  
  bool is_issued_by_known_root;

  
  
  bool client_cert_sent;

  
  bool channel_id_sent;

  HandshakeType handshake_type;

  
  
  HashValueVector public_key_hashes;

  
  
  SignedCertificateTimestampAndStatusList signed_certificate_timestamps;
};

}  

#endif  
