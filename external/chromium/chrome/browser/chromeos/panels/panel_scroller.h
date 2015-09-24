// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_PANELS_PANEL_SCROLLER_H_
#define CHROME_BROWSER_CHROMEOS_PANELS_PANEL_SCROLLER_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/slide_animation.h"
#include "views/view.h"

class PanelScrollerHeader;

class PanelScroller : public views::View, public ui::AnimationDelegate {
 public:
  PanelScroller();
  ~PanelScroller();

  static PanelScroller* CreateWindow();

  
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View* parent,
                                    views::View* child) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual bool OnMousePressed(const views::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const views::MouseEvent& event) OVERRIDE;

  
  
  void HeaderClicked(PanelScrollerHeader* source);

 private:
  struct Panel;

  
  virtual void AnimationProgressed(const ui::Animation* animation);

  
  void ScrollToPanel(int index);

  
  std::vector<Panel*> panels_;

  
  int divider_height_;

  bool needs_layout_;

  
  int scroll_pos_;

  ui::SlideAnimation animation_;

  
  
  int animated_scroll_begin_;
  int animated_scroll_end_;

  DISALLOW_COPY_AND_ASSIGN(PanelScroller);
};

#endif  

