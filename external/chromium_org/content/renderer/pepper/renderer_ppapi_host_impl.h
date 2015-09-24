// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_RENDERER_PPAPI_HOST_IMPL_H_
#define CONTENT_RENDERER_PEPPER_RENDERER_PPAPI_HOST_IMPL_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/renderer/renderer_ppapi_host.h"
#include "content/renderer/pepper/content_renderer_pepper_host_factory.h"
#include "ppapi/host/ppapi_host.h"

namespace IPC {
class Sender;
}

namespace ppapi {

namespace proxy {
class HostDispatcher;
}

namespace thunk {
class ResourceCreationAPI;
}

}  

namespace content {

class PepperInProcessRouter;
class PepperPluginInstanceImpl;
class PluginModule;

class RendererPpapiHostImpl : public RendererPpapiHost {
 public:
  virtual ~RendererPpapiHostImpl();

  
  
  
  
  
  
  static RendererPpapiHostImpl* CreateOnModuleForOutOfProcess(
      PluginModule* module,
      ppapi::proxy::HostDispatcher* dispatcher,
      const ppapi::PpapiPermissions& permissions);
  static RendererPpapiHostImpl* CreateOnModuleForInProcess(
      PluginModule* module,
      const ppapi::PpapiPermissions& permissions);

  
  
  static RendererPpapiHostImpl* GetForPPInstance(PP_Instance pp_instance);

  
  
  
  PepperInProcessRouter* in_process_router() {
    return in_process_router_.get();
  }

  
  
  
  
  
  scoped_ptr<ppapi::thunk::ResourceCreationAPI>
      CreateInProcessResourceCreationAPI(PepperPluginInstanceImpl* instance);

  PepperPluginInstanceImpl* GetPluginInstanceImpl(PP_Instance instance) const;

  
  virtual ppapi::host::PpapiHost* GetPpapiHost() OVERRIDE;
  virtual bool IsValidInstance(PP_Instance instance) const OVERRIDE;
  virtual PepperPluginInstance* GetPluginInstance(
      PP_Instance instance) const OVERRIDE;
  virtual RenderFrame* GetRenderFrameForInstance(
      PP_Instance instance) const OVERRIDE;
  virtual RenderView* GetRenderViewForInstance(
      PP_Instance instance) const OVERRIDE;
  virtual blink::WebPluginContainer* GetContainerForInstance(
      PP_Instance instance) const OVERRIDE;
  virtual base::ProcessId GetPluginPID() const OVERRIDE;
  virtual bool HasUserGesture(PP_Instance instance) const OVERRIDE;
  virtual int GetRoutingIDForWidget(PP_Instance instance) const OVERRIDE;
  virtual gfx::Point PluginPointToRenderFrame(
      PP_Instance instance,
      const gfx::Point& pt) const OVERRIDE;
  virtual IPC::PlatformFileForTransit ShareHandleWithRemote(
      base::PlatformFile handle,
      bool should_close_source) OVERRIDE;
  virtual bool IsRunningInProcess() const OVERRIDE;
  virtual void CreateBrowserResourceHosts(
      PP_Instance instance,
      const std::vector<IPC::Message>& nested_msgs,
      const base::Callback<void(
          const std::vector<int>&)>& callback) const OVERRIDE;
  virtual GURL GetDocumentURL(PP_Instance instance) const OVERRIDE;

 private:
  RendererPpapiHostImpl(PluginModule* module,
                        ppapi::proxy::HostDispatcher* dispatcher,
                        const ppapi::PpapiPermissions& permissions);
  RendererPpapiHostImpl(PluginModule* module,
                        const ppapi::PpapiPermissions& permissions);

  
  
  
  
  
  
  PepperPluginInstanceImpl* GetAndValidateInstance(PP_Instance instance) const;

  PluginModule* module_;  

  
  
  ppapi::proxy::HostDispatcher* dispatcher_;

  scoped_ptr<ppapi::host::PpapiHost> ppapi_host_;

  
  scoped_ptr<PepperInProcessRouter> in_process_router_;

  
  bool is_running_in_process_;

  DISALLOW_COPY_AND_ASSIGN(RendererPpapiHostImpl);
};

}  

#endif  
