// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_WIDGET_DELEGATE_H_
#define UI_VIEWS_WIDGET_WIDGET_DELEGATE_H_

#include <string>
#include <vector>

#include "ui/base/accessibility/accessibility_types.h"
#include "ui/base/ui_base_types.h"
#include "ui/views/view.h"

namespace gfx {
class ImageSkia;
class Rect;
}

namespace views {
class BubbleDelegateView;
class ClientView;
class DialogDelegate;
class NonClientFrameView;
class View;
class Widget;

class VIEWS_EXPORT WidgetDelegate {
 public:
  WidgetDelegate();

  
  virtual void OnWidgetMove();

  
  virtual void OnDisplayChanged();

  
  
  virtual void OnWorkAreaChanged();

  
  
  virtual View* GetInitiallyFocusedView();

  virtual BubbleDelegateView* AsBubbleDelegate();
  virtual DialogDelegate* AsDialogDelegate();

  
  virtual bool CanResize() const;

  
  virtual bool CanMaximize() const;

  
  virtual bool CanActivate() const;

  
  
  virtual ui::ModalType GetModalType() const;

  virtual ui::AccessibilityTypes::Role GetAccessibleWindowRole() const;

  
  virtual string16 GetAccessibleWindowTitle() const;

  
  virtual string16 GetWindowTitle() const;

  
  virtual bool ShouldShowWindowTitle() const;

  
  virtual bool ShouldShowCloseButton() const;

  
  
  virtual bool ShouldHandleSystemCommands() const;

  
  
  virtual gfx::ImageSkia GetWindowAppIcon();

  
  virtual gfx::ImageSkia GetWindowIcon();

  
  virtual bool ShouldShowWindowIcon() const;

  
  
  virtual bool ExecuteWindowsCommand(int command_id);

  
  
  virtual std::string GetWindowName() const;

  
  
  
  virtual void SaveWindowPlacement(const gfx::Rect& bounds,
                                   ui::WindowShowState show_state);

  
  
  virtual bool GetSavedWindowPlacement(const Widget* widget,
                                       gfx::Rect* bounds,
                                       ui::WindowShowState* show_state) const;

  
  
  
  
  virtual bool ShouldRestoreWindowSize() const;

  
  
  virtual void WindowClosing() {}

  
  
  
  virtual void DeleteDelegate() {}

  
  virtual void OnWindowBeginUserBoundsChange() {}
  virtual void OnWindowEndUserBoundsChange() {}

  
  virtual Widget* GetWidget() = 0;
  virtual const Widget* GetWidget() const = 0;

  
  virtual View* GetContentsView();

  
  
  virtual ClientView* CreateClientView(Widget* widget);

  
  
  virtual NonClientFrameView* CreateNonClientFrameView(Widget* widget);

  
  
  
  
  virtual View* CreateOverlayView();

  
  
  
  
  virtual bool WillProcessWorkAreaChange() const;

  
  virtual bool WidgetHasHitTestMask() const;

  
  virtual void GetWidgetHitTestMask(gfx::Path* mask) const;

  
  
  
  virtual bool ShouldAdvanceFocusToTopLevelWidget() const;

  
  
  virtual bool ShouldDescendIntoChildForEventHandling(
      gfx::NativeView child,
      const gfx::Point& location);

  
  
  virtual void GetAccessiblePanes(std::vector<View*>* panes) {}

 protected:
  virtual ~WidgetDelegate() {}

 private:
  View* default_contents_view_;

  DISALLOW_COPY_AND_ASSIGN(WidgetDelegate);
};

class VIEWS_EXPORT WidgetDelegateView : public WidgetDelegate, public View {
 public:
  WidgetDelegateView();
  virtual ~WidgetDelegateView();

  
  virtual void DeleteDelegate() OVERRIDE;
  virtual Widget* GetWidget() OVERRIDE;
  virtual const Widget* GetWidget() const OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(WidgetDelegateView);
};

}  

#endif  
