// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TAB_MODAL_CONFIRM_DIALOG_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_TAB_MODAL_CONFIRM_DIALOG_VIEWS_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/ui/tab_modal_confirm_dialog.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/controls/link_listener.h"
#include "ui/views/window/dialog_delegate.h"

namespace content {
class WebContents;
class BrowserContext;
}

namespace views {
class MessageBoxView;
class Widget;
}

class TabModalConfirmDialogViews : public TabModalConfirmDialog,
                                   public views::DialogDelegate,
                                   public views::LinkListener {
 public:
  TabModalConfirmDialogViews(TabModalConfirmDialogDelegate* delegate,
                             content::WebContents* web_contents);

  
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual base::string16 GetDialogButtonLabel(
      ui::DialogButton button) const OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual bool Close() OVERRIDE;

  
  virtual views::View* GetContentsView() OVERRIDE;
  virtual views::NonClientFrameView* CreateNonClientFrameView(
      views::Widget* widget) OVERRIDE;
  virtual views::Widget* GetWidget() OVERRIDE;
  virtual const views::Widget* GetWidget() const OVERRIDE;
  virtual void DeleteDelegate() OVERRIDE;
  virtual ui::ModalType GetModalType() const OVERRIDE;

 private:
  virtual ~TabModalConfirmDialogViews();

  
  virtual void AcceptTabModalDialog() OVERRIDE;
  virtual void CancelTabModalDialog() OVERRIDE;

  
  virtual void CloseDialog() OVERRIDE;

  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

  scoped_ptr<TabModalConfirmDialogDelegate> delegate_;

  
  views::MessageBoxView* message_box_view_;

  views::Widget* dialog_;
  content::BrowserContext* browser_context_;

  DISALLOW_COPY_AND_ASSIGN(TabModalConfirmDialogViews);
};

#endif  
