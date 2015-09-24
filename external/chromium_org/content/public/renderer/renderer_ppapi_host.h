// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_RENDERER_PPAPI_HOST_H_
#define CONTENT_PUBLIC_RENDERER_RENDERER_PPAPI_HOST_H_

#include <vector>

#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/platform_file.h"
#include "base/process/process.h"
#include "content/common/content_export.h"
#include "ipc/ipc_platform_file.h"
#include "ppapi/c/pp_instance.h"
#include "url/gurl.h"

namespace base {
class FilePath;
}

namespace gfx {
class Point;
}

namespace IPC {
class Message;
}

namespace ppapi {
namespace host {
class PpapiHost;
}
}

namespace blink {
class WebPluginContainer;
}

namespace content {
class PepperPluginInstance;
class RenderFrame;
class RenderView;

class RendererPpapiHost {
 public:
  
  
  
  
  
  
  
  CONTENT_EXPORT static RendererPpapiHost* GetForPPInstance(
      PP_Instance instance);

  
  virtual ppapi::host::PpapiHost* GetPpapiHost() = 0;

  
  
  virtual bool IsValidInstance(PP_Instance instance) const = 0;

  
  
  
  virtual PepperPluginInstance* GetPluginInstance(
      PP_Instance instance) const = 0;

  
  
  virtual RenderFrame* GetRenderFrameForInstance(
      PP_Instance instance) const = 0;

  
  
  virtual RenderView* GetRenderViewForInstance(PP_Instance instance) const = 0;

  
  
  virtual blink::WebPluginContainer* GetContainerForInstance(
      PP_Instance instance) const = 0;

  
  
  virtual base::ProcessId GetPluginPID() const = 0;

  
  
  
  
  
  virtual bool HasUserGesture(PP_Instance instance) const = 0;

  
  
  
  
  virtual int GetRoutingIDForWidget(PP_Instance instance) const = 0;

  
  
  
  virtual gfx::Point PluginPointToRenderFrame(
      PP_Instance instance,
      const gfx::Point& pt) const = 0;

  
  
  
  
  
  
  virtual IPC::PlatformFileForTransit ShareHandleWithRemote(
      base::PlatformFile handle,
      bool should_close_source) = 0;

  
  virtual bool IsRunningInProcess() const = 0;

  
  
  
  
  
  
  
  virtual void CreateBrowserResourceHosts(
      PP_Instance instance,
      const std::vector<IPC::Message>& nested_msgs,
      const base::Callback<void(const std::vector<int>&)>& callback) const = 0;

  
  
  
  
  
  virtual GURL GetDocumentURL(PP_Instance instance) const = 0;

 protected:
  virtual ~RendererPpapiHost() {}
};

}  

#endif  
