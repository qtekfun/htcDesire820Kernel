// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_GESTURES_TRAY_GESTURE_HANDLER_H_
#define ASH_WM_GESTURES_TRAY_GESTURE_HANDLER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/widget/widget_observer.h"

namespace ui {
class GestureEvent;
}

namespace ash {
namespace internal {

class TrayGestureHandler : public views::WidgetObserver {
 public:
  TrayGestureHandler();
  virtual ~TrayGestureHandler();

  
  
  
  
  bool UpdateGestureDrag(const ui::GestureEvent& event);

  void CompleteGestureDrag(const ui::GestureEvent& event);

 private:
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;

  
  views::Widget* widget_;

  
  float gesture_drag_amount_;

  DISALLOW_COPY_AND_ASSIGN(TrayGestureHandler);
};

}  
}  

#endif  
