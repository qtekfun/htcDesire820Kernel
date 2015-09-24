// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_WEB_DIALOGS_WEB_DIALOG_UI_H_
#define UI_WEB_DIALOGS_WEB_DIALOG_UI_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_ui_controller.h"
#include "ui/base/ui_base_types.h"
#include "ui/web_dialogs/web_dialogs_export.h"
#include "url/gurl.h"

namespace content {
class WebContents;
class WebUIMessageHandler;
struct ContextMenuParams;
}

namespace gfx {
class Size;
}

namespace ui {

class WebDialogDelegate;

class WEB_DIALOGS_EXPORT WebDialogUI : public content::WebUIController {
 public:
  struct WebDialogParams {
    
    GURL url;
    
    int width;
    
    int height;
    
    std::string json_input;
  };

  
  
  explicit WebDialogUI(content::WebUI* web_ui);
  virtual ~WebDialogUI();

  
  void CloseDialog(const base::ListValue* args);

  
  static void SetDelegate(content::WebContents* web_contents,
                          WebDialogDelegate* delegate);

 private:
  
  virtual void RenderViewCreated(
      content::RenderViewHost* render_view_host) OVERRIDE;

  
  static WebDialogDelegate* GetDelegate(content::WebContents* web_contents);

  
  void OnDialogClosed(const base::ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(WebDialogUI);
};

class WEB_DIALOGS_EXPORT ExternalWebDialogUI : public WebDialogUI {
 public:
  explicit ExternalWebDialogUI(content::WebUI* web_ui);
  virtual ~ExternalWebDialogUI();
};

}  

#endif  
