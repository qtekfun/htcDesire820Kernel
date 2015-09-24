// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_X509_UTIL_MAC_H_
#define NET_CERT_X509_UTIL_MAC_H_

#include <CoreFoundation/CFArray.h>
#include <Security/Security.h>

#include <string>

#include "base/basictypes.h"
#include "net/base/net_export.h"

namespace net {

namespace x509_util {

OSStatus NET_EXPORT CreateSSLClientPolicy(SecPolicyRef* policy);

OSStatus NET_EXPORT CreateSSLServerPolicy(const std::string& hostname,
                                          SecPolicyRef* policy);

OSStatus NET_EXPORT CreateBasicX509Policy(SecPolicyRef* policy);

OSStatus NET_EXPORT CreateRevocationPolicies(bool enable_revocation_checking,
                                             bool enable_ev_checking,
                                             CFMutableArrayRef policies);

class CSSMFieldValue {
 public:
  CSSMFieldValue();
  CSSMFieldValue(CSSM_CL_HANDLE cl_handle,
                 const CSSM_OID* oid,
                 CSSM_DATA_PTR field);
  ~CSSMFieldValue();

  CSSM_OID_PTR oid() const { return oid_; }
  CSSM_DATA_PTR field() const { return field_; }

  
  
  
  
  
  template <typename T> const T* GetAs() const {
    if (!field_ || field_->Length < sizeof(T))
      return NULL;
    return reinterpret_cast<const T*>(field_->Data);
  }

  void Reset(CSSM_CL_HANDLE cl_handle,
             CSSM_OID_PTR oid,
             CSSM_DATA_PTR field);

 private:
  CSSM_CL_HANDLE cl_handle_;
  CSSM_OID_PTR oid_;
  CSSM_DATA_PTR field_;

  DISALLOW_COPY_AND_ASSIGN(CSSMFieldValue);
};

class CSSMCachedCertificate {
 public:
  CSSMCachedCertificate();
  ~CSSMCachedCertificate();

  
  
  
  
  OSStatus Init(SecCertificateRef os_cert_handle);

  
  
  
  
  OSStatus GetField(const CSSM_OID* field_oid, CSSMFieldValue* field) const;

 private:
  CSSM_CL_HANDLE cl_handle_;
  CSSM_HANDLE cached_cert_handle_;
};

}  

}  

#endif  
