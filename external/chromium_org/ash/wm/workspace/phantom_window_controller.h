// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WORKSPACE_PHANTOM_WINDOW_CONTROLLER_H_
#define ASH_WM_WORKSPACE_PHANTOM_WINDOW_CONTROLLER_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/rect.h"

namespace aura {
class Window;
}

namespace gfx {
class SlideAnimation;
}

namespace views {
class Widget;
}

namespace ash {
namespace internal {

class ASH_EXPORT PhantomWindowController : public gfx::AnimationDelegate {
 public:
  explicit PhantomWindowController(aura::Window* window);
  virtual ~PhantomWindowController();

  
  const gfx::Rect& bounds_in_screen() const { return bounds_in_screen_; }

  
  
  
  
  
  void Show(const gfx::Rect& bounds_in_screen);

  
  void Hide();

  
  bool IsShowing() const;

  
  
  void set_phantom_below_window(aura::Window* phantom_below_window) {
    phantom_below_window_ = phantom_below_window;
  }

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(WorkspaceWindowResizerTest, PhantomWindowShow);

  
  
  views::Widget* CreatePhantomWidget(aura::Window* root_window,
                                     const gfx::Rect& bounds_in_screen);

  
  aura::Window* window_;

  
  aura::Window* phantom_below_window_;

  
  
  
  
  gfx::Rect start_bounds_;

  
  gfx::Rect bounds_in_screen_;

  
  
  views::Widget* phantom_widget_;

  
  
  
  views::Widget* phantom_widget_start_;

  
  scoped_ptr<gfx::SlideAnimation> animation_;

  DISALLOW_COPY_AND_ASSIGN(PhantomWindowController);
};

}  
}  

#endif  
