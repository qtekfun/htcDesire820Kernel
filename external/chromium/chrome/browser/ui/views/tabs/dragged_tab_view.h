// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_DRAGGED_TAB_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_DRAGGED_TAB_VIEW_H_
#pragma once

#include <vector>

#include "build/build_config.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "views/view.h"

class NativeViewPhotobooth;

class DraggedTabView : public views::View {
 public:
  
  
  DraggedTabView(const std::vector<views::View*>& renderers,
                 const std::vector<gfx::Rect>& renderer_bounds,
                 const gfx::Point& mouse_tab_offset,
                 const gfx::Size& contents_size,
                 NativeViewPhotobooth* photobooth);
  virtual ~DraggedTabView();

  
  
  void MoveTo(const gfx::Point& screen_point);

  
  void set_mouse_tab_offset(const gfx::Point& offset) {
    mouse_tab_offset_ = offset;
  }

  
  void Update();

 private:
  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  void PaintDetachedView(gfx::Canvas* canvas);

  
  void PaintFocusRect(gfx::Canvas* canvas);

  
  gfx::Size PreferredContainerSize();

  
  int ScaleValue(int value);

  
  scoped_ptr<views::Widget> container_;

  
  std::vector<views::View*> renderers_;

  
  std::vector<gfx::Rect> renderer_bounds_;

  
  bool show_contents_on_drag_;

  
  
  
  
  gfx::Point mouse_tab_offset_;

  
  gfx::Size tab_size_;

  
  
  scoped_ptr<NativeViewPhotobooth> photobooth_;

  
  gfx::Size contents_size_;

  DISALLOW_COPY_AND_ASSIGN(DraggedTabView);
};

#endif  
