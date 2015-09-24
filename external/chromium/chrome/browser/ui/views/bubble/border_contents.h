// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BUBBLE_BORDER_CONTENTS_H_
#define CHROME_BROWSER_UI_VIEWS_BUBBLE_BORDER_CONTENTS_H_
#pragma once

#include "chrome/browser/ui/views/bubble/bubble_border.h"
#include "third_party/skia/include/core/SkColor.h"
#include "views/view.h"

class BorderContents : public views::View {
 public:
  BorderContents() : bubble_border_(NULL) { }

  
  void Init();

  
  void SetBackgroundColor(SkColor color);

  
  
  
  
  
  
  virtual void SizeAndGetBounds(
      const gfx::Rect& position_relative_to,  
      BubbleBorder::ArrowLocation arrow_location,
      bool allow_bubble_offscreen,
      const gfx::Size& contents_size,
      gfx::Rect* contents_bounds,             
      gfx::Rect* window_bounds);              

 protected:
  virtual ~BorderContents() { }

  
  
  virtual gfx::Rect GetMonitorBounds(const gfx::Rect& rect);

  
  static const int kLeftMargin = 6;
  static const int kTopMargin = 6;
  static const int kRightMargin = 6;
  static const int kBottomMargin = 9;

  BubbleBorder* bubble_border_;

 private:
  
  
  
  void MirrorArrowIfOffScreen(bool vertical,
                              const gfx::Rect& position_relative_to,
                              const gfx::Rect& monitor_bounds,
                              const gfx::Size& local_contents_size,
                              BubbleBorder::ArrowLocation* arrow_location,
                              gfx::Rect* window_bounds);

  
  
  
  
  static bool ComputeOffScreenInsets(const gfx::Rect& monitor_bounds,
                                     const gfx::Rect& window_bounds,
                                     gfx::Insets* offscreen_insets);

  
  
  static int GetInsetsLength(const gfx::Insets& insets, bool vertical);

  DISALLOW_COPY_AND_ASSIGN(BorderContents);
};

#endif  
