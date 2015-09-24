// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_TRAY_TRAY_ITEM_VIEW_H_
#define ASH_SYSTEM_TRAY_TRAY_ITEM_VIEW_H_

#include "ash/ash_export.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/views/view.h"

namespace gfx {
class SlideAnimation;
}

namespace views {
class ImageView;
class Label;
}

namespace ash {

class SystemTrayItem;

namespace internal {

class ASH_EXPORT TrayItemView : public views::View,
                                public gfx::AnimationDelegate {
 public:
  explicit TrayItemView(SystemTrayItem* owner);
  virtual ~TrayItemView();

  static void DisableAnimationsForTest();

  
  void CreateLabel();
  void CreateImageView();

  SystemTrayItem* owner() const { return owner_; }
  views::Label* label() const { return label_; }
  views::ImageView* image_view() const { return image_view_; }

  
  virtual void SetVisible(bool visible) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual int GetHeightForWidth(int width) OVERRIDE;

 protected:
  
  
  void ApplyChange();

  
  
  
  
  virtual gfx::Size DesiredSize();

  
  virtual int GetAnimationDurationMS();

 private:
  
  virtual void ChildPreferredSizeChanged(View* child) OVERRIDE;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationCanceled(const gfx::Animation* animation) OVERRIDE;

  SystemTrayItem* owner_;
  scoped_ptr<gfx::SlideAnimation> animation_;
  views::Label* label_;
  views::ImageView* image_view_;

  DISALLOW_COPY_AND_ASSIGN(TrayItemView);
};

}  
}  

#endif  
