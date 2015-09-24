// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_INFO_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_INFO_BUBBLE_VIEW_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"

namespace info_bubble {

const CGFloat kBubbleArrowHeight = 8.0;
const CGFloat kBubbleArrowWidth = 15.0;
const CGFloat kBubbleCornerRadius = 2.0;
const CGFloat kBubbleArrowXOffset = kBubbleArrowWidth + kBubbleCornerRadius;

enum CornerFlags {
  kRoundedTopCorners = 1,
  kRoundedBottomCorners = 1 << 1,
  kRoundedAllCorners = kRoundedTopCorners | kRoundedBottomCorners,
};

enum BubbleArrowLocation {
  kTopLeft,
  kTopCenter,
  kTopRight,
  kNoArrow,
};

enum BubbleAlignment {
  
  kAlignArrowToAnchor,
  
  kAlignEdgeToAnchorEdge,
  
  kAlignRightEdgeToAnchorEdge,
  
  kAlignLeftEdgeToAnchorEdge,
};

}  

@interface InfoBubbleView : NSView {
 @private
  info_bubble::BubbleArrowLocation arrowLocation_;
  info_bubble::BubbleAlignment alignment_;
  info_bubble::CornerFlags cornerFlags_;
  base::scoped_nsobject<NSColor> backgroundColor_;
}

@property(assign, nonatomic) info_bubble::BubbleArrowLocation arrowLocation;
@property(assign, nonatomic) info_bubble::BubbleAlignment alignment;
@property(assign, nonatomic) info_bubble::CornerFlags cornerFlags;

- (NSPoint)arrowTip;

- (NSColor*)backgroundColor;
- (void)setBackgroundColor:(NSColor*)backgroundColor;

@end

#endif  
