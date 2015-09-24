// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_CONTENTS_CONTAINER_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_CONTENTS_CONTAINER_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "ui/base/animation/animation_delegate.h"
#include "views/view.h"

class TabContents;

namespace ui {
class SlideAnimation;
}

namespace views {
class Widget;
}

class ContentsContainer : public views::View, public ui::AnimationDelegate {
 public:
  explicit ContentsContainer(views::View* active);
  virtual ~ContentsContainer();

  
  
  
  void MakePreviewContentsActiveContents();

  
  void SetPreview(views::View* preview, TabContents* preview_tab_contents);

  TabContents* preview_tab_contents() const { return preview_tab_contents_; }

  
  void SetActiveTopMargin(int margin);

  
  
  gfx::Rect GetPreviewBounds();

  
  void FadeActiveContents();

  
  void ShowFade();

  
  void RemoveFade();

  
  virtual void Layout();

  
  virtual void AnimationProgressed(const ui::Animation* animation);

 private:
  class OverlayContentView;

  
  void CreateOverlay(int initial_opacity);

  
  void OverlayViewDestroyed();

  views::View* active_;

  views::View* preview_;

  TabContents* preview_tab_contents_;

  
  
  views::Widget* active_overlay_;

  
  OverlayContentView* overlay_view_;

  
  scoped_ptr<ui::SlideAnimation> overlay_animation_;

  
  
  int active_top_margin_;

  DISALLOW_COPY_AND_ASSIGN(ContentsContainer);
};

#endif  
