// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SCROLLBAR_KENNEDY_SCROLL_BAR_H_
#define UI_VIEWS_CONTROLS_SCROLLBAR_KENNEDY_SCROLL_BAR_H_

#include "ui/views/controls/scrollbar/base_scroll_bar.h"

namespace views {

class VIEWS_EXPORT KennedyScrollBar : public BaseScrollBar {
 public:
  explicit KennedyScrollBar(bool horizontal);
  virtual ~KennedyScrollBar();

 protected:
  
  virtual gfx::Rect GetTrackBounds() const OVERRIDE;

  
  virtual int GetLayoutSize() const OVERRIDE;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(KennedyScrollBar);
};

}  

#endif  
