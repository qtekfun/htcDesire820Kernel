// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_OVERFLOW_BUTTON_H_
#define ASH_SHELF_OVERFLOW_BUTTON_H_

#include "ash/shelf/shelf_types.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/views/controls/button/custom_button.h"

namespace ash {
namespace internal {

class OverflowButton : public views::CustomButton {
 public:
  explicit OverflowButton(views::ButtonListener* listener);
  virtual ~OverflowButton();

  void OnShelfAlignmentChanged();

 private:
  void PaintBackground(gfx::Canvas* canvas, int alpha);

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  
  gfx::ImageSkia left_image_;
  gfx::ImageSkia right_image_;
  
  const gfx::ImageSkia* bottom_image_;

  DISALLOW_COPY_AND_ASSIGN(OverflowButton);
};

}  
}  

#endif  
