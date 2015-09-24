// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_PAGE_INFO_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_PAGE_INFO_BUBBLE_VIEW_H_
#pragma once

#include "chrome/browser/page_info_model.h"
#include "chrome/browser/ui/views/bubble/bubble.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/slide_animation.h"
#include "views/controls/link.h"
#include "views/view.h"

namespace views {
class Label;
}

class PageInfoBubbleView : public views::View,
                           public PageInfoModel::PageInfoModelObserver,
                           public BubbleDelegate,
                           public views::LinkController,
                           public ui::AnimationDelegate {
 public:
  PageInfoBubbleView(gfx::NativeWindow parent_window,
                     Profile* profile,
                     const GURL& url,
                     const NavigationEntry::SSLStatus& ssl,
                     bool show_history);
  virtual ~PageInfoBubbleView();

  
  void ShowCertDialog();

  void set_bubble(Bubble* bubble) { bubble_ = bubble; }

  
  virtual gfx::Size GetPreferredSize();

  
  virtual void ModelChanged();

  
  virtual void BubbleClosing(Bubble* bubble, bool closed_by_escape);
  virtual bool CloseOnEscape();
  virtual bool FadeInOnShow();
  virtual std::wstring accessible_name();

  
  virtual void LinkActivated(views::Link* source, int event_flags);

  
  virtual void AnimationEnded(const ui::Animation* animation);
  virtual void AnimationProgressed(const ui::Animation* animation);

 private:
  
  void LayoutSections();

  
  static Bubble* bubble_;

  
  PageInfoModel model_;

  
  gfx::NativeWindow parent_window_;

  
  int cert_id_;

  
  views::Link* help_center_link_;

  
  ui::SlideAnimation resize_animation_;

  
  int animation_start_height_;

  DISALLOW_COPY_AND_ASSIGN(PageInfoBubbleView);
};

#endif  
