// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INFOBARS_INFOBAR_H_
#define CHROME_BROWSER_INFOBARS_INFOBAR_H_

#include <utility>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/infobars/infobar_delegate.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/slide_animation.h"
#include "ui/gfx/size.h"

class InfoBarContainer;
class InfoBarService;

class InfoBar : public gfx::AnimationDelegate {
 public:
  
  typedef InfoBar AddedDetails;
  typedef std::pair<InfoBar*, bool> RemovedDetails;
  typedef std::pair<InfoBar*, InfoBar*> ReplacedDetails;

  
  static const int kDefaultBarTargetHeight;
  static const int kSeparatorLineHeight;
  static const int kDefaultArrowTargetHeight;
  static const int kMaximumArrowTargetHeight;
  
  
  static const int kDefaultArrowTargetHalfWidth;
  static const int kMaximumArrowTargetHalfWidth;

  explicit InfoBar(scoped_ptr<InfoBarDelegate> delegate);
  virtual ~InfoBar();

  static SkColor GetTopColor(InfoBarDelegate::Type infobar_type);
  static SkColor GetBottomColor(InfoBarDelegate::Type infobar_type);

  InfoBarService* owner() { return owner_; }
  InfoBarDelegate* delegate() { return delegate_.get(); }
  const InfoBarDelegate* delegate() const { return delegate_.get(); }
  void set_container(InfoBarContainer* container) { container_ = container; }

  
  
  
  void SetOwner(InfoBarService* owner);

  
  
  void Show(bool animate);

  
  
  
  
  void Hide(bool animate);

  
  
  void SetArrowTargetHeight(int height);

  
  
  void CloseSoon();

  
  
  void RemoveSelf();

  
  void SetBarTargetHeight(int height);

  const gfx::SlideAnimation& animation() const { return animation_; }
  int arrow_height() const { return arrow_height_; }
  int arrow_target_height() const { return arrow_target_height_; }
  int arrow_half_width() const { return arrow_half_width_; }
  int total_height() const { return arrow_height_ + bar_height_; }

 protected:
  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

  const InfoBarContainer* container() const { return container_; }
  InfoBarContainer* container() { return container_; }
  gfx::SlideAnimation* animation() { return &animation_; }
  int bar_height() const { return bar_height_; }
  int bar_target_height() const { return bar_target_height_; }

  
  
  virtual void PlatformSpecificSetOwner() {}
  virtual void PlatformSpecificShow(bool animate) {}
  virtual void PlatformSpecificHide(bool animate) {}
  virtual void PlatformSpecificOnCloseSoon() {}
  virtual void PlatformSpecificOnHeightsRecalculated() {}

 private:
  
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;

  
  
  
  
  void RecalculateHeights(bool force_notify);

  
  
  
  void MaybeDelete();

  InfoBarService* owner_;
  scoped_ptr<InfoBarDelegate> delegate_;
  InfoBarContainer* container_;
  gfx::SlideAnimation animation_;

  
  
  
  int arrow_height_;         
  int arrow_target_height_;
  int arrow_half_width_;     
  int bar_height_;           
  int bar_target_height_;

  DISALLOW_COPY_AND_ASSIGN(InfoBar);
};

#endif  
