// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_PRIVATE_INSTANCE_PRIVATE_H_
#define PPAPI_CPP_PRIVATE_INSTANCE_PRIVATE_H_


#include "ppapi/c/ppb_console.h"
#include "ppapi/cpp/instance.h"

namespace pp {

class Var;
class VarPrivate;

class InstancePrivate : public Instance {
 public:
  explicit InstancePrivate(PP_Instance instance);
  virtual ~InstancePrivate();

  
  

  
  virtual Var GetInstanceObject();

  

  
  

  
  VarPrivate GetWindowObject();

  
  VarPrivate GetOwnerElementObject();

  
  VarPrivate ExecuteScript(const Var& script, Var* exception = NULL);

  
};

}  

#endif  
