// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_WINDOW_MODALITY_CONTROLLER_H_
#define UI_VIEWS_COREWM_WINDOW_MODALITY_CONTROLLER_H_

#include <vector>

#include "base/compiler_specific.h"
#include "ui/aura/env_observer.h"
#include "ui/aura/window_observer.h"
#include "ui/events/event_handler.h"
#include "ui/views/views_export.h"

namespace ui {
class EventTarget;
class LocatedEvent;
}

namespace views {
namespace corewm {

VIEWS_EXPORT void SetModalParent(aura::Window* child, aura::Window* parent);

VIEWS_EXPORT aura::Window* GetModalTransient(aura::Window* window);

class VIEWS_EXPORT WindowModalityController : public ui::EventHandler,
                                              public aura::EnvObserver,
                                              public aura::WindowObserver {
 public:
  explicit WindowModalityController(ui::EventTarget* event_target);
  virtual ~WindowModalityController();

  
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;

  
  virtual void OnWindowInitialized(aura::Window* window) OVERRIDE;

  
  virtual void OnWindowPropertyChanged(aura::Window* window,
                                       const void* key,
                                       intptr_t old) OVERRIDE;
  virtual void OnWindowVisibilityChanged(aura::Window* window,
                                         bool visible) OVERRIDE;
  virtual void OnWindowDestroyed(aura::Window* window) OVERRIDE;

 private:
  
  
  bool ProcessLocatedEvent(aura::Window* target,
                           ui::LocatedEvent* event);

  std::vector<aura::Window*> windows_;

  ui::EventTarget* event_target_;

  DISALLOW_COPY_AND_ASSIGN(WindowModalityController);
};

}  
}  

#endif  
