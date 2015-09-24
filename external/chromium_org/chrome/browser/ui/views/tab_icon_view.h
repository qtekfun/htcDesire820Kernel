// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TAB_ICON_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_TAB_ICON_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/view.h"

namespace chrome {
class TabIconViewModel;
}

namespace gfx {
class ImageSkia;
}

class TabIconView : public views::View {
 public:
  static void InitializeIfNeeded();

  explicit TabIconView(chrome::TabIconViewModel* model);
  virtual ~TabIconView();

  
  void Update();

  
  void set_is_light(bool is_light) { is_light_ = is_light; }

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;

 private:
  void PaintThrobber(gfx::Canvas* canvas);
  void PaintFavicon(gfx::Canvas* canvas, const gfx::ImageSkia& image);
  void PaintIcon(gfx::Canvas* canvas,
                 const gfx::ImageSkia& image,
                 int src_x,
                 int src_y,
                 int src_w,
                 int src_h,
                 bool filter);

  
  chrome::TabIconViewModel* model_;

  
  bool throbber_running_;

  
  bool is_light_;

  
  
  int throbber_frame_;

  DISALLOW_COPY_AND_ASSIGN(TabIconView);
};

#endif  
