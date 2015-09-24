// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_DETACHABLE_TOOLBAR_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_DETACHABLE_TOOLBAR_VIEW_H_

#include "chrome/browser/ui/host_desktop.h"
#include "ui/views/accessible_pane_view.h"

struct SkRect;

class DetachableToolbarView : public views::AccessiblePaneView {
 public:
  
  static const SkColor kEdgeDividerColor;
  
  static const SkColor kMiddleDividerColor;

  DetachableToolbarView() {}
  virtual ~DetachableToolbarView() {}

  
  virtual bool IsDetached() const = 0;

  
  virtual double GetAnimationValue() const = 0;

  
  virtual int GetToolbarOverlap() const = 0;

  
  
  
  static void PaintBackgroundAttachedMode(
      gfx::Canvas* canvas,
      ui::ThemeProvider* theme_provider,
      const gfx::Rect& bounds,
      const gfx::Point& background_origin,
     chrome::HostDesktopType host_desktop_type);

  
  
  
  
  
  
  static void CalculateContentArea(double animation_state,
                                   double horizontal_padding,
                                   double vertical_padding,
                                   SkRect* rect,
                                   double* roundness,
                                   views::View* view);

  
  
  static void PaintHorizontalBorder(gfx::Canvas* canvas,
                                    DetachableToolbarView* view,
                                    bool at_top,
                                    SkColor color);

  
  
  
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
                                   SkColor top_color,
                                   SkColor middle_color,
                                   SkColor bottom_color);

 private:
  DISALLOW_COPY_AND_ASSIGN(DetachableToolbarView);
};

#endif  
