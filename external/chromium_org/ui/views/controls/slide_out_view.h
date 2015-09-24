// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SLIDE_OUT_VIEW_H_
#define UI_VIEWS_CONTROLS_SLIDE_OUT_VIEW_H_

#include "ui/compositor/layer_animation_observer.h"
#include "ui/views/view.h"
#include "ui/views/views_export.h"

namespace views {

class VIEWS_EXPORT SlideOutView : public views::View,
                                  public ui::ImplicitAnimationObserver {
 public:
  SlideOutView();
  virtual ~SlideOutView();

 protected:
  
  virtual void OnSlideOut() = 0;

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

 private:
  enum SlideDirection {
    SLIDE_LEFT,
    SLIDE_RIGHT,
  };

  
  void RestoreVisualState();

  
  void SlideOutAndClose(SlideDirection direction);

  
  virtual void OnImplicitAnimationsCompleted() OVERRIDE;

  float gesture_scroll_amount_;

  DISALLOW_COPY_AND_ASSIGN(SlideOutView);
};

}  

#endif  
