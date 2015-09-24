// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_HTML_DIALOG_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_HTML_DIALOG_VIEW_H_
#pragma once

#include <string>

#include "chrome/browser/ui/views/dom_view.h"
#include "chrome/browser/ui/webui/html_dialog_tab_contents_delegate.h"
#include "chrome/browser/ui/webui/html_dialog_ui.h"
#include "ui/gfx/size.h"
#include "views/window/window_delegate.h"

class Browser;
namespace views {
class Window;
}

class HtmlDialogView
    : public DOMView,
      public HtmlDialogTabContentsDelegate,
      public HtmlDialogUIDelegate,
      public views::WindowDelegate {
 public:
  HtmlDialogView(Profile* profile, HtmlDialogUIDelegate* delegate);
  virtual ~HtmlDialogView();

  
  void InitDialog();

  
  virtual gfx::Size GetPreferredSize();
  virtual bool AcceleratorPressed(const views::Accelerator& accelerator);

  
  virtual bool CanResize() const;
  virtual bool IsModal() const;
  virtual std::wstring GetWindowTitle() const;
  virtual void WindowClosing();
  virtual views::View* GetContentsView();
  virtual views::View* GetInitiallyFocusedView();
  virtual bool ShouldShowWindowTitle() const;

  
  virtual bool IsDialogModal() const;
  virtual std::wstring GetDialogTitle() const;
  virtual GURL GetDialogContentURL() const;
  virtual void GetWebUIMessageHandlers(
      std::vector<WebUIMessageHandler*>* handlers) const;
  virtual void GetDialogSize(gfx::Size* size) const;
  virtual std::string GetDialogArgs() const;
  virtual void OnWindowClosed();
  virtual void OnDialogClosed(const std::string& json_retval);
  virtual void OnCloseContents(TabContents* source, bool* out_close_dialog);
  virtual bool ShouldShowDialogTitle() const;
  virtual bool HandleContextMenu(const ContextMenuParams& params);

  
  virtual void MoveContents(TabContents* source, const gfx::Rect& pos);
  virtual void HandleKeyboardEvent(const NativeWebKeyboardEvent& event);
  virtual void CloseContents(TabContents* source);

 private:
  
  
  
  
  HtmlDialogUIDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(HtmlDialogView);
};

#endif  
