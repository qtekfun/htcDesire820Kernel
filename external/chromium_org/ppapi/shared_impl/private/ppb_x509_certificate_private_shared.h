// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_PRIVATE_PPB_X509_CERTIFICATE_PRIVATE_IMPL_H_
#define PPAPI_SHARED_IMPL_PRIVATE_PPB_X509_CERTIFICATE_PRIVATE_IMPL_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "ppapi/c/private/ppb_x509_certificate_private.h"
#include "ppapi/shared_impl/resource.h"
#include "ppapi/thunk/ppb_x509_certificate_private_api.h"

namespace IPC {
template <class T>
struct ParamTraits;
}

namespace ppapi {

class PPAPI_SHARED_EXPORT PPB_X509Certificate_Fields {
 public:
  PPB_X509Certificate_Fields();
  PPB_X509Certificate_Fields(const PPB_X509Certificate_Fields& fields);

  
  void SetField(PP_X509Certificate_Private_Field field, base::Value* value);
  PP_Var GetFieldAsPPVar(PP_X509Certificate_Private_Field field) const;

 private:
  
  friend struct IPC::ParamTraits<ppapi::PPB_X509Certificate_Fields>;

  base::ListValue values_;
};


class PPAPI_SHARED_EXPORT PPB_X509Certificate_Private_Shared
    : public thunk::PPB_X509Certificate_Private_API,
      public Resource {
 public:
  PPB_X509Certificate_Private_Shared(ResourceObjectType type,
                                     PP_Instance instance);
  
  
  PPB_X509Certificate_Private_Shared(ResourceObjectType type,
                                     PP_Instance instance,
                                     const PPB_X509Certificate_Fields& fields);
  virtual ~PPB_X509Certificate_Private_Shared();

  
  virtual PPB_X509Certificate_Private_API*
      AsPPB_X509Certificate_Private_API() OVERRIDE;

  
  virtual PP_Bool Initialize(const char* bytes, uint32_t length) OVERRIDE;
  virtual PP_Var GetField(PP_X509Certificate_Private_Field field) OVERRIDE;

 protected:
  virtual bool ParseDER(const std::vector<char>& der,
                        PPB_X509Certificate_Fields* result);

 private:
  scoped_ptr<PPB_X509Certificate_Fields> fields_;

  DISALLOW_COPY_AND_ASSIGN(PPB_X509Certificate_Private_Shared);
};

}  

#endif  
