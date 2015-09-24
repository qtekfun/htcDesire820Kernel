// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_CONTENT_SETTING_IMAGE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_CONTENT_SETTING_IMAGE_VIEW_H_

#include "base/memory/scoped_ptr.h"
#include "chrome/common/content_settings_types.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/slide_animation.h"
#include "ui/views/painter.h"
#include "ui/views/view.h"
#include "ui/views/widget/widget_observer.h"

class ContentSettingImageModel;
class LocationBarView;

namespace content {
class WebContents;
}

namespace gfx {
class FontList;
}

namespace views {
class ImageView;
class Label;
}

class ContentSettingImageView : public gfx::AnimationDelegate,
                                public views::View,
                                public views::WidgetObserver {
 public:
  ContentSettingImageView(ContentSettingsType content_type,
                          LocationBarView* parent,
                          const gfx::FontList& font_list,
                          SkColor text_color,
                          SkColor parent_background_color);
  virtual ~ContentSettingImageView();

  
  void Update(content::WebContents* web_contents);

 private:
  
  
  static const int kOpenTimeMS;

  
  
  static const int kAnimationDurationMS;

  
  
  
  
  
  static int GetBubbleOuterPadding(bool by_icon);

  
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationCanceled(const gfx::Animation* animation) OVERRIDE;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual void OnPaintBackground(gfx::Canvas* canvas) OVERRIDE;

  
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;

  bool background_showing() const {
    return slide_animator_.is_animating() || pause_animation_;
  }

  int GetTotalSpacingWhileAnimating() const;
  void OnClick();

  LocationBarView* parent_;  
  scoped_ptr<ContentSettingImageModel> content_setting_image_model_;
  scoped_ptr<views::Painter> background_painter_;
  views::ImageView* icon_;
  views::Label* text_label_;
  gfx::SlideAnimation slide_animator_;
  bool pause_animation_;
  double pause_animation_state_;
  views::Widget* bubble_widget_;

  DISALLOW_COPY_AND_ASSIGN(ContentSettingImageView);
};

#endif  
