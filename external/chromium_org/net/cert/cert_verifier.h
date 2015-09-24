// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_CERT_VERIFIER_H_
#define NET_CERT_CERT_VERIFIER_H_

#include <string>

#include "base/basictypes.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"

namespace net {

class BoundNetLog;
class CertVerifyResult;
class CRLSet;
class X509Certificate;

class NET_EXPORT CertVerifier {
 public:
  
  typedef void* RequestHandle;

  enum VerifyFlags {
    
    
    VERIFY_REV_CHECKING_ENABLED = 1 << 0,

    
    
    
    
    
    
    
    
    VERIFY_EV_CERT = 1 << 1,

    
    
    
    
    
    VERIFY_CERT_IO_ENABLED = 1 << 2,

    
    
    
    VERIFY_REV_CHECKING_ENABLED_EV_ONLY = 1 << 3,

    
    
    
    
    
    
    
    
    
    VERIFY_REV_CHECKING_REQUIRED_LOCAL_ANCHORS = 1 << 4,
  };

  
  
  virtual ~CertVerifier() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Verify(X509Certificate* cert,
                     const std::string& hostname,
                     int flags,
                     CRLSet* crl_set,
                     CertVerifyResult* verify_result,
                     const CompletionCallback& callback,
                     RequestHandle* out_req,
                     const BoundNetLog& net_log) = 0;

  
  
  virtual void CancelRequest(RequestHandle req) = 0;

  
  
  static CertVerifier* CreateDefault();
};

}  

#endif  
