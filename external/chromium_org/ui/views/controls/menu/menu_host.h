// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_HOST_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_HOST_H_

#include "base/compiler_specific.h"
#include "ui/gfx/rect.h"
#include "ui/views/widget/widget.h"

namespace views {

class NativeWidget;
class SubmenuView;
class View;
class Widget;

class MenuHost : public Widget {
 public:
  explicit MenuHost(SubmenuView* submenu);
  virtual ~MenuHost();

  
  
  void InitMenuHost(Widget* parent,
                    const gfx::Rect& bounds,
                    View* contents_view,
                    bool do_capture);

  
  bool IsMenuHostVisible();

  
  
  void ShowMenuHost(bool do_capture);

  
  void HideMenuHost();

  
  void DestroyMenuHost();

  
  void SetMenuHostBounds(const gfx::Rect& bounds);

  
  void ReleaseMenuHostCapture();

 private:
  
  virtual internal::RootView* CreateRootView() OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnNativeWidgetDestroyed() OVERRIDE;
  virtual void OnOwnerClosing() OVERRIDE;

  
  SubmenuView* submenu_;

  
  bool destroying_;

  
  bool ignore_capture_lost_;

  DISALLOW_COPY_AND_ASSIGN(MenuHost);
};

}  

#endif  
