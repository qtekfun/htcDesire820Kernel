// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_AUTOFILL_INFO_BUBBLE_H_
#define CHROME_BROWSER_UI_VIEWS_AUTOFILL_INFO_BUBBLE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "ui/gfx/insets.h"
#include "ui/views/bubble/bubble_delegate.h"

namespace autofill {

class InfoBubbleFrame;

class InfoBubble : public views::BubbleDelegateView {
 public:
  InfoBubble(views::View* anchor, const base::string16& message);
  virtual ~InfoBubble();

  
  void Show();

  
  void Hide();

  
  void UpdatePosition();

  
  virtual views::NonClientFrameView* CreateNonClientFrameView(
      views::Widget* widget) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void OnWidgetDestroyed(views::Widget* widget) OVERRIDE;
  virtual void OnWidgetBoundsChanged(views::Widget* widget,
                                     const gfx::Rect& new_bounds) OVERRIDE;

  views::View* anchor() { return anchor_; }

  void set_align_to_anchor_edge(bool align_to_anchor_edge) {
    align_to_anchor_edge_ = align_to_anchor_edge;
  }

  void set_preferred_width(int preferred_width) {
    preferred_width_ = preferred_width;
  }

  void set_show_above_anchor(bool show_above_anchor) {
    show_above_anchor_ = show_above_anchor;
  }

 private:
  views::Widget* widget_;  
  views::View* const anchor_;  
  InfoBubbleFrame* frame_;  

  
  
  bool align_to_anchor_edge_;

  
  int preferred_width_;

  
  bool show_above_anchor_;

  DISALLOW_COPY_AND_ASSIGN(InfoBubble);
};

}  

#endif  
