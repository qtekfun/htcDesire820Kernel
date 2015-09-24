// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PORT_BROWSER_WEB_CONTENTS_VIEW_PORT_H_
#define CONTENT_PORT_BROWSER_WEB_CONTENTS_VIEW_PORT_H_

#include "content/public/browser/web_contents_view.h"

namespace content {
class RenderViewHost;
class RenderWidgetHost;
class RenderWidgetHostView;

class CONTENT_EXPORT WebContentsViewPort : public WebContentsView {
 public:
  virtual ~WebContentsViewPort() {}

  virtual void CreateView(
      const gfx::Size& initial_size, gfx::NativeView context) = 0;

  
  
  
  virtual RenderWidgetHostView* CreateViewForWidget(
      RenderWidgetHost* render_widget_host) = 0;

  
  virtual RenderWidgetHostView* CreateViewForPopupWidget(
      RenderWidgetHost* render_widget_host) = 0;

  
  
  
  
  virtual void SetPageTitle(const base::string16& title) = 0;

  
  
  virtual void RenderViewCreated(RenderViewHost* host) = 0;

  
  
  virtual void RenderViewSwappedIn(RenderViewHost* host) = 0;

  
  virtual void SetOverscrollControllerEnabled(bool enabled) = 0;

#if defined(OS_MACOSX)
  
  
  
  
  
  virtual bool IsEventTracking() const = 0;
  virtual void CloseTabAfterEventTracking() = 0;
#endif
};

}  

#endif  
