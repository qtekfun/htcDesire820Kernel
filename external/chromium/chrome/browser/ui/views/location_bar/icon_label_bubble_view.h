// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_ICON_LABEL_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_ICON_LABEL_BUBBLE_VIEW_H_
#pragma once

#include <string>

#include "ui/gfx/size.h"
#include "views/painter.h"
#include "views/view.h"

namespace gfx {
class Canvas;
class Font;
}
namespace views {
class ImageView;
class Label;
}

class SkBitmap;

class IconLabelBubbleView : public views::View {
 public:
  IconLabelBubbleView(const int background_images[],
                      int contained_image,
                      const SkColor& color);
  virtual ~IconLabelBubbleView();

  void SetFont(const gfx::Font& font);
  void SetLabel(const std::wstring& label);
  void SetImage(const SkBitmap& bitmap);
  void set_is_extension_icon(bool is_extension_icon) {
    is_extension_icon_ = is_extension_icon;
  }

  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;

 protected:
  void SetElideInMiddle(bool elide_in_middle);
  gfx::Size GetNonLabelSize() const;

 private:
  int GetPreLabelWidth() const;
  int GetNonLabelWidth() const;

  
  views::HorizontalPainter background_painter_;

  
  views::ImageView* image_;
  views::Label* label_;

  bool is_extension_icon_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(IconLabelBubbleView);
};

#endif  