// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"


enum {
  kRoundedTopLeftCorner = 1,
  kRoundedTopRightCorner = 1 << 1,
  kRoundedBottomLeftCorner = 1 << 2,
  kRoundedBottomRightCorner = 1 << 3,
  kRoundedAllCorners = kRoundedTopLeftCorner |
                       kRoundedTopRightCorner |
                       kRoundedBottomLeftCorner |
                       kRoundedBottomRightCorner
};

enum {
  kBubbleViewTextPositionX = 4,
  kBubbleViewTextPositionY = 2
};

@interface BubbleView : NSView {
 @private
  base::scoped_nsobject<NSString> content_;
  unsigned long cornerFlags_;
  
  
  
  NSWindow* themeProvider_;
}

- (id)initWithFrame:(NSRect)frame themeProvider:(NSWindow*)provider;

- (void)setContent:(NSString*)content;

- (void)setCornerFlags:(unsigned long)flags;

- (void)setThemeProvider:(NSWindow*)provider;

- (NSFont*)font;

@end

@interface BubbleView(TestingOnly)
- (NSString*)content;
- (unsigned long)cornerFlags;
@end
