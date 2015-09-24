// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_ROOT_WINDOW_H_
#define UI_AURA_ROOT_WINDOW_H_

#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "ui/aura/aura_export.h"
#include "ui/aura/client/capture_delegate.h"
#include "ui/aura/window_tree_host.h"
#include "ui/aura/window_tree_host_delegate.h"
#include "ui/base/cursor/cursor.h"
#include "ui/compositor/compositor.h"
#include "ui/compositor/layer_animation_observer.h"
#include "ui/events/event_constants.h"
#include "ui/events/event_processor.h"
#include "ui/events/event_targeter.h"
#include "ui/events/gestures/gesture_recognizer.h"
#include "ui/events/gestures/gesture_types.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/gfx/transform.h"

namespace gfx {
class Size;
class Transform;
}

namespace ui {
class GestureEvent;
class GestureRecognizer;
class KeyEvent;
class LayerAnimationSequence;
class MouseEvent;
class ScrollEvent;
class TouchEvent;
class ViewProp;
}

namespace aura {
class RootWindow;
class RootWindowHost;
class RootWindowObserver;
class RootWindowTransformer;
class TestScreen;
class WindowTargeter;

class AURA_EXPORT RootWindow : public ui::EventProcessor,
                               public ui::GestureEventHelper,
                               public ui::LayerAnimationObserver,
                               public aura::client::CaptureDelegate,
                               public aura::RootWindowHostDelegate {
 public:
  struct AURA_EXPORT CreateParams {
    
    explicit CreateParams(const gfx::Rect& initial_bounds);
    ~CreateParams() {}

    gfx::Rect initial_bounds;

    
    
    RootWindowHost* host;
  };

  explicit RootWindow(const CreateParams& params);
  virtual ~RootWindow();

  
  
  static RootWindow* GetForAcceleratedWidget(gfx::AcceleratedWidget widget);

  Window* window() {
    return const_cast<Window*>(const_cast<const RootWindow*>(this)->window());
  }
  const Window* window() const { return window_.get(); }
  RootWindowHost* host() {
    return const_cast<RootWindowHost*>(
        const_cast<const RootWindow*>(this)->host());
  }
  const RootWindowHost* host() const { return host_.get(); }
  ui::Compositor* compositor() { return compositor_.get(); }
  gfx::NativeCursor last_cursor() const { return last_cursor_; }
  Window* mouse_pressed_handler() { return mouse_pressed_handler_; }
  Window* mouse_moved_handler() { return mouse_moved_handler_; }

  
  void Init();

  
  void PrepareForShutdown();

  
  
  
  void RepostEvent(const ui::LocatedEvent& event);

  RootWindowHostDelegate* AsRootWindowHostDelegate();

  
  void SetHostSize(const gfx::Size& size_in_pixel);

  
  void SetHostBounds(const gfx::Rect& size_in_pizel);

  
  
  
  
  void SetCursor(gfx::NativeCursor cursor);

  
  void OnCursorVisibilityChanged(bool visible);

  
  void OnMouseEventsEnableStateChanged(bool enabled);

  
  void MoveCursorTo(const gfx::Point& location);

  
  void MoveCursorToHostLocation(const gfx::Point& host_location);

  
  void ScheduleRedrawRect(const gfx::Rect& damage_rect);

  
  Window* GetGestureTarget(ui::GestureEvent* event);

  
  
  
  void DispatchGestureEvent(ui::GestureEvent* event);

  
  void OnWindowDestroying(Window* window);

  
  
  void OnWindowBoundsChanged(Window* window, bool contained_mouse);

  
  void DispatchMouseExitToHidingWindow(Window* window);

  
  void DispatchMouseExitAtPoint(const gfx::Point& point);

  
  void OnWindowVisibilityChanged(Window* window, bool is_visible);

  
  
  
  void OnWindowTransformed(Window* window, bool contained_mouse);

  
  
  void OnKeyboardMappingChanged();

  
  void OnRootWindowHostCloseRequested();

  
  
  
  
  void AddRootWindowObserver(RootWindowObserver* observer);
  void RemoveRootWindowObserver(RootWindowObserver* observer);

  
  
  void ConvertPointToHost(gfx::Point* point) const;

  
  
  void ConvertPointFromHost(gfx::Point* point) const;

  

  
  
  
  
  
  void ProcessedTouchEvent(ui::TouchEvent* event,
                           Window* window,
                           ui::EventResult result);

  
  
  
  
  
  
  
  void HoldPointerMoves();
  void ReleasePointerMoves();

  
  
  gfx::Point GetLastMouseLocationInRoot() const;

  void SetRootWindowTransformer(scoped_ptr<RootWindowTransformer> transformer);
  gfx::Transform GetRootTransform() const;

  void SetTransform(const gfx::Transform& transform);

 private:
  FRIEND_TEST_ALL_PREFIXES(RootWindowTest, KeepTranslatedEventInRoot);

  friend class Window;
  friend class TestScreen;

  
  enum WindowHiddenReason {
    WINDOW_DESTROYED,  
    WINDOW_HIDDEN,     
    WINDOW_MOVING,     
                       
  };

  
  
  
  
  
  void TransformEventForDeviceScaleFactor(ui::LocatedEvent* event);

  
  
  void MoveCursorToInternal(const gfx::Point& root_location,
                            const gfx::Point& host_location);

  
  
  ui::EventDispatchDetails DispatchMouseEnterOrExit(
      const ui::MouseEvent& event,
      ui::EventType type) WARN_UNUSED_RESULT;
  ui::EventDispatchDetails ProcessGestures(
      ui::GestureRecognizer::Gestures* gestures) WARN_UNUSED_RESULT;

  
  void OnWindowAddedToRootWindow(Window* window);
  void OnWindowRemovedFromRootWindow(Window* window, Window* new_root);

  
  
  
  void OnWindowHidden(Window* invisible, WindowHiddenReason reason);

  
  
  void CleanupGestureState(Window* window);

  
  
  void UpdateRootWindowSize(const gfx::Size& host_size);

  
  virtual void UpdateCapture(Window* old_capture, Window* new_capture) OVERRIDE;
  virtual void OnOtherRootGotCapture() OVERRIDE;
  virtual void SetNativeCapture() OVERRIDE;
  virtual void ReleaseNativeCapture() OVERRIDE;

  
  virtual ui::EventTarget* GetRootTarget() OVERRIDE;
  virtual void PrepareEventForDispatch(ui::Event* event) OVERRIDE;

  
  virtual bool CanDispatchToTarget(ui::EventTarget* target) OVERRIDE;
  virtual ui::EventDispatchDetails PreDispatchEvent(ui::EventTarget* target,
                                                    ui::Event* event) OVERRIDE;
  virtual ui::EventDispatchDetails PostDispatchEvent(
      ui::EventTarget* target, const ui::Event& event) OVERRIDE;

  
  virtual bool CanDispatchToConsumer(ui::GestureConsumer* consumer) OVERRIDE;
  virtual void DispatchPostponedGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual void DispatchCancelTouchEvent(ui::TouchEvent* event) OVERRIDE;

  
  virtual void OnLayerAnimationEnded(
      ui::LayerAnimationSequence* animation) OVERRIDE;
  virtual void OnLayerAnimationScheduled(
      ui::LayerAnimationSequence* animation) OVERRIDE;
  virtual void OnLayerAnimationAborted(
      ui::LayerAnimationSequence* animation) OVERRIDE;

  
  virtual bool OnHostKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual bool OnHostMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual bool OnHostScrollEvent(ui::ScrollEvent* event) OVERRIDE;
  virtual bool OnHostTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void OnHostCancelMode() OVERRIDE;
  virtual void OnHostActivated() OVERRIDE;
  virtual void OnHostLostWindowCapture() OVERRIDE;
  virtual void OnHostLostMouseGrab() OVERRIDE;
  virtual void OnHostPaint(const gfx::Rect& damage_rect) OVERRIDE;
  virtual void OnHostMoved(const gfx::Point& origin) OVERRIDE;
  virtual void OnHostResized(const gfx::Size& size) OVERRIDE;
  virtual float GetDeviceScaleFactor() OVERRIDE;
  virtual RootWindow* AsRootWindow() OVERRIDE;
  virtual const RootWindow* AsRootWindow() const OVERRIDE;
  virtual ui::EventProcessor* GetEventProcessor() OVERRIDE;

  ui::EventDispatchDetails OnHostMouseEventImpl(ui::MouseEvent* event)
      WARN_UNUSED_RESULT;

  
  
  
  
  
  
  
  ui::EventDispatchDetails DispatchMouseEventImpl(ui::MouseEvent* event)
      WARN_UNUSED_RESULT;
  ui::EventDispatchDetails DispatchMouseEventRepost(ui::MouseEvent* event)
      WARN_UNUSED_RESULT;
  ui::EventDispatchDetails DispatchMouseEventToTarget(ui::MouseEvent* event,
                                                      Window* target)
      WARN_UNUSED_RESULT;
  ui::EventDispatchDetails DispatchTouchEventImpl(ui::TouchEvent* event)
      WARN_UNUSED_RESULT;
  ui::EventDispatchDetails DispatchHeldEvents() WARN_UNUSED_RESULT;
  
  
  ui::EventDispatchDetails SynthesizeMouseMoveEvent() WARN_UNUSED_RESULT;

  void SynthesizeMouseMoveEventAsync();

  
  
  void PostMouseMoveEventAfterWindowChange();

  gfx::Transform GetInverseRootTransform() const;

  void PreDispatchLocatedEvent(Window* target, ui::LocatedEvent* event);

  
  scoped_ptr<Window> window_;

  scoped_ptr<ui::Compositor> compositor_;

  scoped_ptr<RootWindowHost> host_;

  
  uint32 touch_ids_down_;

  
  gfx::NativeCursor last_cursor_;

  ObserverList<RootWindowObserver> observers_;

  Window* mouse_pressed_handler_;
  Window* mouse_moved_handler_;
  Window* event_dispatch_target_;
  Window* old_dispatch_target_;

  bool synthesize_mouse_move_;
  bool waiting_on_compositing_end_;
  bool draw_on_compositing_end_;

  bool defer_draw_scheduling_;

  
  
  int move_hold_count_;
  scoped_ptr<ui::LocatedEvent> held_move_event_;

  
  scoped_ptr<ui::LocatedEvent>  held_repostable_event_;

  
  bool dispatching_held_event_;

  scoped_ptr<ui::ViewProp> prop_;

  scoped_ptr<RootWindowTransformer> transformer_;

  
  base::WeakPtrFactory<RootWindow> repost_event_factory_;

  
  base::WeakPtrFactory<RootWindow> held_event_factory_;

  DISALLOW_COPY_AND_ASSIGN(RootWindow);
};

}  

#endif  
