// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_FULLSCREEN_FULLSCREEN_EXIT_BUBBLE_H_
#define CHROME_BROWSER_UI_FULLSCREEN_FULLSCREEN_EXIT_BUBBLE_H_

#include "base/timer/timer.h"
#include "chrome/browser/ui/fullscreen/fullscreen_exit_bubble_type.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/point.h"
#include "url/gurl.h"

class Browser;

namespace gfx {
class Rect;
}

class FullscreenExitBubble : public gfx::AnimationDelegate {
 public:
  explicit FullscreenExitBubble(Browser* browser,
                                const GURL& url,
                                FullscreenExitBubbleType bubble_type);
  virtual ~FullscreenExitBubble();

 protected:
  static const int kPaddingPx;           
  static const int kInitialDelayMs;      
  static const int kIdleTimeMs;          
  static const int kPositionCheckHz;     
  static const int kSlideInRegionHeightPx;
                                         
                                         
  static const int kPopupTopPx;          
                                         
  static const int kSlideInDurationMs;   
  static const int kSlideOutDurationMs;  

  
  
  
  virtual gfx::Rect GetPopupRect(bool ignore_animation_state) const = 0;
  virtual gfx::Point GetCursorScreenPoint() = 0;
  virtual bool WindowContainsPoint(gfx::Point pos) = 0;

  
  virtual bool IsWindowActive() = 0;

  
  
  virtual void Hide() = 0;

  
  virtual void Show() = 0;

  virtual bool IsAnimating() = 0;

  
  
  virtual bool CanMouseTriggerSlideIn() const = 0;

  void StartWatchingMouse();
  void StopWatchingMouse();
  bool IsWatchingMouse() const;

  
  
  void CheckMousePosition();

  void ToggleFullscreen();
  
  void Accept();
  
  void Cancel();

  
  base::string16 GetCurrentMessageText() const;
  base::string16 GetCurrentDenyButtonText() const;

  
  base::string16 GetAllowButtonText() const;
  base::string16 GetInstructionText() const;

  
  Browser* browser_;

  
  GURL url_;

  
  FullscreenExitBubbleType bubble_type_;

 private:
  
  
  base::OneShotTimer<FullscreenExitBubble> initial_delay_;

  
  base::OneShotTimer<FullscreenExitBubble> idle_timeout_;

  
  
  
  
  base::RepeatingTimer<FullscreenExitBubble> mouse_position_checker_;

  
  
  gfx::Point last_mouse_pos_;

  DISALLOW_COPY_AND_ASSIGN(FullscreenExitBubble);
};

#endif  
