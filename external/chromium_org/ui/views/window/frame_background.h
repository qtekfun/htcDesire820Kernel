// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WINDOW_FRAME_BACKGROUND_H_
#define UI_VIEWS_WINDOW_FRAME_BACKGROUND_H_

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/views/views_export.h"

namespace gfx {
class Canvas;
class ImageSkia;
}

namespace views {

class View;

class VIEWS_EXPORT FrameBackground {
 public:
  FrameBackground();
  ~FrameBackground();

  
  void set_frame_color(SkColor color) { frame_color_ = color; }

  
  
  void set_theme_image(const gfx::ImageSkia* image) { theme_image_ = image; }

  
  
  
  void set_theme_overlay_image(gfx::ImageSkia* image) {
    theme_overlay_image_ = image;
  }

  
  
  void set_top_area_height(int height) { top_area_height_ = height; }

  
  void set_theme_background_y(int y) { theme_background_y_ = y; }

  
  void set_maximized_top_offset(int offset) { maximized_top_offset_ = offset; }

  
  
  void SetSideImages(const gfx::ImageSkia* left,
                     const gfx::ImageSkia* top,
                     const gfx::ImageSkia* right,
                     const gfx::ImageSkia* bottom);

  
  
  void SetCornerImages(const gfx::ImageSkia* top_left,
                       const gfx::ImageSkia* top_right,
                       const gfx::ImageSkia* bottom_left,
                       const gfx::ImageSkia* bottom_right);

  
  
  
  void PaintRestored(gfx::Canvas* canvas, View* view) const;

  
  
  void PaintMaximized(gfx::Canvas* canvas, View* view) const;

 private:
  
  void PaintFrameColor(gfx::Canvas* canvas, View* view) const;

  SkColor frame_color_;
  const gfx::ImageSkia* theme_image_;
  gfx::ImageSkia* theme_overlay_image_;
  int top_area_height_;

  
  const gfx::ImageSkia* left_edge_;
  const gfx::ImageSkia* top_edge_;
  const gfx::ImageSkia* right_edge_;
  const gfx::ImageSkia* bottom_edge_;

  
  const gfx::ImageSkia* top_left_corner_;
  const gfx::ImageSkia* top_right_corner_;
  const gfx::ImageSkia* bottom_left_corner_;
  const gfx::ImageSkia* bottom_right_corner_;

  
  
  gfx::ImageSkia* maximized_top_left_;
  gfx::ImageSkia* maximized_top_right_;
  int maximized_top_offset_;
  int theme_background_y_;

  DISALLOW_COPY_AND_ASSIGN(FrameBackground);
};

}  

#endif  
