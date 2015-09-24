// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_BUBBLE_BUBBLE_BORDER_H_
#define UI_VIEWS_BUBBLE_BUBBLE_BORDER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/background.h"
#include "ui/views/border.h"

namespace gfx {
class ImageSkia;
class Rect;
}

namespace views {

namespace internal {
struct BorderImages;
}

class VIEWS_EXPORT BubbleBorder : public Border {
 public:
  
  
  
  
  
  enum ArrowMask {
    RIGHT    = 0x01,
    BOTTOM   = 0x02,
    VERTICAL = 0x04,
    CENTER   = 0x08,
  };

  enum Arrow {
    TOP_LEFT      = 0,
    TOP_RIGHT     = RIGHT,
    BOTTOM_LEFT   = BOTTOM,
    BOTTOM_RIGHT  = BOTTOM | RIGHT,
    LEFT_TOP      = VERTICAL,
    RIGHT_TOP     = VERTICAL | RIGHT,
    LEFT_BOTTOM   = VERTICAL | BOTTOM,
    RIGHT_BOTTOM  = VERTICAL | BOTTOM | RIGHT,
    TOP_CENTER    = CENTER,
    BOTTOM_CENTER = CENTER | BOTTOM,
    LEFT_CENTER   = CENTER | VERTICAL,
    RIGHT_CENTER  = CENTER | VERTICAL | RIGHT,
    NONE  = 16,  
    FLOAT = 17,  
  };

  enum Shadow {
    NO_SHADOW = 0,
    NO_SHADOW_OPAQUE_BORDER,
    BIG_SHADOW,
    SMALL_SHADOW,
    SHADOW_COUNT,
  };

  
  enum BubbleAlignment {
    
    ALIGN_ARROW_TO_MID_ANCHOR,
    
    ALIGN_EDGE_TO_ANCHOR_EDGE,
  };

  
  enum ArrowPaintType {
    
    PAINT_NORMAL,
    
    PAINT_TRANSPARENT,
    
    PAINT_NONE,
  };

  BubbleBorder(Arrow arrow, Shadow shadow, SkColor color);
  virtual ~BubbleBorder();

  
  
  static int GetCornerRadius() {
    
    
    
    
    return 4;
  }

  static bool has_arrow(Arrow a) { return a < NONE; }

  static bool is_arrow_on_left(Arrow a) {
    return has_arrow(a) && (a == LEFT_CENTER || !(a & (RIGHT | CENTER)));
  }

  static bool is_arrow_on_top(Arrow a) {
    return has_arrow(a) && (a == TOP_CENTER || !(a & (BOTTOM | CENTER)));
  }

  static bool is_arrow_on_horizontal(Arrow a) {
    return a >= NONE ? false : !(a & VERTICAL);
  }

  static bool is_arrow_at_center(Arrow a) {
    return has_arrow(a) && !!(a & CENTER);
  }

  static Arrow horizontal_mirror(Arrow a) {
    return (a == TOP_CENTER || a == BOTTOM_CENTER || a >= NONE) ?
        a : static_cast<Arrow>(a ^ RIGHT);
  }

  static Arrow vertical_mirror(Arrow a) {
    return (a == LEFT_CENTER || a == RIGHT_CENTER || a >= NONE) ?
        a : static_cast<Arrow>(a ^ BOTTOM);
  }

  
  void set_arrow(Arrow arrow) { arrow_ = arrow; }
  Arrow arrow() const { return arrow_; }

  
  void set_alignment(BubbleAlignment alignment) { alignment_ = alignment; }
  BubbleAlignment alignment() const { return alignment_; }

  
  Shadow shadow() const { return shadow_; }

  
  void set_background_color(SkColor color) { background_color_ = color; }
  SkColor background_color() const { return background_color_; }

  
  
  
  
  
  void set_arrow_offset(int offset) { arrow_offset_ = offset; }

  
  void set_paint_arrow(ArrowPaintType value) { arrow_paint_type_ = value; }

  
  
  virtual gfx::Rect GetBounds(const gfx::Rect& anchor_rect,
                              const gfx::Size& contents_size) const;

  
  int GetBorderThickness() const;

  
  int GetBorderCornerRadius() const;

  
  int GetArrowOffset(const gfx::Size& border_size) const;

  
  virtual void Paint(const View& view, gfx::Canvas* canvas) OVERRIDE;
  virtual gfx::Insets GetInsets() const OVERRIDE;
  virtual gfx::Size GetMinimumSize() const OVERRIDE;

 private:
  gfx::Size GetSizeForContentsSize(const gfx::Size& contents_size) const;
  gfx::ImageSkia* GetArrowImage() const;
  gfx::Rect GetArrowRect(const gfx::Rect& bounds) const;
  void DrawArrow(gfx::Canvas* canvas, const gfx::Rect& arrow_bounds) const;

  Arrow arrow_;
  int arrow_offset_;
  ArrowPaintType arrow_paint_type_;
  BubbleAlignment alignment_;
  Shadow shadow_;
  internal::BorderImages* images_;
  SkColor background_color_;

  DISALLOW_COPY_AND_ASSIGN(BubbleBorder);
};

class VIEWS_EXPORT BubbleBackground : public Background {
 public:
  explicit BubbleBackground(BubbleBorder* border) : border_(border) {}

  
  virtual void Paint(gfx::Canvas* canvas, View* view) const OVERRIDE;

 private:
  BubbleBorder* border_;

  DISALLOW_COPY_AND_ASSIGN(BubbleBackground);
};

}  

#endif  
