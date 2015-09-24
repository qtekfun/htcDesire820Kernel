// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_RENDER_VIEW_HOST_FACTORY_H_
#define CONTENT_BROWSER_RENDERER_HOST_RENDER_VIEW_HOST_FACTORY_H_

#include "base/basictypes.h"
#include "content/common/content_export.h"

namespace content {
class RenderFrameHostDelegate;
class RenderViewHost;
class RenderViewHostDelegate;
class RenderWidgetHostDelegate;
class SessionStorageNamespace;
class SiteInstance;

class RenderViewHostFactory {
 public:
  
  
  
  static RenderViewHost* Create(
      SiteInstance* instance,
      RenderViewHostDelegate* delegate,
      RenderFrameHostDelegate* frame_delegate,
      RenderWidgetHostDelegate* widget_delegate,
      int routing_id,
      int main_frame_routing_id,
      bool swapped_out,
      bool hidden);

  
  static bool has_factory() {
    return !!factory_;
  }

 protected:
  RenderViewHostFactory() {}
  virtual ~RenderViewHostFactory() {}

  
  
  virtual RenderViewHost* CreateRenderViewHost(
      SiteInstance* instance,
      RenderViewHostDelegate* delegate,
      RenderFrameHostDelegate* frame_delegate,
      RenderWidgetHostDelegate* widget_delegate,
      int routing_id,
      int main_frame_routing_id,
      bool swapped_out) = 0;

  
  
  
  CONTENT_EXPORT static void RegisterFactory(RenderViewHostFactory* factory);

  
  
  CONTENT_EXPORT static void UnregisterFactory();

 private:
  
  
  CONTENT_EXPORT static RenderViewHostFactory* factory_;

  DISALLOW_COPY_AND_ASSIGN(RenderViewHostFactory);
};

}  

#endif  
