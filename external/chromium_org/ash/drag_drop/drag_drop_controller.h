// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DRAG_DROP_DRAG_DROP_CONTROLLER_H_
#define ASH_DRAG_DROP_DRAG_DROP_CONTROLLER_H_

#include "ash/ash_export.h"
#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "ui/aura/client/drag_drop_client.h"
#include "ui/aura/window_observer.h"
#include "ui/base/dragdrop/os_exchange_data.h"
#include "ui/events/event_constants.h"
#include "ui/events/event_handler.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/rect.h"

namespace gfx {
class LinearAnimation;
}

namespace ash {

namespace test {
class DragDropControllerTest;
}

namespace internal {

class DragDropTracker;
class DragDropTrackerDelegate;
class DragImageView;

class ASH_EXPORT DragDropController
    : public aura::client::DragDropClient,
      public ui::EventHandler,
      public gfx::AnimationDelegate,
      public aura::WindowObserver {
 public:
  DragDropController();
  virtual ~DragDropController();

  void set_should_block_during_drag_drop(bool should_block_during_drag_drop) {
    should_block_during_drag_drop_ = should_block_during_drag_drop;
  }

  
  virtual int StartDragAndDrop(
      const ui::OSExchangeData& data,
      aura::Window* root_window,
      aura::Window* source_window,
      const gfx::Point& root_location,
      int operation,
      ui::DragDropTypes::DragEventSource source) OVERRIDE;
  virtual void DragUpdate(aura::Window* target,
                          const ui::LocatedEvent& event) OVERRIDE;
  virtual void Drop(aura::Window* target,
                    const ui::LocatedEvent& event) OVERRIDE;
  virtual void DragCancel() OVERRIDE;
  virtual bool IsDragDropInProgress() OVERRIDE;

  
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  virtual void OnWindowDestroyed(aura::Window* window) OVERRIDE;

 protected:
  
  
  
  virtual gfx::LinearAnimation* CreateCancelAnimation(
      int duration,
      int frame_rate,
      gfx::AnimationDelegate* delegate);

  
  virtual void DoDragCancel(int drag_cancel_animation_duration_ms);

 private:
  friend class ash::test::DragDropControllerTest;

  
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationCanceled(const gfx::Animation* animation) OVERRIDE;

  
  void StartCanceledAnimation(int animation_duration_ms);

  
  void ForwardPendingLongTap();

  
  void Cleanup();

  scoped_ptr<DragImageView> drag_image_;
  gfx::Vector2d drag_image_offset_;
  const ui::OSExchangeData* drag_data_;
  int drag_operation_;

  
  aura::Window* drag_window_;

  
  gfx::Rect drag_image_initial_bounds_for_cancel_animation_;
  gfx::Rect drag_image_final_bounds_for_cancel_animation_;

  scoped_ptr<gfx::LinearAnimation> cancel_animation_;

  
  aura::Window* drag_source_window_;

  
  
  bool should_block_during_drag_drop_;

  
  base::Closure quit_closure_;

  scoped_ptr<ash::internal::DragDropTracker> drag_drop_tracker_;
  scoped_ptr<DragDropTrackerDelegate> drag_drop_window_delegate_;

  ui::DragDropTypes::DragEventSource current_drag_event_source_;

  
  
  scoped_ptr<ui::GestureEvent> pending_long_tap_;

  base::WeakPtrFactory<DragDropController> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(DragDropController);
};

}  
}  

#endif  
