// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_INFO_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_INFO_BUBBLE_VIEW_H_
#pragma once

#import <Cocoa/Cocoa.h>

namespace info_bubble {

const CGFloat kBubbleArrowHeight = 8.0;
const CGFloat kBubbleArrowWidth = 15.0;
const CGFloat kBubbleCornerRadius = 8.0;
const CGFloat kBubbleArrowXOffset = kBubbleArrowWidth + kBubbleCornerRadius;

enum BubbleArrowLocation {
  kTopLeft,
  kTopRight,
};

}  

@interface InfoBubbleView : NSView {
 @private
  info_bubble::BubbleArrowLocation arrowLocation_;
}

@property(assign, nonatomic) info_bubble::BubbleArrowLocation arrowLocation;

- (NSPoint)arrowTip;

@end

#endif  
