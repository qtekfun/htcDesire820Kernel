// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_CAPTION_BUTTONS_FRAME_MAXIMIZE_BUTTON_H_
#define ASH_WM_CAPTION_BUTTONS_FRAME_MAXIMIZE_BUTTON_H_

#include "ash/ash_export.h"
#include "ash/wm/caption_buttons/caption_button_types.h"
#include "ash/wm/caption_buttons/frame_caption_button.h"
#include "ash/wm/workspace/snap_types.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/timer/timer.h"
#include "ui/aura/window_observer.h"
#include "ui/views/widget/widget_observer.h"

namespace views {
class Widget;
}

namespace ash {
class FrameMaximizeButtonObserver;

namespace internal {
class PhantomWindowController;
class SnapSizer;
}

class MaximizeBubbleController;

class ASH_EXPORT FrameMaximizeButton : public FrameCaptionButton,
                                       public views::WidgetObserver,
                                       public aura::WindowObserver {
 public:
  FrameMaximizeButton(views::ButtonListener* listener,
                      views::Widget* frame);
  virtual ~FrameMaximizeButton();

  void AddObserver(FrameMaximizeButtonObserver* observer);
  void RemoveObserver(FrameMaximizeButtonObserver* observer);

  
  
  void SnapButtonHovered(SnapType type);

  
  
  void ExecuteSnapAndCloseMenu(SnapType type);

  
  void OnMaximizeBubbleShown(views::Widget* bubble);

  
  void DestroyMaximizeMenu();

  
  bool is_snap_enabled() const { return is_snap_enabled_; }

  
  virtual void OnWindowBoundsChanged(aura::Window* window,
                                     const gfx::Rect& old_bounds,
                                     const gfx::Rect& new_bounds) OVERRIDE;
  virtual void OnWindowPropertyChanged(aura::Window* window,
                                       const void* key,
                                       intptr_t old) OVERRIDE;
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;

  
  virtual void OnWidgetActivationChanged(views::Widget* widget,
                                        bool active) OVERRIDE;

  
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  virtual void SetVisible(bool visible) OVERRIDE;

  
  void set_bubble_appearance_delay_ms(int bubble_appearance_delay_ms) {
    bubble_appearance_delay_ms_ = bubble_appearance_delay_ms;
  }

  
  MaximizeBubbleController* maximizer() { return maximizer_.get(); }

  
  bool phantom_window_open() { return phantom_window_.get() != NULL; }

 private:
  class EscapeEventFilter;

  
  
  void ProcessStartEvent(const ui::LocatedEvent& event);

  
  
  void ProcessUpdateEvent(const ui::LocatedEvent& event);

  
  bool ProcessEndEvent(const ui::LocatedEvent& event);

  
  
  void Cancel(bool keep_menu_open);

  
  void InstallEventFilter();
  void UninstallEventFilter();

  
  
  void UpdateSnapFromEventLocation();

  
  
  
  void UpdateSnap(const gfx::Point& location,
                  bool select_default,
                  bool is_touch);

  
  SnapType SnapTypeForLocation(const gfx::Point& location) const;

  
  gfx::Rect ScreenBoundsForType(SnapType type,
                                const internal::SnapSizer& snap_sizer) const;

  
  
  gfx::Point LocationForSnapSizer(const gfx::Point& location) const;

  
  void Snap(internal::SnapSizer* snap_sizer);

  
  MaximizeBubbleFrameState GetMaximizeBubbleFrameState() const;

  
  
  views::Widget* frame_;

  
  bool observing_frame_;

  
  scoped_ptr<internal::PhantomWindowController> phantom_window_;

  
  
  bool is_snap_enabled_;

  
  bool exceeded_drag_threshold_;

  
  gfx::Point press_location_;

  
  bool press_is_gesture_;

  
  SnapType snap_type_;

  scoped_ptr<internal::SnapSizer> snap_sizer_;

  scoped_ptr<EscapeEventFilter> escape_event_filter_;

  base::OneShotTimer<FrameMaximizeButton> update_timer_;

  scoped_ptr<MaximizeBubbleController> maximizer_;

  
  int bubble_appearance_delay_ms_;

  ObserverList<FrameMaximizeButtonObserver> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(FrameMaximizeButton);
};

}  

#endif  
