// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_PINNED_CONTENTS_INFO_BUBBLE_H_
#define CHROME_BROWSER_UI_VIEWS_PINNED_CONTENTS_INFO_BUBBLE_H_
#pragma once

#include "chrome/browser/ui/views/bubble/border_contents.h"
#include "chrome/browser/ui/views/bubble/bubble.h"

class PinnedContentsBorderContents : public BorderContents {
 public:
  explicit PinnedContentsBorderContents(const gfx::Point& bubble_anchor)
      : bubble_anchor_(bubble_anchor) {}

  
  virtual void SizeAndGetBounds(
      const gfx::Rect& position_relative_to,  
      BubbleBorder::ArrowLocation arrow_location,
      bool allow_bubble_offscreen,
      const gfx::Size& contents_size,
      gfx::Rect* contents_bounds,             
      gfx::Rect* window_bounds);              

 private:
  
  const gfx::Point bubble_anchor_;

  DISALLOW_COPY_AND_ASSIGN(PinnedContentsBorderContents);
};

class PinnedContentsInfoBubble : public Bubble {
 public:
  
  
  
  
  
  static PinnedContentsInfoBubble* Show(
      views::Widget* parent,
      const gfx::Rect& position_relative_to,
      BubbleBorder::ArrowLocation arrow_location,
      const gfx::Point& bubble_anchor_,
      views::View* contents,
      BubbleDelegate* delegate);

  
  virtual BorderContents* CreateBorderContents();

 private:
  explicit PinnedContentsInfoBubble(const gfx::Point& bubble_anchor)
      : bubble_anchor_(bubble_anchor) {}
  virtual ~PinnedContentsInfoBubble() {}

  
  const gfx::Point bubble_anchor_;

  DISALLOW_COPY_AND_ASSIGN(PinnedContentsInfoBubble);
};

#endif  
