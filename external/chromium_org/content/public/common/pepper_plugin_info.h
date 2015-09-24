// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_PEPPER_PLUGIN_INFO_H_
#define CONTENT_PUBLIC_COMMON_PEPPER_PLUGIN_INFO_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "content/common/content_export.h"
#include "content/public/common/webplugininfo.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/ppb.h"

namespace content {

struct CONTENT_EXPORT PepperPluginInfo {
  typedef const void* (*GetInterfaceFunc)(const char*);
  typedef int (*PPP_InitializeModuleFunc)(PP_Module, PPB_GetInterface);
  typedef void (*PPP_ShutdownModuleFunc)();

  struct EntryPoints {
    
    CONTENT_EXPORT EntryPoints();

    GetInterfaceFunc get_interface;
    PPP_InitializeModuleFunc initialize_module;
    PPP_ShutdownModuleFunc shutdown_module;  
  };

  PepperPluginInfo();
  ~PepperPluginInfo();

  WebPluginInfo ToWebPluginInfo() const;

  
  
  
  
  bool is_internal;

  
  bool is_out_of_process;

  
  
  bool is_sandboxed;

  base::FilePath path;  
  std::string name;
  std::string description;
  std::string version;
  std::vector<WebPluginMimeType> mime_types;

  
  
  EntryPoints internal_entry_points;

  
  uint32 permissions;
};

}  

#endif  
