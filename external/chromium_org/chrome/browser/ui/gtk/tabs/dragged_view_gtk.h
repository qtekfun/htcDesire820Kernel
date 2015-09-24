// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TABS_DRAGGED_VIEW_GTK_H_
#define CHROME_BROWSER_UI_GTK_TABS_DRAGGED_VIEW_GTK_H_

#include <gtk/gtk.h>

#include <vector>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/slide_animation.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"

class DragData;
class TabRendererGtk;

class DraggedViewGtk : public gfx::AnimationDelegate {
 public:
  DraggedViewGtk(DragData* drag_data,
                const gfx::Point& mouse_tab_offset,
                const gfx::Size& contents_size);
  virtual ~DraggedViewGtk();

  
  
  
  void MoveAttachedTo(const gfx::Point& tabstrip_point);

  
  
  void MoveDetachedTo(const gfx::Point& screen_point);

  
  void set_mouse_tab_offset(const gfx::Point& offset) {
    mouse_tab_offset_ = offset;
  }

  
  
  
  
  void Attach(int normal_width, int mini_width, int parent_window_width);

  
  void Resize(int width);

  
  void Detach();

  
  void Update();

  
  
  void AnimateToBounds(const gfx::Rect& bounds, const base::Closure& callback);

  
  
  const gfx::Size& attached_tab_size() const { return attached_tab_size_; }
  int GetAttachedTabWidthAt(int index);

  GtkWidget* widget() const { return container_; }

  int mini_width() { return mini_width_; }
  int normal_width() { return normal_width_; }

  
  
  int GetWidthInTabStripFromTo(int from, int to);

  
  int GetTotalWidthInTabStrip();

  
  
  int GetWidthInTabStripUpToSourceTab();

  
  
  
  int GetWidthInTabStripUpToMousePointer();

  
  
  gfx::Point GetDistanceFromTabStripOriginToMousePointer();

 private:
  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationCanceled(const gfx::Animation* animation) OVERRIDE;

  
  void Layout();

  
  gfx::Size GetPreferredSize();

  
  void ResizeContainer();

  
  int ScaleValue(int value);

  
  gfx::Rect bounds() const;

  
  
  void SetContainerColorMap();

  
  
  void SetContainerTransparency();

  
  
  
  
  void SetContainerShapeMask();

  void PaintTab(int index, GtkWidget* widget, cairo_t* cr, int widget_width);

  
  CHROMEGTK_CALLBACK_1(DraggedViewGtk, gboolean, OnExpose, GdkEventExpose*);

  
  GtkWidget* container_;

  
  
  GtkWidget* fixed_;

  
  std::vector<TabRendererGtk*> renderers_;

  
  
  DragData* drag_data_;

  
  int mini_width_;

  
  
  int normal_width_;

  
  
  bool attached_;

  
  
  int parent_window_width_;

  
  
  
  
  gfx::Point mouse_tab_offset_;

  
  
  gfx::Size attached_tab_size_;

  
  gfx::Size contents_size_;

  
  gfx::SlideAnimation close_animation_;

  
  base::Closure animation_callback_;

  
  gfx::Rect animation_start_bounds_;
  gfx::Rect animation_end_bounds_;

  DISALLOW_COPY_AND_ASSIGN(DraggedViewGtk);
};

#endif  
