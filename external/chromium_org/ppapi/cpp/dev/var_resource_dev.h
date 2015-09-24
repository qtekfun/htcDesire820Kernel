// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_VAR_RESOURCE_DEV_H_
#define PPAPI_CPP_VAR_RESOURCE_DEV_H_

#include "ppapi/cpp/resource.h"
#include "ppapi/cpp/var.h"


namespace pp {

class VarResource_Dev : public Var {
 public:
  
  explicit VarResource_Dev(const pp::Resource& resource);

  
  
  
  
  
  explicit VarResource_Dev(const Var& var);

  
  VarResource_Dev(const VarResource_Dev& other);

  virtual ~VarResource_Dev();

  
  VarResource_Dev& operator=(const VarResource_Dev& other);

  
  
  
  
  
  
  
  virtual Var& operator=(const Var& other);

  
  
  
  pp::Resource AsResource();
};

}  

#endif  
