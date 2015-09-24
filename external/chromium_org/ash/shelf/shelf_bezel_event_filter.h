// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_SHELF_BEZEL_EVENT_FILTER_H_
#define ASH_SHELF_SHELF_BEZEL_EVENT_FILTER_H_

#include "ash/wm/gestures/shelf_gesture_handler.h"
#include "ui/events/event_handler.h"
#include "ui/gfx/rect.h"

namespace ash {
namespace internal {
class ShelfLayoutManager;

class ShelfBezelEventFilter : public ui::EventHandler {
 public:
  explicit ShelfBezelEventFilter(ShelfLayoutManager* shelf);
  virtual ~ShelfBezelEventFilter();

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

 private:
  bool IsShelfOnBezel(const gfx::Rect& screen,
                      const gfx::Point& point) const;

  ShelfLayoutManager* shelf_;  
  bool in_touch_drag_;
  ShelfGestureHandler gesture_handler_;
  DISALLOW_COPY_AND_ASSIGN(ShelfBezelEventFilter);
};

}  
}  

#endif  
