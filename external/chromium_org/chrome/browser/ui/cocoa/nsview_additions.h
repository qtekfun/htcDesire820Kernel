// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_NSVIEW_ADDITIONS_H_
#define CHROME_BROWSER_UI_COCOA_NSVIEW_ADDITIONS_H_

#import <Cocoa/Cocoa.h>

@interface NSView (ChromeAdditions)

- (CGFloat)cr_lineWidth;

- (BOOL)cr_isMouseInView;

- (BOOL)cr_isBelowView:(NSView*)otherView;

- (BOOL)cr_isAboveView:(NSView*)otherView;

- (void)cr_ensureSubview:(NSView*)subview
            isPositioned:(NSWindowOrderingMode)place
              relativeTo:(NSView *)otherView;

- (NSColor*)cr_keyboardFocusIndicatorColor;

- (void)cr_recursivelySetNeedsDisplay:(BOOL)flag;

@end

#endif  
