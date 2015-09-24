// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DESKTOP_BACKGROUND_DESKTOP_BACKGROUND_WIDGET_CONTROLLER_H_
#define ASH_DESKTOP_BACKGROUND_DESKTOP_BACKGROUND_WIDGET_CONTROLLER_H_

#include "ash/ash_export.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/window.h"
#include "ui/compositor/layer.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_observer.h"

namespace ash {
namespace internal {
class RootWindowController;

class ASH_EXPORT DesktopBackgroundWidgetController
    : public views::WidgetObserver {
 public:
  
  explicit DesktopBackgroundWidgetController(views::Widget* widget);

  virtual ~DesktopBackgroundWidgetController();

  
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;

  
  void SetBounds(gfx::Rect bounds);

  
  
  
  bool Reparent(aura::Window* root_window,
                int src_container,
                int dest_container);

  
  
  
  void StartAnimating(RootWindowController* root_window_controller);

  views::Widget* widget() { return widget_; }

 private:
  views::Widget* widget_;

  DISALLOW_COPY_AND_ASSIGN(DesktopBackgroundWidgetController);
};

class ASH_EXPORT AnimatingDesktopController {
 public:
  explicit AnimatingDesktopController(
      DesktopBackgroundWidgetController* component);
  ~AnimatingDesktopController();

  
  
  
  
  
  
  
  
  
  
  
  
  
  void StopAnimating();

  
  
  DesktopBackgroundWidgetController* GetController(bool pass_ownership);

 private:
  scoped_ptr<DesktopBackgroundWidgetController> controller_;

  DISALLOW_COPY_AND_ASSIGN(AnimatingDesktopController);
};

}  
}  

#endif  
