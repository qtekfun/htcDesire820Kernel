// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WINDOW_DIALOG_CLIENT_VIEW_H_
#define UI_VIEWS_WINDOW_DIALOG_CLIENT_VIEW_H_

#include "ui/base/ui_base_types.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/focus/focus_manager.h"
#include "ui/views/window/client_view.h"

namespace views {

class DialogDelegate;
class LabelButton;
class Widget;

class VIEWS_EXPORT DialogClientView : public ClientView,
                                      public ButtonListener,
                                      public FocusChangeListener {
 public:
  DialogClientView(Widget* widget, View* contents_view);
  virtual ~DialogClientView();

  
  void AcceptWindow();
  void CancelWindow();

  
  LabelButton* ok_button() const { return ok_button_; }
  LabelButton* cancel_button() const { return cancel_button_; }

  
  void UpdateDialogButtons();

  
  virtual bool CanClose() OVERRIDE;
  virtual DialogClientView* AsDialogClientView() OVERRIDE;
  virtual const DialogClientView* AsDialogClientView() const OVERRIDE;

  
  virtual void OnWillChangeFocus(View* focused_before,
                                 View* focused_now) OVERRIDE;
  virtual void OnDidChangeFocus(View* focused_before,
                                View* focused_now) OVERRIDE;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  virtual void NativeViewHierarchyChanged() OVERRIDE;

  
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE;

 protected:
  
  DialogClientView(View* contents_view);

  
  virtual DialogDelegate* GetDialogDelegate() const;

  
  void CreateExtraView();

  
  void CreateFootnoteView();

  
  virtual void ChildPreferredSizeChanged(View* child) OVERRIDE;
  virtual void ChildVisibilityChanged(View* child) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(DialogClientViewTest, FocusManager);

  bool has_dialog_buttons() const { return ok_button_ || cancel_button_; }

  
  LabelButton* CreateDialogButton(ui::DialogButton type);

  
  void UpdateButton(LabelButton* button, ui::DialogButton type);

  
  int GetButtonsAndExtraViewRowHeight() const;

  
  gfx::Insets GetButtonRowInsets() const;

  
  void Close();

  
  LabelButton* ok_button_;
  LabelButton* cancel_button_;

  
  LabelButton* default_button_;

  
  FocusManager* focus_manager_;

  
  View* extra_view_;

  
  View* footnote_view_;

  
  
  
  
  bool notified_delegate_;

  DISALLOW_COPY_AND_ASSIGN(DialogClientView);
};

}  

#endif  
