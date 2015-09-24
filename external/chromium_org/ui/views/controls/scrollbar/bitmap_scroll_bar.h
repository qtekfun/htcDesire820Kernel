// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SCROLLBAR_BITMAP_SCROLL_BAR_H_
#define UI_VIEWS_CONTROLS_SCROLLBAR_BITMAP_SCROLL_BAR_H_

#include "ui/views/controls/scrollbar/base_scroll_bar.h"

namespace views {

namespace {
class BitmapScrollBarThumb;
}

class VIEWS_EXPORT BitmapScrollBar : public BaseScrollBar,
                                     public ButtonListener {
 public:
  BitmapScrollBar(bool horizontal, bool show_scroll_buttons);
  virtual ~BitmapScrollBar() { }

  
  enum ScrollBarPart {
    
    PREV_BUTTON = 0,
    
    
    
    NEXT_BUTTON,
    
    THUMB_START_CAP,
    
    THUMB_MIDDLE,
    
    THUMB_END_CAP,
    
    THUMB_GRIPPY,
    
    THUMB_TRACK,
    PART_COUNT
  };

  
  void SetImage(ScrollBarPart part,
                CustomButton::ButtonState state,
                gfx::ImageSkia* image_skia);


  gfx::Rect GetTrackBounds() const;

 protected:
  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  virtual int GetLayoutSize() const OVERRIDE;

  
  virtual void ButtonPressed(Button* sender,
                             const ui::Event& event) OVERRIDE;

 private:
  
  ImageButton* prev_button_;
  ImageButton* next_button_;

  
  friend BitmapScrollBarThumb;
  gfx::ImageSkia* images_[PART_COUNT][CustomButton::STATE_COUNT];

  
  bool show_scroll_buttons_;

  DISALLOW_COPY_AND_ASSIGN(BitmapScrollBar);
};

}  

#endif  
