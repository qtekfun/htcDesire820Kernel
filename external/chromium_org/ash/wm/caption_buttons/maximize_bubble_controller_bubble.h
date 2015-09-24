// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_CAPTION_BUTTONS_MAXIMIZE_BUBBLE_CONTROLLER_BUBBLE_H_
#define ASH_WM_CAPTION_BUTTONS_MAXIMIZE_BUBBLE_CONTROLLER_BUBBLE_H_

#include "ash/wm/workspace/snap_types.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/mouse_watcher.h"

namespace views {
class CustomButton;
}

namespace ash {

class BubbleContentsView;
class MaximizeBubbleBorder;
class MaximizeBubbleController;

class MaximizeBubbleControllerBubble : public views::BubbleDelegateView,
                                       public views::MouseWatcherListener {
 public:
  static const SkColor kBubbleBackgroundColor;
  static const int kLayoutSpacing;  

  MaximizeBubbleControllerBubble(MaximizeBubbleController* owner,
                                 int appearance_delay_ms,
                                 SnapType initial_snap_type);
  virtual ~MaximizeBubbleControllerBubble();

  
  aura::Window* GetBubbleWindow();

  
  virtual gfx::Rect GetAnchorRect() OVERRIDE;
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual bool CanActivate() const OVERRIDE;

  
  virtual bool WidgetHasHitTestMask() const OVERRIDE;
  virtual void GetWidgetHitTestMask(gfx::Path* mask) const OVERRIDE;

  
  virtual void MouseMovedOutOfHost() OVERRIDE;

  
  virtual bool Contains(const gfx::Point& screen_point,
                        views::MouseWatcherHost::MouseEventType type);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;

  
  
  virtual void ControllerRequestsCloseAndDelete();

  
  
  void SetSnapType(SnapType snap_type);

  
  
  MaximizeBubbleController* controller() const { return owner_; }

  
  
  views::CustomButton* GetButtonForUnitTest(SnapType state);

 private:
  
  bool shutting_down_;

  
  MaximizeBubbleController* owner_;

  
  views::Widget* bubble_widget_;

  
  BubbleContentsView* contents_view_;

  
  MaximizeBubbleBorder* bubble_border_;

  
  gfx::Rect initial_position_;

  
  scoped_ptr<views::MouseWatcher> mouse_watcher_;

  
  const int appearance_delay_ms_;

  DISALLOW_COPY_AND_ASSIGN(MaximizeBubbleControllerBubble);
};

}  

#endif  
