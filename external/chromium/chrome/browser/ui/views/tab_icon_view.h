// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TAB_ICON_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_TAB_ICON_VIEW_H_
#pragma once

#include "views/view.h"

class SkBitmap;
class TabContents;

class TabIconView : public views::View {
 public:
  
  class TabIconViewModel {
   public:
    
    virtual bool ShouldTabIconViewAnimate() const = 0;

    
    virtual SkBitmap GetFaviconForTabIconView() = 0;
  };

  static void InitializeIfNeeded();

  explicit TabIconView(TabIconViewModel* provider);
  virtual ~TabIconView();

  
  void Update();

  
  void set_is_light(bool is_light) { is_light_ = is_light; }

  
  virtual void OnPaint(gfx::Canvas* canvas);
  virtual gfx::Size GetPreferredSize();

 private:
  void PaintThrobber(gfx::Canvas* canvas);
  void PaintFavicon(gfx::Canvas* canvas, const SkBitmap& bitmap);
  void PaintIcon(gfx::Canvas* canvas,
                 const SkBitmap& bitmap,
                 int src_x,
                 int src_y,
                 int src_w,
                 int src_h,
                 bool filter);

  
  TabIconViewModel* model_;

  
  bool throbber_running_;

  
  bool is_light_;

  
  
  int throbber_frame_;

  DISALLOW_COPY_AND_ASSIGN(TabIconView);
};

#endif  
