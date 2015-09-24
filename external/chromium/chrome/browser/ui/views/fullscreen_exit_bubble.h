// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FULLSCREEN_EXIT_BUBBLE_H__
#define CHROME_BROWSER_UI_VIEWS_FULLSCREEN_EXIT_BUBBLE_H__
#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/timer.h"
#include "chrome/browser/command_updater.h"
#include "ui/base/animation/animation_delegate.h"
#include "views/controls/link.h"

namespace ui {
class SlideAnimation;
}

#if defined(OS_LINUX)
namespace views {
class WidgetGtk;
}
#endif


class FullscreenExitBubble : public views::LinkController,
                             public ui::AnimationDelegate {
 public:
  explicit FullscreenExitBubble(
      views::Widget* frame,
      CommandUpdater::CommandUpdaterDelegate* delegate);
  virtual ~FullscreenExitBubble();

 private:
  class FullscreenExitView;

  static const double kOpacity;          
  static const int kInitialDelayMs;      
  static const int kIdleTimeMs;          
  static const int kPositionCheckHz;     
  static const int kSlideInRegionHeightPx;
                                         
  static const int kSlideInDurationMs;   
  static const int kSlideOutDurationMs;  

  
  virtual void LinkActivated(views::Link* source, int event_flags);

  
  virtual void AnimationProgressed(const ui::Animation* animation);
  virtual void AnimationEnded(const ui::Animation* animation);

  
  
  void CheckMousePosition();

  
  
  void Hide();

  
  
  
  gfx::Rect GetPopupRect(bool ignore_animation_state) const;

  
  views::View* root_view_;

  
  
  CommandUpdater::CommandUpdaterDelegate* delegate_;

  views::Widget* popup_;

  
  FullscreenExitView* view_;

  
  scoped_ptr<ui::SlideAnimation> size_animation_;

  
  
  base::OneShotTimer<FullscreenExitBubble> initial_delay_;

  
  base::OneShotTimer<FullscreenExitBubble> idle_timeout_;

  
  
  
  
  base::RepeatingTimer<FullscreenExitBubble> mouse_position_checker_;

  
  
  gfx::Point last_mouse_pos_;

  DISALLOW_COPY_AND_ASSIGN(FullscreenExitBubble);
};

#endif  
