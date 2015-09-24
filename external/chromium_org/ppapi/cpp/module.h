// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_MODULE_H_
#define PPAPI_CPP_MODULE_H_

#include <map>
#include <string>

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/ppb.h"
#include "ppapi/c/ppb_core.h"
#include "ppapi/cpp/core.h"


namespace pp {

class Instance;

class Module {
 public:
  typedef std::map<PP_Instance, Instance*> InstanceMap;

  
  
  
  Module();
  virtual ~Module();

  
  
  
  
  static Module* Get();

  
  
  
  
  
  virtual bool Init();

  
  
  
  PP_Module pp_module() const { return pp_module_; }

  
  
  
  
  
  
  PPB_GetInterface get_browser_interface() const {
    return get_browser_interface_;
  }

  
  
  
  
  
  
  
  Core* core() { return core_; }

  
  
  
  
  
  const void* GetPluginInterface(const char* interface_name);

  
  
  
  const void* GetBrowserInterface(const char* interface_name);

  
  
  
  
  
  
  
  
  
  
  Instance* InstanceForPPInstance(PP_Instance instance);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void AddPluginInterface(const std::string& interface_name,
                          const void* vtable);

  
  
  
  
  
  
  
  
  
  bool InternalInit(PP_Module mod,
                    PPB_GetInterface get_browser_interface);

  
  
  
  
  const InstanceMap& current_instances() const { return current_instances_; }

 protected:
  
  
  
  
  
  virtual Instance* CreateInstance(PP_Instance instance) = 0;

 private:
  friend PP_Bool Instance_DidCreate(PP_Instance pp_instance,
                                    uint32_t argc,
                                    const char* argn[],
                                    const char* argv[]);
  friend void Instance_DidDestroy(PP_Instance instance);

  
  Module(const Module&);
  Module& operator=(const Module&);

  
  InstanceMap current_instances_;

  PP_Module pp_module_;
  PPB_GetInterface get_browser_interface_;

  Core* core_;

  
  
  typedef std::map<std::string, const void*> InterfaceMap;
  InterfaceMap additional_interfaces_;
};

}  

#endif  
