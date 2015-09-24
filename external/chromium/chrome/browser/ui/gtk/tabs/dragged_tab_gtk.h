// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TABS_DRAGGED_TAB_GTK_H_
#define CHROME_BROWSER_UI_GTK_TABS_DRAGGED_TAB_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/slide_animation.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"

class TabContents;
class TabRendererGtk;

class DraggedTabGtk : public ui::AnimationDelegate {
 public:
  DraggedTabGtk(TabContents* datasource,
                const gfx::Point& mouse_tab_offset,
                const gfx::Size& contents_size,
                bool mini);
  virtual ~DraggedTabGtk();

  
  
  void MoveTo(const gfx::Point& screen_point);

  
  void set_mouse_tab_offset(const gfx::Point& offset) {
    mouse_tab_offset_ = offset;
  }

  
  void Attach(int selected_width);

  
  void Resize(int width);

  
  void Detach();

  
  void Update();

  
  
  typedef Callback0::Type AnimateToBoundsCallback;
  void AnimateToBounds(const gfx::Rect& bounds,
                       AnimateToBoundsCallback* callback);

  
  
  const gfx::Size& attached_tab_size() const { return attached_tab_size_; }

  GtkWidget* widget() const { return container_; }

 private:
  
  virtual void AnimationProgressed(const ui::Animation* animation);
  virtual void AnimationEnded(const ui::Animation* animation);
  virtual void AnimationCanceled(const ui::Animation* animation);

  
  void Layout();

  
  gfx::Size GetPreferredSize();

  
  void ResizeContainer();

  
  int ScaleValue(int value);

  
  gfx::Rect bounds() const;

  
  
  void SetContainerColorMap();

  
  
  void SetContainerTransparency();

  
  
  
  
  void SetContainerShapeMask(cairo_surface_t* surface);

  
  static gboolean OnExposeEvent(GtkWidget* widget, GdkEventExpose* event,
                                DraggedTabGtk* dragged_tab);

  
  TabContents* data_source_;

  
  GtkWidget* container_;

  
  
  GtkWidget* fixed_;

  
  scoped_ptr<TabRendererGtk> renderer_;

  
  
  bool attached_;

  
  
  
  
  gfx::Point mouse_tab_offset_;

  
  
  gfx::Size attached_tab_size_;

  
  gfx::Size contents_size_;

  
  ui::SlideAnimation close_animation_;

  
  scoped_ptr<Callback0::Type> animation_callback_;

  
  gfx::Rect animation_start_bounds_;
  gfx::Rect animation_end_bounds_;

  DISALLOW_COPY_AND_ASSIGN(DraggedTabGtk);
};

#endif  
