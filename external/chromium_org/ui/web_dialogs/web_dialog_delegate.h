// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_WEB_DIALOGS_WEB_DIALOG_DELEGATE_H_
#define UI_WEB_DIALOGS_WEB_DIALOG_DELEGATE_H_

#include <string>
#include <vector>

#include "base/strings/string16.h"
#include "ui/base/ui_base_types.h"
#include "ui/base/window_open_disposition.h"
#include "ui/web_dialogs/web_dialogs_export.h"

class GURL;

namespace content {
class RenderViewHost;
class WebContents;
class WebUI;
class WebUIMessageHandler;
struct ContextMenuParams;
struct OpenURLParams;
}

namespace gfx {
class Rect;
class Size;
}

namespace ui {

class WEB_DIALOGS_EXPORT WebDialogDelegate {
 public:
  
  virtual ModalType GetDialogModalType() const = 0;

  
  virtual base::string16 GetDialogTitle() const = 0;

  
  
  virtual std::string GetDialogName() const;

  
  virtual GURL GetDialogContentURL() const = 0;

  
  
  
  
  virtual void GetWebUIMessageHandlers(
      std::vector<content::WebUIMessageHandler*>* handlers) const = 0;

  
  virtual void GetDialogSize(gfx::Size* size) const = 0;

  
  virtual void GetMinimumDialogSize(gfx::Size* size) const;

  
  virtual std::string GetDialogArgs() const = 0;

  
  
  
  
  virtual bool CanCloseDialog() const;

  
  
  virtual void OnLoadingStateChanged(content::WebContents* source) {}

  
  
  
  virtual void OnDialogShown(content::WebUI* webui,
                             content::RenderViewHost* render_view_host) {}

  
  
  
  
  virtual void OnDialogClosed(const std::string& json_retval) = 0;

  
  
  virtual void OnDialogCloseFromWebUI(const std::string& json_retval);

  
  
  
  
  virtual void OnCloseContents(content::WebContents* source,
                               bool* out_close_dialog) = 0;

  
  
  virtual bool ShouldShowDialogTitle() const = 0;

  
  
  
  
  virtual bool HandleContextMenu(const content::ContextMenuParams& params);

  
  
  
  virtual bool HandleOpenURLFromTab(content::WebContents* source,
                                    const content::OpenURLParams& params,
                                    content::WebContents** out_new_contents);

  
  
  
  
  
  virtual bool HandleAddNewContents(content::WebContents* source,
                                    content::WebContents* new_contents,
                                    WindowOpenDisposition disposition,
                                    const gfx::Rect& initial_pos,
                                    bool user_gesture);

  
  virtual void StoreDialogSize(const gfx::Size& dialog_size) {}

  virtual ~WebDialogDelegate() {}
};

}  

#endif  
