// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_CONFIRM_BUBBLE_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_CONFIRM_BUBBLE_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"

class ConfirmBubbleModel;

@interface ConfirmBubbleController :
    NSViewController<NSTextViewDelegate> {
 @private
  NSView* parent_;  
  CGPoint origin_;
  scoped_ptr<ConfirmBubbleModel> model_;
}

- (id)initWithParent:(NSView*)parent
              origin:(CGPoint)origin
               model:(ConfirmBubbleModel*)model;

- (NSPoint)origin;
- (NSString*)title;
- (NSString*)messageText;
- (NSString*)linkText;
- (NSString*)okButtonText;
- (NSString*)cancelButtonText;
- (BOOL)hasOkButton;
- (BOOL)hasCancelButton;
- (NSImage*)icon;

- (void)accept;
- (void)cancel;
- (void)linkClicked;

@end

#endif  
