// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PLUGIN_MODULE_H_
#define CONTENT_RENDERER_PEPPER_PLUGIN_MODULE_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/native_library.h"
#include "base/process/process.h"
#include "content/common/content_export.h"
#include "content/public/common/pepper_plugin_info.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/ppb_core.h"
#include "ppapi/c/private/ppb_instance_private.h"
#include "ppapi/shared_impl/ppapi_permissions.h"

typedef void* NPIdentifier;

class GURL;

namespace base {
class FilePath;
}

namespace ppapi {
class CallbackTracker;
class WebKitForwarding;
}  

namespace IPC {
struct ChannelHandle;
}

namespace blink {
class WebPluginContainer;
}  

namespace content {
class HostDispatcherWrapper;
class PepperPluginInstanceImpl;
class PepperBroker;
class RendererPpapiHostImpl;
class RenderFrameImpl;
struct WebPluginInfo;

class CONTENT_EXPORT PluginModule :
    public base::RefCounted<PluginModule>,
    public base::SupportsWeakPtr<PluginModule> {
 public:
  typedef std::set<PepperPluginInstanceImpl*> PluginInstanceSet;

  
  
  
  
  
  
  PluginModule(const std::string& name,
               const base::FilePath& path,
               const ppapi::PpapiPermissions& perms);

  
  
  
  void SetRendererPpapiHost(scoped_ptr<RendererPpapiHostImpl> host);

  
  
  
  bool InitAsInternalPlugin(const PepperPluginInfo::EntryPoints& entry_points);

  
  
  bool InitAsLibrary(const base::FilePath& path);

  
  
  void InitAsProxied(HostDispatcherWrapper* host_dispatcher_wrapper);

  
  
  
  scoped_refptr<PluginModule> CreateModuleForExternalPluginInstance();

  
  
  
  
  PP_ExternalPluginResult InitAsProxiedExternalPlugin(
      PepperPluginInstanceImpl* instance);

  bool IsProxied() const;

  
  
  base::ProcessId GetPeerProcessId();

  
  
  
  
  
  int GetPluginChildId();

  static const PPB_Core* GetCore();

  
  
  
  
  
  static bool SupportsInterface(const char* name);

  RendererPpapiHostImpl* renderer_ppapi_host() {
    return renderer_ppapi_host_.get();
  }

  
  
  PP_Module pp_module() const { return pp_module_; }

  const std::string& name() const { return name_; }
  const base::FilePath& path() const { return path_; }
  const ppapi::PpapiPermissions& permissions() const { return permissions_; }

  PepperPluginInstanceImpl* CreateInstance(
      RenderFrameImpl* render_frame,
      blink::WebPluginContainer* container,
      const GURL& plugin_url);

  
  
  
  
  PepperPluginInstanceImpl* GetSomeInstance() const;

  const PluginInstanceSet& GetAllInstances() const { return instances_; }

  
  
  const void* GetPluginInterface(const char* name) const;

  
  
  
  void InstanceCreated(PepperPluginInstanceImpl* instance);
  void InstanceDeleted(PepperPluginInstanceImpl* instance);

  scoped_refptr<ppapi::CallbackTracker> GetCallbackTracker();

  
  
  void PluginCrashed();

  bool is_in_destructor() const { return is_in_destructor_; }
  bool is_crashed() const { return is_crashed_; }

  
  
  
  
  
  
  
  
  void SetReserveInstanceIDCallback(
      PP_Bool (*reserve)(PP_Module, PP_Instance));
  bool ReserveInstanceID(PP_Instance instance);

  
  void SetBroker(PepperBroker* broker);
  PepperBroker* GetBroker();

  
  
  RendererPpapiHostImpl* CreateOutOfProcessModule(
      RenderFrameImpl* render_frame,
      const base::FilePath& path,
      ppapi::PpapiPermissions permissions,
      const IPC::ChannelHandle& channel_handle,
      base::ProcessId plugin_pid,
      int plugin_child_id,
      bool is_external);

  
  
  
  
  static void ResetHostGlobalsForTest();

  
  
  
  
  
  
  
  
  
  static scoped_refptr<PluginModule> Create(RenderFrameImpl* render_frame,
                                            const WebPluginInfo& webplugin_info,
                                            bool* pepper_plugin_was_registered);

 private:
  friend class base::RefCounted<PluginModule>;
  ~PluginModule();
  
  
  
  bool InitializeModule(const PepperPluginInfo::EntryPoints& entry_points);

  scoped_ptr<RendererPpapiHostImpl> renderer_ppapi_host_;

  
  
  scoped_refptr<ppapi::CallbackTracker> callback_tracker_;

  PP_Module pp_module_;

  
  
  bool is_in_destructor_;

  
  bool is_crashed_;

  
  
  
  scoped_ptr<HostDispatcherWrapper> host_dispatcher_wrapper_;

  
  
  PepperBroker* broker_;

  
  
  
  
  base::NativeLibrary library_;

  
  
  
  PepperPluginInfo::EntryPoints entry_points_;

  
  const std::string name_;
  const base::FilePath path_;

  ppapi::PpapiPermissions permissions_;

  
  
  PluginInstanceSet instances_;

  PP_Bool (*reserve_instance_id_)(PP_Module, PP_Instance);

  DISALLOW_COPY_AND_ASSIGN(PluginModule);
};

}  

#endif  
