// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_UI_FOCUS_RING_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_UI_FOCUS_RING_CONTROLLER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/views/focus/focus_manager.h"
#include "ui/views/focus/widget_focus_manager.h"
#include "ui/views/widget/widget_observer.h"

namespace views {
class View;
class Widget;
}

namespace chromeos {

class FocusRingLayer;

class FocusRingController : public views::WidgetObserver,
                            public views::WidgetFocusChangeListener,
                            public views::FocusChangeListener {
 public:
  FocusRingController();
  virtual ~FocusRingController();

  
  void SetVisible(bool visible);

 private:
  
  void SetWidget(views::Widget* widget);

  
  
  void UpdateFocusRing();

  
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;
  virtual void OnWidgetBoundsChanged(views::Widget* widget,
                                     const gfx::Rect& new_bounds) OVERRIDE;

  
  virtual void OnNativeFocusChange(gfx::NativeView focused_before,
                                   gfx::NativeView focused_now) OVERRIDE;

  
  virtual void OnWillChangeFocus(views::View* focused_before,
                                 views::View* focused_now) OVERRIDE;
  virtual void OnDidChangeFocus(views::View* focused_before,
                                views::View* focused_now) OVERRIDE;

  bool visible_;

  views::Widget* widget_;
  scoped_ptr<FocusRingLayer> focus_ring_layer_;

  DISALLOW_COPY_AND_ASSIGN(FocusRingController);
};

}  

#endif  
