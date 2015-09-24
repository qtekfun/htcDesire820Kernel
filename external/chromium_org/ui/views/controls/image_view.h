// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_IMAGE_VIEW_H_
#define UI_VIEWS_CONTROLS_IMAGE_VIEW_H_

#include "ui/gfx/image/image_skia.h"
#include "ui/views/view.h"

namespace gfx {
class Canvas;
}

namespace views {

class Painter;

class VIEWS_EXPORT ImageView : public View {
 public:
  enum Alignment {
    LEADING = 0,
    CENTER,
    TRAILING
  };

  ImageView();
  virtual ~ImageView();

  
  void SetImage(const gfx::ImageSkia& img);

  
  
  
  void SetImage(const gfx::ImageSkia* image_skia);

  
  
  const gfx::ImageSkia& GetImage();

  
  void SetImageSize(const gfx::Size& image_size);

  
  
  
  bool GetImageSize(gfx::Size* image_size);

  
  gfx::Rect GetImageBounds() const;

  
  void ResetImageSize();

  
  void SetHorizontalAlignment(Alignment ha);
  Alignment GetHorizontalAlignment() const;

  
  void SetVerticalAlignment(Alignment va);
  Alignment GetVerticalAlignment() const;

  
  void SetTooltipText(const string16& tooltip);
  string16 GetTooltipText() const;

  void set_interactive(bool interactive) { interactive_ = interactive; }

  void SetFocusPainter(scoped_ptr<Painter> focus_painter);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual bool GetTooltipText(const gfx::Point& p,
                              string16* tooltip) const OVERRIDE;
  virtual bool HitTestRect(const gfx::Rect& rect) const OVERRIDE;

 private:
  void OnPaintImage(gfx::Canvas* canvas);

  
  
  
  bool IsImageEqual(const gfx::ImageSkia& img) const;

  
  
  gfx::Point ComputeImageOrigin(const gfx::Size& image_size) const;

  
  bool image_size_set_;

  
  gfx::Size image_size_;

  
  gfx::ImageSkia image_;

  
  Alignment horiz_alignment_;

  
  Alignment vert_alignment_;

  
  string16 tooltip_text_;

  
  bool interactive_;

  
  float last_paint_scale_;

  
  
  void* last_painted_bitmap_pixels_;

  scoped_ptr<views::Painter> focus_painter_;

  DISALLOW_COPY_AND_ASSIGN(ImageView);
};

}  

#endif  
