// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BUBBLE_BORDER_H_
#define CHROME_BROWSER_UI_VIEWS_BUBBLE_BORDER_H_
#pragma once

#include "third_party/skia/include/core/SkColor.h"
#include "views/background.h"
#include "views/border.h"

class SkBitmap;

class BubbleBorder : public views::Border {
 public:
  
  
  
  
  enum ArrowLocation {
    TOP_LEFT     = 0,
    TOP_RIGHT    = 1,
    BOTTOM_LEFT  = 2,
    BOTTOM_RIGHT = 3,
    LEFT_TOP     = 4,
    RIGHT_TOP    = 5,
    LEFT_BOTTOM  = 6,
    RIGHT_BOTTOM = 7,
    NONE  = 8,  
    FLOAT = 9   
  };

  explicit BubbleBorder(ArrowLocation arrow_location)
      : override_arrow_offset_(0),
        arrow_location_(arrow_location),
        background_color_(SK_ColorWHITE) {
    InitClass();
  }

  
  static int GetCornerRadius() {
    
    
    
    
    return 4;
  }

  
  void set_arrow_location(ArrowLocation arrow_location) {
    arrow_location_ = arrow_location;
  }
  ArrowLocation arrow_location() const { return arrow_location_; }

  static ArrowLocation horizontal_mirror(ArrowLocation loc) {
    return loc >= NONE ? loc : static_cast<ArrowLocation>(loc ^ 1);
  }

  static ArrowLocation vertical_mirror(ArrowLocation loc) {
    return loc >= NONE ? loc : static_cast<ArrowLocation>(loc ^ 2);
  }

  static bool has_arrow(ArrowLocation loc) {
    return loc >= NONE ? false : true;
  }

  static bool is_arrow_on_left(ArrowLocation loc) {
    return loc >= NONE ? false : !(loc & 1);
  }

  static bool is_arrow_on_top(ArrowLocation loc) {
    return loc >= NONE ? false : !(loc & 2);
  }

  static bool is_arrow_on_horizontal(ArrowLocation loc) {
    return loc >= NONE ? false : !(loc & 4);
  }

  
  
  void set_background_color(SkColor background_color) {
    background_color_ = background_color;
  }
  SkColor background_color() const { return background_color_; }

  
  
  
  
  gfx::Rect GetBounds(const gfx::Rect& position_relative_to,
                      const gfx::Size& contents_size) const;

  
  
  
  
  int SetArrowOffset(int offset, const gfx::Size& contents_size);

  
  virtual void GetInsets(gfx::Insets* insets) const;

 private:
  
  static void InitClass();

  virtual ~BubbleBorder() { }

  
  virtual void Paint(const views::View& view, gfx::Canvas* canvas) const;

  void DrawEdgeWithArrow(gfx::Canvas* canvas,
                         bool is_horizontal,
                         SkBitmap* edge,
                         SkBitmap* arrow,
                         int start_x,
                         int start_y,
                         int before_arrow,
                         int after_arrow,
                         int offset) const;

  void DrawArrowInterior(gfx::Canvas* canvas,
                         bool is_horizontal,
                         int tip_x,
                         int tip_y,
                         int shift_x,
                         int shift_y) const;

  
  static SkBitmap* left_;
  static SkBitmap* top_left_;
  static SkBitmap* top_;
  static SkBitmap* top_right_;
  static SkBitmap* right_;
  static SkBitmap* bottom_right_;
  static SkBitmap* bottom_;
  static SkBitmap* bottom_left_;
  static SkBitmap* left_arrow_;
  static SkBitmap* top_arrow_;
  static SkBitmap* right_arrow_;
  static SkBitmap* bottom_arrow_;

  
  static int arrow_offset_;

  
  int override_arrow_offset_;

  ArrowLocation arrow_location_;
  SkColor background_color_;

  DISALLOW_COPY_AND_ASSIGN(BubbleBorder);
};

class BubbleBackground : public views::Background {
 public:
  explicit BubbleBackground(BubbleBorder* border) : border_(border) {}

  
  virtual void Paint(gfx::Canvas* canvas, views::View* view) const;

 private:
  BubbleBorder* border_;

  DISALLOW_COPY_AND_ASSIGN(BubbleBackground);
};

#endif  
