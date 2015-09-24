// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TOUCH_TABS_TOUCH_TAB_H_
#define CHROME_BROWSER_UI_TOUCH_TABS_TOUCH_TAB_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/views/tabs/base_tab.h"
#include "ui/gfx/point.h"

class TouchTab : public BaseTab {
 public:
  
  static const char kViewClassName[];

  explicit TouchTab(TabController* controller);
  virtual ~TouchTab();

  
  
  void set_background_offset(const gfx::Point& offset) {
    background_offset_ = offset;
  }

  
  static gfx::Size GetMinimumUnselectedSize();

 protected:
  virtual const gfx::Rect& GetTitleBounds() const;
  virtual const gfx::Rect& GetIconBounds() const;

 private:
  
  virtual bool OnMousePressed(const views::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const views::MouseEvent& event) OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas);
  virtual void Layout();
  virtual bool HasHitTestMask() const;
  virtual void GetHitTestMask(gfx::Path* path) const;

  
  void PaintTabBackground(gfx::Canvas* canvas);
  void PaintIcon(gfx::Canvas* canvas);
  void PaintActiveTabBackground(gfx::Canvas* canvas);

  
  

  
  static void InitTabResources();

  
  static void LoadTabImages();

  
  gfx::Rect title_bounds_;

  
  gfx::Rect favicon_bounds_;

  
  gfx::Point background_offset_;

  
  
  
  struct TouchTabImage {
    SkBitmap* image_l;
    SkBitmap* image_c;
    SkBitmap* image_r;
    int l_width;
    int r_width;
    int y_offset;
  };
  static TouchTabImage tab_active;
  static TouchTabImage tab_inactive;
  static TouchTabImage tab_alpha;

  DISALLOW_COPY_AND_ASSIGN(TouchTab);
};

#endif  
