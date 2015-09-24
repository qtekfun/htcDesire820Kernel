// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_TOOLTIP_CONTROLLER_H_
#define UI_VIEWS_COREWM_TOOLTIP_CONTROLLER_H_

#include <map>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "base/timer/timer.h"
#include "ui/aura/client/tooltip_client.h"
#include "ui/aura/window_observer.h"
#include "ui/events/event_handler.h"
#include "ui/gfx/point.h"
#include "ui/views/views_export.h"

namespace aura {
class Window;
}

namespace views {
namespace corewm {

class Tooltip;

namespace test {
class TooltipControllerTestHelper;
}  

class VIEWS_EXPORT TooltipController : public aura::client::TooltipClient,
                                       public ui::EventHandler,
                                       public aura::WindowObserver {
 public:
  explicit TooltipController(scoped_ptr<Tooltip> tooltip);
  virtual ~TooltipController();

  
  virtual void UpdateTooltip(aura::Window* target) OVERRIDE;
  virtual void SetTooltipShownTimeout(aura::Window* target,
                                      int timeout_in_ms) OVERRIDE;
  virtual void SetTooltipsEnabled(bool enable) OVERRIDE;

  
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void OnCancelMode(ui::CancelModeEvent* event) OVERRIDE;

  
  virtual void OnWindowDestroyed(aura::Window* window) OVERRIDE;

  const gfx::Point& mouse_location() const { return curr_mouse_loc_; }

 private:
  friend class test::TooltipControllerTestHelper;

  void TooltipTimerFired();
  void TooltipShownTimerFired();

  
  
  void UpdateIfRequired();

  
  bool IsTooltipVisible();

  bool IsDragDropInProgress();

  
  bool IsCursorVisible();

  int GetTooltipShownTimeout();

  aura::Window* tooltip_window_;
  base::string16 tooltip_text_;

  
  aura::Window* tooltip_window_at_mouse_press_;
  base::string16 tooltip_text_at_mouse_press_;

  scoped_ptr<Tooltip> tooltip_;

  base::RepeatingTimer<TooltipController> tooltip_timer_;

  
  
  base::OneShotTimer<TooltipController> tooltip_shown_timer_;

  
  gfx::Point curr_mouse_loc_;

  bool tooltips_enabled_;

  std::map<aura::Window*, int> tooltip_shown_timeout_map_;

  DISALLOW_COPY_AND_ASSIGN(TooltipController);
};

}  
}  

#endif  
