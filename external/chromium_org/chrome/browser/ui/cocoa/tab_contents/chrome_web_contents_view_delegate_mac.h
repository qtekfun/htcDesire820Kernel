// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_CHROME_WEB_CONTENTS_VIEW_DELEGATE_MAC_H_
#define CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_CHROME_WEB_CONTENTS_VIEW_DELEGATE_MAC_H_

#if defined(__OBJC__)

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/web_contents_view_delegate.h"

class RenderViewContextMenuMac;
class WebDragBookmarkHandlerMac;

namespace content {
class WebContents;
}

class ChromeWebContentsViewDelegateMac
    : public content::WebContentsViewDelegate {
 public:
  explicit ChromeWebContentsViewDelegateMac(content::WebContents* web_contents);
  virtual ~ChromeWebContentsViewDelegateMac();

  
  virtual NSObject<RenderWidgetHostViewMacDelegate>*
      CreateRenderWidgetHostViewDelegate(
          content::RenderWidgetHost* render_widget_host) OVERRIDE;
  virtual content::WebDragDestDelegate* GetDragDestDelegate() OVERRIDE;
  virtual void ShowContextMenu(
      const content::ContextMenuParams& params) OVERRIDE;

 private:
  
  scoped_ptr<RenderViewContextMenuMac> context_menu_;

  
  scoped_ptr<WebDragBookmarkHandlerMac> bookmark_handler_;

  
  content::WebContents* web_contents_;

  DISALLOW_COPY_AND_ASSIGN(ChromeWebContentsViewDelegateMac);
};

#endif  

#endif  
