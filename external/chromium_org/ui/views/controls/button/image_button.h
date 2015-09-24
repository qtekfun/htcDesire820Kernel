// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_BUTTON_IMAGE_BUTTON_H_
#define UI_VIEWS_CONTROLS_BUTTON_IMAGE_BUTTON_H_

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/layout.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/views/controls/button/custom_button.h"

namespace views {

class Painter;



class VIEWS_EXPORT ImageButton : public CustomButton {
 public:
  static const char kViewClassName[];

  enum HorizontalAlignment {
    ALIGN_LEFT = 0,
    ALIGN_CENTER,
    ALIGN_RIGHT
  };

  enum VerticalAlignment {
    ALIGN_TOP = 0,
    ALIGN_MIDDLE,
    ALIGN_BOTTOM
  };

  explicit ImageButton(ButtonListener* listener);
  virtual ~ImageButton();

  
  virtual const gfx::ImageSkia& GetImage(ButtonState state) const;

  
  virtual void SetImage(ButtonState state, const gfx::ImageSkia* image);

  
  void SetBackground(SkColor color,
                     const gfx::ImageSkia* image,
                     const gfx::ImageSkia* mask);

  
  
  void SetOverlayImage(const gfx::ImageSkia* image);

  
  void SetImageAlignment(HorizontalAlignment h_align,
                         VerticalAlignment v_align);

  void SetFocusPainter(scoped_ptr<Painter> focus_painter);

  
  
  void SetPreferredSize(const gfx::Size& preferred_size) {
    preferred_size_ = preferred_size;
  }

  
  void SetDrawImageMirrored(bool mirrored) {
    draw_image_mirrored_ = mirrored;
  }

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

 protected:
  
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;

  
  
  virtual gfx::ImageSkia GetImageToPaint();

  
  void UpdateButtonBackground(ui::ScaleFactor scale_factor);

  Painter* focus_painter() { return focus_painter_.get(); }

  
  gfx::ImageSkia images_[STATE_COUNT];

  gfx::ImageSkia background_image_;

  
  gfx::ImageSkia overlay_image_;

 private:
  FRIEND_TEST_ALL_PREFIXES(ImageButtonTest, Basics);
  FRIEND_TEST_ALL_PREFIXES(ImageButtonTest, ImagePositionWithBorder);
  FRIEND_TEST_ALL_PREFIXES(ImageButtonTest, LeftAlignedMirrored);
  FRIEND_TEST_ALL_PREFIXES(ImageButtonTest, RightAlignedMirrored);

  
  gfx::Point ComputeImagePaintPosition(const gfx::ImageSkia& image);

  
  HorizontalAlignment h_alignment_;
  VerticalAlignment v_alignment_;
  gfx::Size preferred_size_;

  
  
  
  
  bool draw_image_mirrored_;

  scoped_ptr<Painter> focus_painter_;

  DISALLOW_COPY_AND_ASSIGN(ImageButton);
};

class VIEWS_EXPORT ToggleImageButton : public ImageButton {
 public:
  explicit ToggleImageButton(ButtonListener* listener);
  virtual ~ToggleImageButton();

  
  void SetToggled(bool toggled);

  
  
  
  void SetToggledImage(ButtonState state, const gfx::ImageSkia* image);

  
  void SetToggledTooltipText(const string16& tooltip);

  
  virtual const gfx::ImageSkia& GetImage(ButtonState state) const OVERRIDE;
  virtual void SetImage(ButtonState state,
                        const gfx::ImageSkia* image) OVERRIDE;

  
  virtual bool GetTooltipText(const gfx::Point& p,
                              string16* tooltip) const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

 private:
  
  
  
  gfx::ImageSkia alternate_images_[STATE_COUNT];

  
  bool toggled_;

  
  
  string16 toggled_tooltip_text_;

  DISALLOW_COPY_AND_ASSIGN(ToggleImageButton);
};

}  

#endif  
