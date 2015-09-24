// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_TEST_CHILD_WMODAL_WINDOW_H_
#define UI_VIEWS_TEST_CHILD_WMODAL_WINDOW_H_

#include "ui/views/controls/button/button.h"
#include "ui/views/widget/widget_delegate.h"
#include "ui/views/widget/widget_observer.h"

namespace views {
class LabelButton;
class NativeViewHost;
class Textfield;
class View;
class Widget;
namespace test {

void CreateChildModalParent(gfx::NativeView context);

class ChildModalParent : public WidgetDelegateView,
                         public ButtonListener,
                         public WidgetObserver {
 public:
  ChildModalParent(gfx::NativeView context);
  virtual ~ChildModalParent();

  void ShowChild();
  gfx::NativeWindow GetModalParent() const;
  gfx::NativeWindow GetChild() const;

 private:
  Widget* CreateChild();

  
  virtual View* GetContentsView() OVERRIDE;
  virtual string16 GetWindowTitle() const OVERRIDE;
  virtual bool CanResize() const OVERRIDE;
  virtual void DeleteDelegate() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

  
  virtual void ButtonPressed(Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void OnWidgetDestroying(Widget* widget) OVERRIDE;

  
  
  LabelButton* button_;

  
  Textfield* textfield_;

  
  NativeViewHost* host_;

  
  
  gfx::NativeWindow modal_parent_;

  
  Widget* child_;

  DISALLOW_COPY_AND_ASSIGN(ChildModalParent);
};

}  
}  

#endif  
