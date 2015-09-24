// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_NPOBJECT_VAR_H_
#define CONTENT_RENDERER_PEPPER_NPOBJECT_VAR_H_

#include <string>

#include "base/compiler_specific.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/shared_impl/var.h"
#include "content/common/content_export.h"

typedef struct NPObject NPObject;
typedef struct _NPVariant NPVariant;
typedef void* NPIdentifier;

namespace ppapi {


class NPObjectVar : public Var {
 public:
  
  
  
  NPObjectVar(PP_Instance instance, NPObject* np_object);

  
  virtual NPObjectVar* AsNPObjectVar() OVERRIDE;
  virtual PP_VarType GetType() const OVERRIDE;

  
  
  NPObject* np_object() const { return np_object_; }

  
  
  void InstanceDeleted();

  
  PP_Instance pp_instance() const { return pp_instance_; }

  
  
  CONTENT_EXPORT static scoped_refptr<NPObjectVar> FromPPVar(PP_Var var);

 private:
  virtual ~NPObjectVar();

  
  PP_Instance pp_instance_;

  
  
  NPObject* np_object_;

  DISALLOW_COPY_AND_ASSIGN(NPObjectVar);
};

}  

#endif  
