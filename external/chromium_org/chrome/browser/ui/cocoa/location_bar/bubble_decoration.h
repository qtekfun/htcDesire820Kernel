// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_BUBBLE_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_BUBBLE_DECORATION_H_

#import <Cocoa/Cocoa.h>

#include "base/gtest_prod_util.h"
#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/ui/cocoa/location_bar/location_bar_decoration.h"
#import "ui/base/cocoa/appkit_utils.h"


class BubbleDecoration : public LocationBarDecoration {
 public:
  BubbleDecoration();
  virtual ~BubbleDecoration();

  
  NSImage* GetImage();
  void SetImage(NSImage* image);
  void SetLabel(NSString* label);
  void SetTextColor(NSColor* text_color);
  virtual ui::NinePartImageIds GetBubbleImageIds() = 0;

  
  virtual void DrawInFrame(NSRect frame, NSView* control_view) OVERRIDE;
  virtual void DrawWithBackgroundInFrame(NSRect background_frame,
                                         NSRect frame,
                                         NSView* control_view) OVERRIDE;
  virtual CGFloat GetWidthForSpace(CGFloat width) OVERRIDE;

 protected:
  
  
  CGFloat GetWidthForImageAndLabel(NSImage* image, NSString* label);

  
  
  NSRect GetImageRectInFrame(NSRect frame);

 private:
  friend class SelectedKeywordDecorationTest;
  FRIEND_TEST_ALL_PREFIXES(SelectedKeywordDecorationTest,
                           UsesPartialKeywordIfNarrow);

  
  base::scoped_nsobject<NSImage> image_;

  
  base::scoped_nsobject<NSString> label_;

  
  base::scoped_nsobject<NSMutableDictionary> attributes_;

  DISALLOW_COPY_AND_ASSIGN(BubbleDecoration);
};

#endif  
