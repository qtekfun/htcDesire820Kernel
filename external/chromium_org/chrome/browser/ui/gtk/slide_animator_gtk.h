// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_SLIDE_ANIMATOR_GTK_H_
#define CHROME_BROWSER_UI_GTK_SLIDE_ANIMATOR_GTK_H_

#include <gtk/gtk.h>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/gfx/animation/animation_delegate.h"

namespace gfx {
class SlideAnimation;
}

class SlideAnimatorGtk : public gfx::AnimationDelegate {
 public:
  class Delegate {
   public:
    
    virtual void Closed() = 0;

   protected:
    virtual ~Delegate() {}
  };

  enum Direction {
    DOWN,
    UP
  };

  
  
  
  
  
  
  
  SlideAnimatorGtk(GtkWidget* child,
                   Direction direction,
                   int duration,
                   bool linear,
                   bool control_child_size,
                   Delegate* delegate);

  virtual ~SlideAnimatorGtk();

  GtkWidget* widget() { return widget_.get(); }

  
  void Open();

  
  void OpenWithoutAnimation();

  
  void Close();

  
  void End();

  
  void CloseWithoutAnimation();

  
  bool IsShowing();

  
  bool IsClosing();

  
  
  bool IsAnimating();

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;

  
  static void SetAnimationsForTesting(bool enable);

 private:
  static void OnChildSizeAllocate(GtkWidget* child,
                                  GtkAllocation* allocation,
                                  SlideAnimatorGtk* slider);

  scoped_ptr<gfx::SlideAnimation> animation_;

  
  ui::OwnedWidgetGtk widget_;

  
  
  GtkWidget* child_;

  
  Direction direction_;

  
  Delegate* delegate_;

  
  
  
  
  bool child_needs_move_;

  static bool animations_enabled_;
};

#endif  
