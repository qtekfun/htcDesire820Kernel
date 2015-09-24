// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_CONFIRM_BUBBLE_COCOA_H_
#define CHROME_BROWSER_UI_COCOA_CONFIRM_BUBBLE_COCOA_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"

@class ConfirmBubbleController;
class ConfirmBubbleModel;

@interface ConfirmBubbleCocoa : NSView<NSTextViewDelegate> {
 @private
  NSView* parent_;  
  ConfirmBubbleController* controller_;  

  
  base::scoped_nsobject<NSImageView> icon_;
  base::scoped_nsobject<NSTextView> titleLabel_;
  base::scoped_nsobject<NSTextView> messageLabel_;
  base::scoped_nsobject<NSButton> okButton_;
  base::scoped_nsobject<NSButton> cancelButton_;
}

- (id)initWithParent:(NSView*)parent
          controller:(ConfirmBubbleController*)controller;

@end

@interface ConfirmBubbleCocoa (ExposedForUnitTesting)
- (void)clickOk;
- (void)clickCancel;
- (void)clickLink;
@end

#endif  
