// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_BROWSER_PLUGIN_BROWSER_PLUGIN_POPUP_MENU_HELPER_MAC_H_
#define CONTENT_BROWSER_BROWSER_PLUGIN_BROWSER_PLUGIN_POPUP_MENU_HELPER_MAC_H_

#include "content/browser/renderer_host/popup_menu_helper_mac.h"

namespace content {
class RenderViewHost;
class RenderViewHostImpl;

class BrowserPluginPopupMenuHelper : public PopupMenuHelper {
 public:
  
  
  
  BrowserPluginPopupMenuHelper(RenderViewHost* embedder_rvh,
                               RenderViewHost* guest_rvh);

 private:
  virtual RenderWidgetHostViewMac* GetRenderWidgetHostView() const OVERRIDE;

  RenderViewHostImpl* embedder_rvh_;

  DISALLOW_COPY_AND_ASSIGN(BrowserPluginPopupMenuHelper);
};

}  

#endif  
