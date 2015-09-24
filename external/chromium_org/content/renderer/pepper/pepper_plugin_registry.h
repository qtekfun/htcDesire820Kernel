// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PEPPER_PLUGIN_REGISTRY_H_
#define CONTENT_RENDERER_PEPPER_PEPPER_PLUGIN_REGISTRY_H_

#include <list>
#include <map>

#include "base/memory/ref_counted.h"
#include "content/public/common/pepper_plugin_info.h"

namespace content {
class PluginModule;

class PepperPluginRegistry {
 public:
  ~PepperPluginRegistry();

  static PepperPluginRegistry* GetInstance();

  
  
  
  
  const PepperPluginInfo* GetInfoForPlugin(const WebPluginInfo& info);

  
  
  
  
  PluginModule* GetLiveModule(const base::FilePath& path);

  
  
  
  
  void AddLiveModule(const base::FilePath& path, PluginModule* module);

  void PluginModuleDead(PluginModule* dead_module);

 private:
  PepperPluginRegistry();
  void Initialize();

  
  std::vector<PepperPluginInfo> plugin_list_;

  
  
  typedef std::map<base::FilePath, scoped_refptr<PluginModule> >
      OwningModuleMap;
  OwningModuleMap preloaded_modules_;

  
  
  
  
  
  
  typedef std::map<base::FilePath, PluginModule*> NonOwningModuleMap;
  NonOwningModuleMap live_modules_;

  DISALLOW_COPY_AND_ASSIGN(PepperPluginRegistry);
};

}  

#endif  
