// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_IMAGE_BUTTON_CELL_H_
#define CHROME_BROWSER_UI_COCOA_IMAGE_BUTTON_CELL_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"

namespace image_button_cell {

enum ButtonState {
  kDefaultState = 0,
  kHoverState,
  kPressedState,
  kDisabledState,
  
  kDefaultStateBackground,
  kHoverStateBackground,
  kButtonStateCount
};

} 

@protocol ImageButton
@optional
- (void)mouseInsideStateDidChange:(BOOL)isInside;
@end

@interface ImageButtonCell : NSButtonCell {
 @private
  struct {
    
    int imageId;
    base::scoped_nsobject<NSImage> image;
  } image_[image_button_cell::kButtonStateCount];
  BOOL isMouseInside_;
}

@property(assign, nonatomic) BOOL isMouseInside;

- (NSImage*)imageForState:(image_button_cell::ButtonState)state
                     view:(NSView*)controlView;

- (void)setImageID:(NSInteger)imageID
    forButtonState:(image_button_cell::ButtonState)state;

- (void)setImage:(NSImage*)image
  forButtonState:(image_button_cell::ButtonState)state;

- (CGFloat)imageAlphaForWindowState:(NSWindow*)window;

- (void)drawFocusRingWithFrame:(NSRect)cellFrame inView:(NSView*)controlView;

@end

#endif 
