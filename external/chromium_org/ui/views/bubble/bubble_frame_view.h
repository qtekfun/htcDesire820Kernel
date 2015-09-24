// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_BUBBLE_BUBBLE_FRAME_VIEW_H_
#define UI_VIEWS_BUBBLE_BUBBLE_FRAME_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "ui/gfx/insets.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/window/non_client_view.h"

namespace views {

class Label;
class LabelButton;
class BubbleBorder;

class VIEWS_EXPORT BubbleFrameView : public NonClientFrameView,
                                     public ButtonListener {
 public:
  
  static const char kViewClassName[];

  
  
  static gfx::Insets GetTitleInsets();

  explicit BubbleFrameView(const gfx::Insets& content_margins);
  virtual ~BubbleFrameView();

  
  virtual gfx::Rect GetBoundsForClientView() const OVERRIDE;
  virtual gfx::Rect GetWindowBoundsForClientBounds(
      const gfx::Rect& client_bounds) const OVERRIDE;
  virtual int NonClientHitTest(const gfx::Point& point) OVERRIDE;
  virtual void GetWindowMask(const gfx::Size& size,
                             gfx::Path* window_mask) OVERRIDE;
  virtual void ResetWindowControls() OVERRIDE;
  virtual void UpdateWindowIcon() OVERRIDE;
  virtual void UpdateWindowTitle() OVERRIDE;

  
  virtual gfx::Insets GetInsets() const OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;
  virtual void ChildPreferredSizeChanged(View* child) OVERRIDE;
  virtual void OnThemeChanged() OVERRIDE;

  
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE;

  
  BubbleBorder* bubble_border() const { return bubble_border_; }
  void SetBubbleBorder(BubbleBorder* border);

  gfx::Insets content_margins() const { return content_margins_; }

  void SetTitlebarExtraView(View* view);

  
  
  
  gfx::Rect GetUpdatedWindowBounds(const gfx::Rect& anchor_rect,
                                   gfx::Size client_size,
                                   bool adjust_if_offscreen);

 protected:
  
  virtual gfx::Rect GetAvailableScreenBounds(const gfx::Rect& rect);

 private:
  FRIEND_TEST_ALL_PREFIXES(BubbleFrameViewTest, GetBoundsForClientView);

  
  
  void MirrorArrowIfOffScreen(bool vertical,
                              const gfx::Rect& anchor_rect,
                              const gfx::Size& client_size);

  
  
  void OffsetArrowIfOffScreen(const gfx::Rect& anchor_rect,
                              const gfx::Size& client_size);

  
  gfx::Size GetSizeForClientSize(const gfx::Size& client_size);

  
  BubbleBorder* bubble_border_;

  
  gfx::Insets content_margins_;

  
  Label* title_;
  LabelButton* close_;

  
  
  View* titlebar_extra_view_;

  DISALLOW_COPY_AND_ASSIGN(BubbleFrameView);
};

}  

#endif  
