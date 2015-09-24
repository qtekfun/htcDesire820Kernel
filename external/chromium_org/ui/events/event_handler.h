// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_EVENT_HANDLER_H_
#define UI_EVENTS_EVENT_HANDLER_H_

#include <stack>
#include <vector>

#include "base/basictypes.h"
#include "ui/events/event_constants.h"
#include "ui/events/events_export.h"

namespace ui {

class CancelModeEvent;
class Event;
class EventDispatcher;
class EventTarget;
class GestureEvent;
class KeyEvent;
class MouseEvent;
class ScrollEvent;
class TouchEvent;

class EVENTS_EXPORT EventHandler {
 public:
  EventHandler();
  virtual ~EventHandler();

  
  
  
  
  virtual void OnEvent(Event* event);

  virtual void OnKeyEvent(KeyEvent* event);

  virtual void OnMouseEvent(MouseEvent* event);

  virtual void OnScrollEvent(ScrollEvent* event);

  virtual void OnTouchEvent(TouchEvent* event);

  virtual void OnGestureEvent(GestureEvent* event);

  virtual void OnCancelMode(CancelModeEvent* event);

 private:
  friend class EventDispatcher;

  
  
  std::stack<EventDispatcher*> dispatchers_;

  DISALLOW_COPY_AND_ASSIGN(EventHandler);
};

typedef std::vector<EventHandler*> EventHandlerList;

}  

#endif  
