// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SCROLLBAR_NATIVE_SCROLL_BAR_VIEWS_H_
#define UI_VIEWS_CONTROLS_SCROLLBAR_NATIVE_SCROLL_BAR_VIEWS_H_

#include "base/compiler_specific.h"
#include "ui/gfx/point.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/scrollbar/base_scroll_bar.h"
#include "ui/views/controls/scrollbar/native_scroll_bar_wrapper.h"
#include "ui/views/view.h"

namespace gfx {
class Canvas;
}

namespace views {

class NativeScrollBar;

class VIEWS_EXPORT NativeScrollBarViews : public BaseScrollBar,
                                          public ButtonListener,
                                          public NativeScrollBarWrapper {
 public:
  static const char kViewClassName[];

  
  explicit NativeScrollBarViews(NativeScrollBar* native_scroll_bar);
  virtual ~NativeScrollBarViews();

 private:
  
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;

  
  virtual int GetLayoutSize() const OVERRIDE;

  
  virtual void ScrollToPosition(int position) OVERRIDE;
  virtual int GetScrollIncrement(bool is_page, bool is_positive) OVERRIDE;

  
  virtual void ButtonPressed(Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual int GetPosition() const OVERRIDE;
  virtual View* GetView() OVERRIDE;
  virtual void Update(int viewport_size,
                      int content_size,
                      int current_pos) OVERRIDE;

  
  
  virtual gfx::Rect GetTrackBounds() const OVERRIDE;

  
  NativeScrollBar* native_scroll_bar_;

  
  Button* prev_button_;
  Button* next_button_;

  ui::NativeTheme::ExtraParams params_;
  ui::NativeTheme::Part part_;
  ui::NativeTheme::State state_;

  DISALLOW_COPY_AND_ASSIGN(NativeScrollBarViews);
};

}  

#endif  
