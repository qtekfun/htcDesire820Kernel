// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_OVERVIEW_SCOPED_TRANSFORM_OVERVIEW_WINDOW_H_
#define ASH_WM_OVERVIEW_SCOPED_TRANSFORM_OVERVIEW_WINDOW_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_vector.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/transform.h"

namespace aura {
class Window;
}

namespace ui {
class Layer;
}

namespace views {
class Widget;
}

namespace ash {

class ScopedWindowCopy;

class ScopedTransformOverviewWindow {
 public:
  
  static const int kTransitionMilliseconds;

  
  
  static gfx::Rect ShrinkRectToFitPreservingAspectRatio(
      const gfx::Rect& rect,
      const gfx::Rect& bounds);

  
  static gfx::Transform GetTransformForRect(const gfx::Rect& src_rect,
                                            const gfx::Rect& dst_rect);

  explicit ScopedTransformOverviewWindow(aura::Window* window);
  virtual ~ScopedTransformOverviewWindow();

  
  
  bool Contains(const aura::Window* target) const;

  
  gfx::Rect GetBoundsInScreen() const;

  
  void RestoreWindow();

  
  
  void RestoreWindowOnExit();

  
  
  
  void OnWindowDestroyed();

  
  virtual void PrepareForOverview();

  
  
  
  void SetTransform(aura::Window* root_window,
                    const gfx::Transform& transform,
                    bool animate);

  aura::Window* window() const { return window_; }

 private:
  
  
  void CopyWindowAndTransientParents(aura::Window* target_root,
                                     aura::Window* window);

  
  
  
  void SetTransformOnWindowAndTransientChildren(const gfx::Transform& transform,
                                                bool animate);

  
  aura::Window* window_;

  
  ScopedVector<ScopedWindowCopy> window_copies_;

  
  
  bool minimized_;

  
  bool ignored_by_shelf_;

  
  bool overview_started_;

  
  gfx::Transform original_transform_;

  DISALLOW_COPY_AND_ASSIGN(ScopedTransformOverviewWindow);
};

}  

#endif  
