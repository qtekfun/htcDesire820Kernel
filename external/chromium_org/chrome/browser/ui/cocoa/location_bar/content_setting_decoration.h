// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_CONTENT_SETTING_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_CONTENT_SETTING_DECORATION_H_

#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/location_bar/image_decoration.h"
#include "chrome/common/content_settings_types.h"


@class ContentSettingAnimationState;
class ContentSettingImageModel;
class LocationBarViewMac;
class Profile;

namespace content {
class WebContents;
}

class ContentSettingDecoration : public ImageDecoration {
 public:
  ContentSettingDecoration(ContentSettingsType settings_type,
                           LocationBarViewMac* owner,
                           Profile* profile);
  virtual ~ContentSettingDecoration();

  
  
  bool UpdateFromWebContents(content::WebContents* web_contents);

  
  virtual bool AcceptsMousePress() OVERRIDE;
  virtual bool OnMousePressed(NSRect frame) OVERRIDE;
  virtual NSString* GetToolTip() OVERRIDE;
  virtual CGFloat GetWidthForSpace(CGFloat width) OVERRIDE;
  virtual void DrawInFrame(NSRect frame, NSView* control_view) OVERRIDE;

  
  
  virtual void AnimationTimerFired();

 private:
  
  
  
  NSPoint GetBubblePointInFrame(NSRect frame);

  void SetToolTip(NSString* tooltip);

  
  base::scoped_nsobject<NSAttributedString> CreateAnimatedText();

  
  CGFloat MeasureTextWidth();

  scoped_ptr<ContentSettingImageModel> content_setting_image_model_;

  LocationBarViewMac* owner_;  
  Profile* profile_;  

  base::scoped_nsobject<NSString> tooltip_;

  
  base::scoped_nsobject<ContentSettingAnimationState> animation_;
  CGFloat text_width_;
  base::scoped_nsobject<NSAttributedString> animated_text_;

  DISALLOW_COPY_AND_ASSIGN(ContentSettingDecoration);
};

#endif  
