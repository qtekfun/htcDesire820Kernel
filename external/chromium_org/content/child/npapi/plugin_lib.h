// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_NPAPI_PLUGIN_LIB_H_
#define CONTENT_CHILD_NPAPI_PLUGIN_LIB_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/native_library.h"
#include "build/build_config.h"
#include "content/child/npapi/webplugin.h"
#include "content/common/content_export.h"
#include "content/public/common/webplugininfo.h"
#include "third_party/npapi/bindings/nphostapi.h"

namespace base {
class FilePath;
}

namespace content {

class PluginInstance;

struct PluginEntryPoints {
#if !defined(OS_POSIX) || defined(OS_MACOSX)
  NP_GetEntryPointsFunc np_getentrypoints;
#endif
  NP_InitializeFunc np_initialize;
  NP_ShutdownFunc np_shutdown;
};

class CONTENT_EXPORT PluginLib : public base::RefCounted<PluginLib> {
 public:
  static PluginLib* CreatePluginLib(const base::FilePath& filename);

  
  static void UnloadAllPlugins();

  
  static void ShutdownAllPlugins();

  
  NPPluginFuncs* functions();

  
  PluginInstance* CreateInstance(const std::string& mime_type);

  
  void CloseInstance();

  
  
  const WebPluginInfo& plugin_info() { return web_plugin_info_; }

  
  
  

  
  
  NPError NP_Initialize();

  
  void NP_Shutdown(void);

  
  NPError NP_ClearSiteData(const char* site, uint64 flags, uint64 max_age);

  
  
  char** NP_GetSitesWithData();

  int instance_count() const { return instance_count_; }

  
  
  void PreventLibraryUnload();

  
  void set_defer_unload(bool defer_unload) {
    defer_unload_ = defer_unload;
  }

  
 protected:
  friend class base::RefCounted<PluginLib>;

  
  explicit PluginLib(const WebPluginInfo& info);

  virtual ~PluginLib();

  
  
  bool Load();

  
  void Unload();

  
  void Shutdown();

 private:
  WebPluginInfo web_plugin_info_;  
  base::NativeLibrary library_;  
  NPPluginFuncs plugin_funcs_;  
  bool initialized_;  
  NPSavedData *saved_data_;  
  int instance_count_;  
  bool skip_unload_;  

  
  PluginEntryPoints entry_points_;

  
  bool defer_unload_;

  DISALLOW_COPY_AND_ASSIGN(PluginLib);
};

}  

#endif  
