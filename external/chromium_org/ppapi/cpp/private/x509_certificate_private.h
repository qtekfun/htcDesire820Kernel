// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_PRIVATE_X509_CERTIFICATE_PRIVATE_H_
#define PPAPI_CPP_PRIVATE_X509_CERTIFICATE_PRIVATE_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/private/ppb_x509_certificate_private.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class Var;

class X509CertificatePrivate : public Resource {
 public:
  
  X509CertificatePrivate();
  X509CertificatePrivate(PassRef, PP_Resource resource);
  explicit X509CertificatePrivate(const InstanceHandle& instance);

  
  static bool IsAvailable();

  
  
  bool Initialize(const char* bytes, uint32_t length);
  
  Var GetField(PP_X509Certificate_Private_Field field) const;
};

}  

#endif  
