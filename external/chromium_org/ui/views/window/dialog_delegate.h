// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WINDOW_DIALOG_DELEGATE_H_
#define UI_VIEWS_WINDOW_DIALOG_DELEGATE_H_

#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "ui/base/accessibility/accessibility_types.h"
#include "ui/base/models/dialog_model.h"
#include "ui/base/ui_base_types.h"
#include "ui/views/widget/widget_delegate.h"

namespace views {

class DialogClientView;

class VIEWS_EXPORT DialogDelegate : public ui::DialogModel,
                                    public WidgetDelegate {
 public:
  virtual ~DialogDelegate();

  
  static Widget* CreateDialogWidget(DialogDelegate* dialog,
                                    gfx::NativeWindow context,
                                    gfx::NativeWindow parent);

  
  
  virtual View* CreateExtraView();

  
  
  
  virtual View* CreateTitlebarExtraView();

  
  
  virtual View* CreateFootnoteView();

  
  
  
  
  
  virtual bool Cancel();

  
  
  
  
  
  
  
  virtual bool Accept(bool window_closing);
  virtual bool Accept();

  
  
  
  
  
  virtual bool Close();

  
  virtual base::string16 GetDialogLabel() const OVERRIDE;
  virtual base::string16 GetDialogTitle() const OVERRIDE;
  virtual int GetDialogButtons() const OVERRIDE;
  virtual int GetDefaultDialogButton() const OVERRIDE;
  virtual bool ShouldDefaultButtonBeBlue() const OVERRIDE;
  virtual base::string16 GetDialogButtonLabel(
      ui::DialogButton button) const OVERRIDE;
  virtual bool IsDialogButtonEnabled(ui::DialogButton button) const OVERRIDE;

  
  virtual View* GetInitiallyFocusedView() OVERRIDE;
  virtual DialogDelegate* AsDialogDelegate() OVERRIDE;
  virtual ClientView* CreateClientView(Widget* widget) OVERRIDE;
  virtual NonClientFrameView* CreateNonClientFrameView(Widget* widget) OVERRIDE;

  
  static NonClientFrameView* CreateDialogFrameView(Widget* widget);
  
  
  
  
  
  
  static NonClientFrameView* CreateDialogFrameView(Widget* widget,
                                                   bool force_opaque_border);

  
  virtual bool UseNewStyleForThisDialog() const;

  
  virtual void OnClosed() {}

  
  
  const DialogClientView* GetDialogClientView() const;
  DialogClientView* GetDialogClientView();

 protected:
  
  virtual ui::AccessibilityTypes::Role GetAccessibleWindowRole() const OVERRIDE;
};

class VIEWS_EXPORT DialogDelegateView : public DialogDelegate,
                                        public View {
 public:
  DialogDelegateView();
  virtual ~DialogDelegateView();

  
  virtual void DeleteDelegate() OVERRIDE;
  virtual Widget* GetWidget() OVERRIDE;
  virtual const Widget* GetWidget() const OVERRIDE;
  virtual View* GetContentsView() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(DialogDelegateView);
};

}  

#endif  
