// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SCROLLBAR_NATIVE_SCROLL_BAR_H_
#define UI_VIEWS_CONTROLS_SCROLLBAR_NATIVE_SCROLL_BAR_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "ui/views/controls/scrollbar/scroll_bar.h"
#include "ui/views/view.h"

namespace ui {
class NativeTheme;
}

namespace views {

class NativeScrollBarWrapper;

class VIEWS_EXPORT NativeScrollBar : public ScrollBar {
 public:
  
  static const char kViewClassName[];

  
  explicit NativeScrollBar(bool is_horiz);
  virtual ~NativeScrollBar();

  
  static int GetHorizontalScrollBarHeight(const ui::NativeTheme* theme);
  static int GetVerticalScrollBarWidth(const ui::NativeTheme* theme);

 private:
  friend class NativeScrollBarTest;
  FRIEND_TEST_ALL_PREFIXES(NativeScrollBarTest, Scrolling);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;

  
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& e) OVERRIDE;

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  virtual void Update(int viewport_size,
                      int content_size,
                      int current_pos) OVERRIDE;
  virtual int GetPosition() const OVERRIDE;
  virtual int GetLayoutSize() const OVERRIDE;

  
  NativeScrollBarWrapper* native_wrapper_;

  DISALLOW_COPY_AND_ASSIGN(NativeScrollBar);
};

}  

#endif  
