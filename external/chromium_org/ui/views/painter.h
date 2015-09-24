// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_PAINTER_H_
#define UI_VIEWS_PAINTER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/views/views_export.h"

namespace gfx {
class Canvas;
class ImageSkia;
class Insets;
class Rect;
class Size;
}

#define IMAGE_GRID(x) { x ## _TOP_LEFT,    x ## _TOP,    x ## _TOP_RIGHT, \
                        x ## _LEFT,        x ## _CENTER, x ## _RIGHT, \
                        x ## _BOTTOM_LEFT, x ## _BOTTOM, x ## _BOTTOM_RIGHT, }

namespace views {

class View;

class VIEWS_EXPORT Painter {
 public:
  Painter();
  virtual ~Painter();

  
  
  static void PaintPainterAt(gfx::Canvas* canvas,
                             Painter* painter,
                             const gfx::Rect& rect);

  
  
  static void PaintFocusPainter(View* view,
                                gfx::Canvas* canvas,
                                Painter* focus_painter);

  
  static Painter* CreateHorizontalGradient(SkColor c1, SkColor c2);
  static Painter* CreateVerticalGradient(SkColor c1, SkColor c2);

  
  
  
  
  static Painter* CreateVerticalMultiColorGradient(SkColor* colors,
                                                   SkScalar* pos,
                                                   size_t count);

  
  
  
  
  static Painter* CreateImagePainter(const gfx::ImageSkia& image,
                                     const gfx::Insets& insets);

  
  
  
  
  
  static Painter* CreateImageGridPainter(const int image_ids[]);

  
  static scoped_ptr<Painter> CreateDashedFocusPainter();
  static scoped_ptr<Painter> CreateDashedFocusPainterWithInsets(
      const gfx::Insets& insets);
  static scoped_ptr<Painter> CreateSolidFocusPainter(SkColor color,
                                                     const gfx::Insets& insets);

  
  
  virtual gfx::Size GetMinimumSize() const = 0;

  
  virtual void Paint(gfx::Canvas* canvas, const gfx::Size& size) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(Painter);
};

class VIEWS_EXPORT HorizontalPainter : public Painter {
 public:
  
  
  explicit HorizontalPainter(const int image_resource_names[]);
  virtual ~HorizontalPainter();

  
  virtual gfx::Size GetMinimumSize() const OVERRIDE;
  virtual void Paint(gfx::Canvas* canvas, const gfx::Size& size) OVERRIDE;

 private:
  
  enum BorderElements {
    LEFT,
    CENTER,
    RIGHT
  };

  
  const gfx::ImageSkia* images_[3];

  DISALLOW_COPY_AND_ASSIGN(HorizontalPainter);
};

}  

#endif  
