// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_VIEWS_PADDED_BUTTON_H_
#define UI_MESSAGE_CENTER_VIEWS_PADDED_BUTTON_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/controls/button/image_button.h"

namespace message_center {

class PaddedButton : public views::ImageButton {
 public:
  PaddedButton(views::ButtonListener* listener);
  virtual ~PaddedButton();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnFocus() OVERRIDE;

  
  
  
  
  void SetPadding(int horizontal_padding, int vertical_padding);

  void SetNormalImage(int resource_id);
  void SetHoveredImage(int resource_id);
  void SetPressedImage(int resource_id);

 protected:
  gfx::Point ComputePaddedImagePaintPosition(const gfx::ImageSkia& image);

 private:
  gfx::Insets padding_;

  DISALLOW_COPY_AND_ASSIGN(PaddedButton);
};

}  

#endif  
