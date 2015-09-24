// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_DETACHABLE_TOOLBAR_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_DETACHABLE_TOOLBAR_VIEW_H_
#pragma once

#include "chrome/browser/ui/views/accessible_pane_view.h"

class SkBitmap;
struct SkRect;

class DetachableToolbarView : public AccessiblePaneView {
 public:
  
  static const SkColor kEdgeDividerColor;
  
  static const SkColor kMiddleDividerColor;

  DetachableToolbarView() {}
  virtual ~DetachableToolbarView() {}

  
  virtual bool IsDetached() const = 0;

  
  virtual double GetAnimationValue() const = 0;

  
  virtual int GetToolbarOverlap() const = 0;

  
  
  
  static void PaintBackgroundAttachedMode(gfx::Canvas* canvas,
                                          views::View* view,
                                          const gfx::Point& background_origin);

  
  
  
  
  
  
  static void CalculateContentArea(double animation_state,
                                   double horizontal_padding,
                                   double vertical_padding,
                                   SkRect* rect,
                                   double* roundness,
                                   views::View* view);

  
  static void PaintHorizontalBorder(gfx::Canvas* canvas,
                                    DetachableToolbarView* view);

  
  
  
  static void PaintContentAreaBackground(gfx::Canvas* canvas,
                                         ui::ThemeProvider* theme_provider,
                                         const SkRect& rect,
                                         double roundness);
  
  static void PaintContentAreaBorder(gfx::Canvas* canvas,
                                     ui::ThemeProvider* theme_provider,
                                     const SkRect& rect,
                                     double roundness);

  
  
  
  
  
  
  
  static void PaintVerticalDivider(gfx::Canvas* canvas,
                                   int x,
                                   int height,
                                   int vertical_padding,
                                   const SkColor& top_color,
                                   const SkColor& middle_color,
                                   const SkColor& bottom_color);

 private:
  DISALLOW_COPY_AND_ASSIGN(DetachableToolbarView);
};

#endif  
