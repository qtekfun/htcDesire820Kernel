// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_SLIDE_ANIMATOR_GTK_H_
#define CHROME_BROWSER_UI_GTK_SLIDE_ANIMATOR_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "ui/base/animation/animation_delegate.h"

namespace ui {
class SlideAnimation;
}

class SlideAnimatorGtk : public ui::AnimationDelegate {
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

  
  virtual void AnimationProgressed(const ui::Animation* animation);
  virtual void AnimationEnded(const ui::Animation* animation);

  
  static void SetAnimationsForTesting(bool enable);

 private:
  static void OnChildSizeAllocate(GtkWidget* child,
                                  GtkAllocation* allocation,
                                  SlideAnimatorGtk* slider);

  scoped_ptr<ui::SlideAnimation> animation_;

  
  OwnedWidgetGtk widget_;

  
  
  GtkWidget* child_;

  
  Direction direction_;

  
  Delegate* delegate_;

  
  
  
  
  bool child_needs_move_;

  static bool animations_enabled_;
};

#endif  
