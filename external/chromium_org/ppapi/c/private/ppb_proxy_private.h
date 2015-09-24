// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PRIVATE_PROXY_PRIVATE_H_
#define PPAPI_C_PRIVATE_PROXY_PRIVATE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_resource.h"

#define PPB_PROXY_PRIVATE_INTERFACE "PPB_Proxy_Private;6"

struct PPB_Proxy_Private {
  
  void (*PluginCrashed)(PP_Module module);

  
  PP_Instance (*GetInstanceForResource)(PP_Resource resource);

  
  
  
  
  
  
  
  
  
  void (*SetReserveInstanceIDCallback)(
      PP_Module module,
      PP_Bool (*is_seen)(PP_Module, PP_Instance));

  
  
  
  void (*AddRefModule)(PP_Module module);
  void (*ReleaseModule)(PP_Module module);

  // Allows asserts to be written for some bad conditions while cleaning up.
  PP_Bool (*IsInModuleDestructor)(PP_Module module);
};

#endif  
