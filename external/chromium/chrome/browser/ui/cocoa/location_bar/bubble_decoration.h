// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_BUBBLE_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_BUBBLE_DECORATION_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_nsobject.h"
#include "chrome/browser/ui/cocoa/location_bar/location_bar_decoration.h"


class BubbleDecoration : public LocationBarDecoration {
 public:
  
  BubbleDecoration(NSFont* font);
  virtual ~BubbleDecoration();

  
  NSImage* GetImage();
  void SetImage(NSImage* image);
  void SetLabel(NSString* label);
  void SetColors(NSColor* border_color,
                 NSColor* background_color,
                 NSColor* text_color);

  
  virtual void DrawInFrame(NSRect frame, NSView* control_view);
  virtual CGFloat GetWidthForSpace(CGFloat width);

 protected:
  
  
  CGFloat GetWidthForImageAndLabel(NSImage* image, NSString* label);

  
  
  NSRect GetImageRectInFrame(NSRect frame);

 private:
  friend class SelectedKeywordDecorationTest;
  FRIEND_TEST_ALL_PREFIXES(SelectedKeywordDecorationTest,
                           UsesPartialKeywordIfNarrow);

  
  scoped_nsobject<NSDictionary> attributes_;

  
  scoped_nsobject<NSImage> image_;

  
  scoped_nsobject<NSString> label_;

  
  
  scoped_nsobject<NSColor> background_color_;
  scoped_nsobject<NSColor> border_color_;

  DISALLOW_COPY_AND_ASSIGN(BubbleDecoration);
};

#endif  
