// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BACKGROUND_GRADIENT_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_BACKGROUND_GRADIENT_VIEW_H_

#import <Cocoa/Cocoa.h>

@interface BackgroundGradientView : NSView {
 @private
  BOOL showsDivider_;
}

@property(nonatomic, assign) BOOL showsDivider;

- (NSColor*)strokeColor;

- (void)drawBackgroundWithOpaque:(BOOL)opaque;

@end

#endif  
