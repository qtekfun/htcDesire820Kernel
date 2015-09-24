// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_COMPOUND_EVENT_FILTER_H_
#define UI_VIEWS_COREWM_COMPOUND_EVENT_FILTER_H_

#include "base/compiler_specific.h"
#include "base/observer_list.h"
#include "ui/events/event.h"
#include "ui/events/event_handler.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/views_export.h"

namespace aura {
class CursorManager;
class RootWindow;
}

namespace ui {
class GestureEvent;
class KeyEvent;
class LocatedEvent;
class MouseEvent;
class TouchEvent;
}

namespace views {
namespace corewm {

class VIEWS_EXPORT CompoundEventFilter : public ui::EventHandler {
 public:
  CompoundEventFilter();
  virtual ~CompoundEventFilter();

  
  static gfx::NativeCursor CursorForWindowComponent(int window_component);

  
  
  
  
  void AddHandler(ui::EventHandler* filter);
  void RemoveHandler(ui::EventHandler* filter);

 private:
  
  
  void UpdateCursor(aura::Window* target, ui::MouseEvent* event);

  
  void FilterKeyEvent(ui::KeyEvent* event);
  void FilterMouseEvent(ui::MouseEvent* event);
  void FilterTouchEvent(ui::TouchEvent* event);

  
  void SetCursorVisibilityOnEvent(aura::Window* target,
                                  ui::Event* event,
                                  bool show);

  
  void SetMouseEventsEnableStateOnEvent(aura::Window* target,
                                        ui::Event* event,
                                        bool enable);

  
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  ObserverList<ui::EventHandler, true> handlers_;

  
  bool cursor_hidden_by_filter_;

  DISALLOW_COPY_AND_ASSIGN(CompoundEventFilter);
};

}  
}  

#endif  
