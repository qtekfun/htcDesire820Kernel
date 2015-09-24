// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_INFOBARS_INFOBAR_H_
#define CHROME_BROWSER_UI_VIEWS_INFOBARS_INFOBAR_H_
#pragma once

#include "base/basictypes.h"
#include "base/scoped_ptr.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/gfx/size.h"

class InfoBarContainer;
class InfoBarDelegate;

namespace ui {
class SlideAnimation;
}

class InfoBar : public ui::AnimationDelegate {
 public:
  explicit InfoBar(InfoBarDelegate* delegate);
  virtual ~InfoBar();

  
  static const int kSeparatorLineHeight;
  static const int kDefaultArrowTargetHeight;
  static const int kMaximumArrowTargetHeight;
  
  
  static const int kDefaultArrowTargetHalfWidth;
  static const int kMaximumArrowTargetHalfWidth;

  InfoBarDelegate* delegate() { return delegate_; }
  void set_container(InfoBarContainer* container) { container_ = container; }

  
  
  void Show(bool animate);

  
  
  
  void Hide(bool animate);

  
  
  void SetArrowTargetHeight(int height);

  const ui::SlideAnimation* animation() const { return animation_.get(); }
  int arrow_height() const { return arrow_height_; }
  int total_height() const { return arrow_height_ + bar_height_; }

 protected:
  
  static const int kDefaultBarTargetHeight;

  
  virtual void AnimationProgressed(const ui::Animation* animation) OVERRIDE;

  
  
  void RemoveInfoBar();

  
  void SetBarTargetHeight(int height);

  
  
  
  int OffsetY(const gfx::Size& prefsize) const;

  const InfoBarContainer* container() const { return container_; }
  ui::SlideAnimation* animation() { return animation_.get(); }
  int arrow_half_width() const { return arrow_half_width_; }
  int bar_height() const { return bar_height_; }

  
  
  virtual void PlatformSpecificHide(bool animate) {}
  virtual void PlatformSpecificOnHeightsRecalculated() {}

 private:
  
  virtual void AnimationEnded(const ui::Animation* animation) OVERRIDE;

  
  
  
  
  void RecalculateHeights(bool force_notify);

  
  
  
  void MaybeDelete();

  InfoBarDelegate* delegate_;
  InfoBarContainer* container_;
  scoped_ptr<ui::SlideAnimation> animation_;

  
  
  
  int arrow_height_;         
  int arrow_target_height_;
  int arrow_half_width_;     
  int bar_height_;           
  int bar_target_height_;

  DISALLOW_COPY_AND_ASSIGN(InfoBar);
};

#endif  
