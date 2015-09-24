// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_SSL_CERT_REQUEST_INFO_H_
#define NET_BASE_SSL_CERT_REQUEST_INFO_H_
#pragma once

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "net/base/net_export.h"

namespace net {

class X509Certificate;

class NET_EXPORT SSLCertRequestInfo
    : public base::RefCountedThreadSafe<SSLCertRequestInfo> {
 public:
  SSLCertRequestInfo();

  void Reset();

  
  std::string host_and_port;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  std::vector<scoped_refptr<X509Certificate> > client_certs;

 private:
  friend class base::RefCountedThreadSafe<SSLCertRequestInfo>;

  ~SSLCertRequestInfo();
};

}  

#endif  
