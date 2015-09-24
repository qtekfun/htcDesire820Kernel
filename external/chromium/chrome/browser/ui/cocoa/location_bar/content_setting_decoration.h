// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_CONTENT_SETTING_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_CONTENT_SETTING_DECORATION_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/location_bar/image_decoration.h"
#include "chrome/common/content_settings_types.h"


@class ContentSettingAnimationState;
class ContentSettingImageModel;
class LocationBarViewMac;
class Profile;
class TabContents;

class ContentSettingDecoration : public ImageDecoration {
 public:
  ContentSettingDecoration(ContentSettingsType settings_type,
                           LocationBarViewMac* owner,
                           Profile* profile);
  virtual ~ContentSettingDecoration();

  
  
  bool UpdateFromTabContents(TabContents* tab_contents);

  
  virtual bool AcceptsMousePress();
  virtual bool OnMousePressed(NSRect frame);
  virtual NSString* GetToolTip();
  virtual CGFloat GetWidthForSpace(CGFloat width);
  virtual void DrawInFrame(NSRect frame, NSView* control_view);

  
  
  virtual void AnimationTimerFired();

 private:
  
  
  
  NSPoint GetBubblePointInFrame(NSRect frame);

  void SetToolTip(NSString* tooltip);

  
  
  NSAttributedString* CreateAnimatedText();

  
  CGFloat MeasureTextWidth();

  scoped_ptr<ContentSettingImageModel> content_setting_image_model_;

  LocationBarViewMac* owner_;  
  Profile* profile_;  

  scoped_nsobject<NSString> tooltip_;

  
  scoped_nsobject<ContentSettingAnimationState> animation_;
  CGFloat text_width_;
  scoped_nsobject<NSAttributedString> animated_text_;
  scoped_nsobject<NSGradient> gradient_;

  DISALLOW_COPY_AND_ASSIGN(ContentSettingDecoration);
};

#endif  
