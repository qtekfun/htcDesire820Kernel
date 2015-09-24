// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_GESTURES_GESTURE_RECOGNIZER_H_
#define UI_EVENTS_GESTURES_GESTURE_RECOGNIZER_H_

#include <vector>

#include "base/memory/scoped_vector.h"
#include "ui/events/event_constants.h"
#include "ui/events/events_export.h"
#include "ui/events/gestures/gesture_types.h"

namespace ui {
class EVENTS_EXPORT GestureRecognizer {
 public:
  static GestureRecognizer* Create();
  static GestureRecognizer* Get();

  
  typedef ScopedVector<GestureEvent> Gestures;

  virtual ~GestureRecognizer() {}

  
  
  
  
  virtual Gestures* ProcessTouchEventForGesture(const TouchEvent& event,
                                                ui::EventResult result,
                                                GestureConsumer* consumer) = 0;

  
  
  virtual void CleanupStateForConsumer(GestureConsumer* consumer) = 0;

  
  
  
  virtual GestureConsumer* GetTouchLockedTarget(const TouchEvent& event) = 0;

  
  virtual GestureConsumer* GetTargetForGestureEvent(
      const GestureEvent& event) = 0;

  
  
  
  virtual GestureConsumer* GetTargetForLocation(const gfx::Point& location) = 0;

  
  
  
  
  
  
  
  virtual void TransferEventsTo(GestureConsumer* current_consumer,
                                GestureConsumer* new_consumer) = 0;

  
  
  
  virtual bool GetLastTouchPointForTarget(GestureConsumer* consumer,
                                          gfx::Point* point) = 0;

  
  virtual void CancelActiveTouches(GestureConsumer* consumer) = 0;

  
  
  
  
  virtual void AddGestureEventHelper(GestureEventHelper* helper) = 0;

  
  
  
  virtual void RemoveGestureEventHelper(GestureEventHelper* helper) = 0;
};

}  

#endif  
