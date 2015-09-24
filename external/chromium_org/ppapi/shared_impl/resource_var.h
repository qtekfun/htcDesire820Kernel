// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_RESOURCE_VAR_H_
#define PPAPI_SHARED_IMPL_RESOURCE_VAR_H_

#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"
#include "ppapi/shared_impl/var.h"

namespace IPC {
class Message;
}

namespace ppapi {

class PPAPI_SHARED_EXPORT ResourceVar : public Var {
 public:
  
  
  
  
  virtual PP_Resource GetPPResource() const = 0;

  
  virtual int GetPendingRendererHostId() const;

  
  virtual int GetPendingBrowserHostId() const;

  
  
  virtual const IPC::Message* GetCreationMessage() const;

  
  
  
  virtual bool IsPending() const = 0;

  
  virtual ResourceVar* AsResourceVar() OVERRIDE;
  virtual PP_VarType GetType() const OVERRIDE;

  
  
  static ResourceVar* FromPPVar(PP_Var var);

 protected:
  ResourceVar();

  virtual ~ResourceVar();

 private:
  DISALLOW_COPY_AND_ASSIGN(ResourceVar);
};

}  

#endif  
