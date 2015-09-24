// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PLUGIN_RESOURCE_VAR_H_
#define PPAPI_PROXY_PLUGIN_RESOURCE_VAR_H_

#include "ppapi/c/pp_resource.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/shared_impl/resource.h"
#include "ppapi/shared_impl/resource_var.h"
#include "ppapi/shared_impl/var.h"

class PPAPI_PROXY_EXPORT PluginResourceVar : public ppapi::ResourceVar {
 public:
  
  PluginResourceVar();

  
  
  explicit PluginResourceVar(ppapi::Resource* resource);

  
  virtual PP_Resource GetPPResource() const OVERRIDE;
  virtual bool IsPending() const OVERRIDE;

  scoped_refptr<ppapi::Resource> resource() const { return resource_; }

 protected:
  virtual ~PluginResourceVar();

 private:
  
  scoped_refptr<ppapi::Resource> resource_;

  DISALLOW_COPY_AND_ASSIGN(PluginResourceVar);
};

#endif
