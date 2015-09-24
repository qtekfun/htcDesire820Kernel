// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_VAR_SERIALIZATION_RULES_H_
#define PPAPI_PROXY_VAR_SERIALIZATION_RULES_H_

#include "base/memory/ref_counted.h"
#include "ppapi/c/pp_var.h"

#include <string>

namespace ppapi {
namespace proxy {

class VarSerializationRules : public base::RefCounted<VarSerializationRules> {
 public:
  
  
  
  
  

  
  
  
  virtual PP_Var SendCallerOwned(const PP_Var& var) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  virtual PP_Var BeginReceiveCallerOwned(const PP_Var& var) = 0;
  virtual void EndReceiveCallerOwned(const PP_Var& var) = 0;

  
  
  
  
  
  
  
  
  

  
  
  
  
  
  virtual PP_Var ReceivePassRef(const PP_Var& var) = 0;

  
  
  
  
  
  
  
  virtual PP_Var BeginSendPassRef(const PP_Var& var) = 0;
  virtual void EndSendPassRef(const PP_Var& var) = 0;

  

  virtual void ReleaseObjectRef(const PP_Var& var) = 0;

 protected:
  VarSerializationRules() {}
  virtual ~VarSerializationRules() {}

 private:
  friend class base::RefCounted<VarSerializationRules>;
};

}  
}  

#endif  
