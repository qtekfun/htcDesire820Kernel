// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_HTML_DIALOG_UI_H_
#define CHROME_BROWSER_UI_WEBUI_HTML_DIALOG_UI_H_
#pragma once

#include <string>
#include <vector>

#include "content/browser/webui/web_ui.h"
#include "content/common/property_bag.h"
#include "googleurl/src/gurl.h"

namespace gfx {
class Size;
}

struct ContextMenuParams;

class HtmlDialogUIDelegate {
 public:
  
  virtual bool IsDialogModal() const = 0;

  
  virtual std::wstring GetDialogTitle() const = 0;

  
  virtual GURL GetDialogContentURL() const = 0;

  
  
  
  
  virtual void GetWebUIMessageHandlers(
      std::vector<WebUIMessageHandler*>* handlers) const = 0;

  
  virtual void GetDialogSize(gfx::Size* size) const = 0;

  
  virtual std::string GetDialogArgs() const = 0;

  
  virtual void OnDialogClosed(const std::string& json_retval) = 0;

  
  
  
  virtual void OnWindowClosed();

  
  
  
  
  virtual void OnCloseContents(TabContents* source, bool* out_close_dialog) = 0;

  
  
  virtual bool ShouldShowDialogTitle() const = 0;

  
  
  virtual bool HandleContextMenu(const ContextMenuParams& params);

 protected:
  virtual ~HtmlDialogUIDelegate() {}
};

class HtmlDialogUI : public WebUI {
 public:
  struct HtmlDialogParams {
    
    GURL url;
    
    int width;
    
    int height;
    
    std::string json_input;
  };

  
  explicit HtmlDialogUI(TabContents* tab_contents);
  virtual ~HtmlDialogUI();

  
  
  static PropertyAccessor<HtmlDialogUIDelegate*>& GetPropertyAccessor();

 private:
  
  virtual void RenderViewCreated(RenderViewHost* render_view_host);

  
  void OnDialogClosed(const ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(HtmlDialogUI);
};

class ExternalHtmlDialogUI : public HtmlDialogUI {
 public:
  explicit ExternalHtmlDialogUI(TabContents* tab_contents);
  virtual ~ExternalHtmlDialogUI();
};

#endif  
