// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_RENDER_FRAME_H_
#define CONTENT_PUBLIC_RENDERER_RENDER_FRAME_H_

#include "content/common/content_export.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"
#include "third_party/WebKit/public/web/WebNavigationPolicy.h"

struct WebPreferences;

namespace blink {
class WebFrame;
class WebPlugin;
class WebURLRequest;
struct WebPluginParams;
}

namespace content {
class ContextMenuClient;
class RenderView;
struct ContextMenuParams;
struct WebPluginInfo;

class CONTENT_EXPORT RenderFrame : public IPC::Listener,
                                   public IPC::Sender {
 public:
  
  virtual RenderView* GetRenderView() = 0;

  
  virtual int GetRoutingID() = 0;

   
  virtual WebPreferences& GetWebkitPreferences() = 0;

  
  
  
  
  
  
  
  
  
  
  
  virtual int ShowContextMenu(ContextMenuClient* client,
                              const ContextMenuParams& params) = 0;

  
  
  virtual void CancelContextMenu(int request_id) = 0;

  
  
  virtual blink::WebPlugin* CreatePlugin(
      blink::WebFrame* frame,
      const WebPluginInfo& info,
      const blink::WebPluginParams& params) = 0;

  
  virtual void LoadURLExternally(
      blink::WebFrame* frame,
      const blink::WebURLRequest& request,
      blink::WebNavigationPolicy policy) = 0;

 protected:
  virtual ~RenderFrame() {}

 private:
  
  friend class RenderFrameImpl;
  RenderFrame() {}
};

}  

#endif  
