// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CONSTRAINED_WEB_DIALOG_UI_H_
#define CHROME_BROWSER_UI_WEBUI_CONSTRAINED_WEB_DIALOG_UI_H_

#include "base/compiler_specific.h"
#include "components/web_modal/native_web_contents_modal_dialog.h"
#include "content/public/browser/web_ui_controller.h"
#include "ui/gfx/native_widget_types.h"

namespace content {
class BrowserContext;
class RenderViewHost;
class WebContents;
}

namespace ui {
class WebDialogDelegate;
class WebDialogWebContentsDelegate;
}

class ConstrainedWebDialogDelegate {
 public:
  virtual const ui::WebDialogDelegate* GetWebDialogDelegate() const = 0;
  virtual ui::WebDialogDelegate* GetWebDialogDelegate() = 0;

  
  
  virtual void OnDialogCloseFromWebUI() = 0;

  
  
  
  virtual void ReleaseWebContentsOnDialogClose() = 0;

  
  virtual content::WebContents* GetWebContents() = 0;

  
  virtual web_modal::NativeWebContentsModalDialog GetNativeDialog() = 0;

 protected:
  virtual ~ConstrainedWebDialogDelegate() {}
};

class ConstrainedWebDialogUI
    : public content::WebUIController {
 public:
  explicit ConstrainedWebDialogUI(content::WebUI* web_ui);
  virtual ~ConstrainedWebDialogUI();

  
  virtual void RenderViewCreated(
      content::RenderViewHost* render_view_host) OVERRIDE;

  
  static void SetConstrainedDelegate(content::WebContents* web_contents,
                                     ConstrainedWebDialogDelegate* delegate);

 protected:
  
  
  ConstrainedWebDialogDelegate* GetConstrainedDelegate();

 private:
  
  void OnDialogCloseMessage(const base::ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(ConstrainedWebDialogUI);
};

ConstrainedWebDialogDelegate* CreateConstrainedWebDialog(
    content::BrowserContext* browser_context,
    ui::WebDialogDelegate* delegate,
    ui::WebDialogWebContentsDelegate* tab_delegate,
    content::WebContents* overshadowed);

#endif  
