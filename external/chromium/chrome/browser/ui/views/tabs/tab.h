// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_TAB_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_TAB_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/views/tabs/base_tab.h"
#include "ui/gfx/point.h"

namespace ui {
class MultiAnimation;
class SlideAnimation;
}

class Tab : public BaseTab {
 public:
  
  static const char kViewClassName[];

  explicit Tab(TabController* controller);
  virtual ~Tab();

  
  void StartMiniTabTitleAnimation();
  void StopMiniTabTitleAnimation();

  
  
  void set_background_offset(const gfx::Point& offset) {
    background_offset_ = offset;
  }

  
  static gfx::Size GetMinimumUnselectedSize();
  
  
  
  static gfx::Size GetMinimumSelectedSize();
  
  
  static gfx::Size GetStandardSize();

  
  static int GetMiniWidth();

 protected:
  
  virtual const gfx::Rect& GetTitleBounds() const OVERRIDE;
  virtual const gfx::Rect& GetIconBounds() const OVERRIDE;
  virtual void DataChanged(const TabRendererData& old) OVERRIDE;

 private:
  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnThemeChanged() OVERRIDE;
  virtual std::string GetClassName() const OVERRIDE;
  virtual bool HasHitTestMask() const OVERRIDE;
  virtual void GetHitTestMask(gfx::Path* path) const OVERRIDE;
  virtual bool GetTooltipTextOrigin(const gfx::Point& p,
                                    gfx::Point* origin) OVERRIDE;
  virtual void OnMouseMoved(const views::MouseEvent& event) OVERRIDE;

  
  void PaintTabBackground(gfx::Canvas* canvas);
  void PaintInactiveTabBackgroundWithTitleChange(gfx::Canvas* canvas);
  void PaintInactiveTabBackground(gfx::Canvas* canvas);
  void PaintActiveTabBackground(gfx::Canvas* canvas);
  SkBitmap DrawHoverGlowBitmap(int width, int height);

  
  
  int IconCapacity() const;

  
  bool ShouldShowIcon() const;

  
  bool ShouldShowCloseBox() const;

  
  
  
  double GetThrobValue();

  
  static void InitTabResources();

  
  static void LoadTabImages();

  
  gfx::Rect favicon_bounds_;
  gfx::Rect title_bounds_;

  
  gfx::Point background_offset_;

  
  gfx::Point hover_point_;

  
  scoped_ptr<ui::MultiAnimation> mini_title_animation_;

  struct TabImage {
    SkBitmap* image_l;
    SkBitmap* image_c;
    SkBitmap* image_r;
    int l_width;
    int r_width;
    int y_offset;
  };
  static TabImage tab_active_;
  static TabImage tab_inactive_;
  static TabImage tab_alpha_;

  
  
  bool showing_icon_;

  
  
  bool showing_close_button_;

  
  SkColor close_button_color_;

  static bool initialized_;

  DISALLOW_COPY_AND_ASSIGN(Tab);
};

#endif  
